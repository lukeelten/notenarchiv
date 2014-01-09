#include <QMessageBox>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QModelIndex>
#include <QSqlRecord>
#include <QDebug>
#include <QTime>

#include "AboutDialog.h"
#include "Database.h"
#include "Logfile.h"
#include "Eintrag.h"

#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_changed(false)
{
    QTime t = QTime::currentTime();

    m_model = new QSqlTableModel(this, DB->GetDatabase());
    m_model->setTable("notenarchiv");
    m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_model->setSort(1, Qt::AscendingOrder);

    ui->setupUi(this);

    // Manuelle konnektierungen
    connect(ui->menuAbout, SIGNAL(triggered()), this, SLOT(ShowAbout()));
    connect(ui->toolbarSave, SIGNAL(triggered()), this, SLOT(SaveAll()));
    connect(ui->toolbarDelete, SIGNAL(triggered()), this, SLOT(ItemDelete()));
    //connect(ui->liste, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(ItemChanged(QListWidgetItem*)));
    connect(ui->liste, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(CurrentItemChanged(QListWidgetItem*,QListWidgetItem*)));
    connect(ui->textName, SIGNAL(textEdited(QString)), this, SLOT(NameChanged(QString)));
    connect(ui->textNumber, SIGNAL(textEdited(QString)), this, SLOT(NumberChanged(QString)));
    connect(ui->textStyle, SIGNAL(textEdited(QString)), this, SLOT(StyleChanged(QString)));
    connect(ui->textWriter, SIGNAL(textEdited(QString)), this, SLOT(WriterChanged(QString)));
    connect(ui->textComment, SIGNAL(textChanged()), this, SLOT(CommentChanged()));
    connect(ui->toolbarNew, SIGNAL(triggered()), this, SLOT(Add()));
    connect(ui->buttonSearch, SIGNAL(clicked()), this, SLOT(SearchClicked()));
    connect(ui->textSearch, SIGNAL(textEdited(QString)), this, SLOT(SearchTextChanged(QString)));
    connect(ui->textSearch, SIGNAL(returnPressed()), this, SLOT(SearchClicked()));

    LoadItems();

    qDebug() << "Time for construction: " << t.elapsed();
}

MainWindow::~MainWindow()
{    
    delete ui;
    delete m_model;
}

void MainWindow::ShowAbout() {
    AboutDialog about(this);
    about.exec();
}

void MainWindow::closeEvent(QCloseEvent * event) {
    bool close = false;

    QMessageBox msg;
    msg.setText("Das Notenarchiv wurde verändert.");
    msg.setInformativeText("Möchten Sie die Änderungen speichern?");
    msg.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msg.setIcon(QMessageBox::Question);

    int ret = QMessageBox::Discard;

    if (m_changed) {
        ret = msg.exec();
    }

    switch (ret) {
    case QMessageBox::Save:
        if (SaveAll()) {
            close = true;
        } else {
            msg.setText("Es ist ein Fehler beim Speichern aufgetreten.");
            msg.setInformativeText("Möchten Sie das Programm trotzdem schließen?");
            msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msg.setIcon(QMessageBox::Critical);

            int ret2 = msg.exec();

            if (ret2 == QMessageBox::Yes)
                close = true;
        }
        break;

    case QMessageBox::Discard:
        close = true;
        break;

    case QMessageBox::Cancel:
    default:
        break;

    }

    if (close) {
        event->accept();
    } else {
        event->ignore();
    }
}

bool MainWindow::SaveAll() {
    if (!m_changed)
        return true;

    bool submit = false;
    for (auto iter = m_items.begin(); iter != m_items.end(); iter++) {
        if (iter.value().IsChanged()) {
            m_model->setRecord(iter.value().GetRow(), iter.value().GetRecord());
            iter.value().Saved();
            iter.key()->setText(iter.value().GetName());
            QFont f = iter.key()->font();
            f.setItalic(false);
            iter.key()->setFont(f);
            submit = true;
        }
    }

    if (submit) {
        if (!m_model->submitAll()) {
            if (m_model->lastError().isValid())
                qDebug() << Q_FUNC_INFO << " : " << m_model->lastError();

            return false;
        }
    }

    m_changed = false;
    return true;
}

void MainWindow::ItemChanged(QListWidgetItem* item) {
    if (!item) {
        ui->textName->clear();
        ui->textNumber->clear();
        ui->textComment->clear();
        ui->textStyle->clear();
        ui->textWriter->clear();
        ui->liste->clearSelection();
        qDebug() << Q_FUNC_INFO << " : " << "nullptr";

        return;
    }

    if (!m_items.contains(item)) {
        qDebug() << Q_FUNC_INFO << " : " << " Unknown item";
        return;
    }

    // Gehört zum alten Such-Algo
    /*
    if (item->isHidden()) {
        int row = 0;

        while (item->isHidden()) {
            item = ui->liste->item(row);
            row++;

            if (row > ui->liste->count() || !item) {
                ItemChanged();
                return;
            }
        }
    }
    */

    if (ui->liste->row(item) < 0)
        item = ui->liste->item(0);

    ui->textName->setText(m_items[item].GetName());
    ui->textNumber->setText(m_items[item].GetFach());
    ui->textComment->setPlainText(m_items[item].GetComment());
    ui->textStyle->setText(m_items[item].GetStyle());
    ui->textWriter->setText(m_items[item].GetWriter());

    ui->liste->setCurrentItem(item);
}

void MainWindow::LoadItems() {
    m_items.clear();
    ui->liste->clear();


    m_model->select();

    if (m_model->lastError().isValid()) {
        qDebug() << Q_FUNC_INFO << " : " << m_model->lastError();
        return;
    }

    while (m_model->canFetchMore())
        m_model->fetchMore();


    QListWidgetItem* item = nullptr;
    int numrows = m_model->rowCount();

    for (int i = 0; i < numrows; i++) {
        QSqlRecord record = m_model->record(i);

        if (record.isEmpty())
            break;

        Eintrag noten(record, i);

        item = new QListWidgetItem(noten.GetName(), ui->liste);

        m_items.insert(item, std::move(noten));

        item = nullptr;
    }

    ItemChanged(ui->liste->item(0));
}

void MainWindow::ShowItems(const QString& filter) {
    qDebug() << Q_FUNC_INFO;
    QListWidgetItem* show = ui->liste->currentItem();


    for (QHash<QListWidgetItem*, Eintrag>::iterator i = m_items.begin(); i != m_items.end(); i++) {

        // Alter Such Algo
        /*
        i.key()->setHidden(true);

        bool add = false;

        if (!filter.isEmpty()) {
            QString name = i.value().GetName();
            if (name.contains(filter, Qt::CaseInsensitive))
                add = true;
        } else
            add = true;

        if (add) {
            i.key()->setHidden(false);
        }
        */

        QString name = i.value().GetName();
        if (!name.contains(filter, Qt::CaseInsensitive)) {
            int row = ui->liste->row(i.key());
            if (row >= 0)
                ui->liste->takeItem(row);
        } else {
            if (ui->liste->row(i.key()) < 0)
                ui->liste->addItem(i.key());
        }

    }

    ItemChanged(show);
}

void MainWindow::CommentChanged() {
    QListWidgetItem* current = ui->liste->currentItem();

    if (!current)
        return;

    if (m_items[current].GetComment() == ui->textComment->toPlainText())
        return;

    ChangeItemStyle(current);

    m_items[current].SetComment(ui->textComment->toPlainText());
    m_changed = true;
}

void MainWindow::NameChanged(const QString &) {
    QListWidgetItem* current = ui->liste->currentItem();

    if (!current)
        return;

    if (m_items[current].GetName() == ui->textName->text())
        return;

    m_items[current].SetName(ui->textName->text());
    current->setText(ui->textName->text());
    ChangeItemStyle(current, true);

    m_changed = true;
}

void MainWindow::NumberChanged(const QString &) {
    QListWidgetItem* current = ui->liste->currentItem();

    if (!current)
        return;

    if (m_items[current].GetFach() == ui->textNumber->text())
        return;

    ChangeItemStyle(current);

    m_items[current].SetFach(ui->textNumber->text());
    m_changed = true;
}

void MainWindow::StyleChanged(const QString &) {
    QListWidgetItem* current = ui->liste->currentItem();

    if (!current)
        return;

    if (m_items[current].GetStyle() == ui->textStyle->text())
        return;

    ChangeItemStyle(current);

    m_items[current].SetStyle(ui->textStyle->text());
    m_changed = true;
}

void MainWindow::WriterChanged(const QString &) {
    QListWidgetItem* current = ui->liste->currentItem();

    if (!current)
        return;

    if (m_items[current].GetWriter() == ui->textWriter->text())
        return;

    ChangeItemStyle(current);

    m_items[current].SetWriter(ui->textWriter->text());
    m_changed = true;
}

void MainWindow::ChangeItemStyle(QListWidgetItem *item, bool anywhere) {
    if (!m_items[item].IsChanged() || anywhere) {
        item->setText(item->text() + " *");
        QFont font = item->font();
        font.setItalic(true);
        item->setFont(font);
    }
}

void MainWindow::Add() {
    QListWidgetItem* item = new QListWidgetItem("Neuer Eintrag", ui->liste);

    Eintrag e(m_model->record(0));
    e.SetName("Neuer Eintrag");

    m_items.insert(item, e);

    ui->liste->setCurrentItem(item);
    ItemChanged(item);
    ChangeItemStyle(item, true);
}

void MainWindow::ItemDelete() {
    QListWidgetItem* current = ui->liste->currentItem();

    QMessageBox msg;
    msg.setIcon(QMessageBox::Question);
    msg.setText("Löschen bestätigen");
    msg.setInformativeText("Möchten Sie den Eintrag " + m_items[current].GetName() + " wirklich löschen?");
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

    int ret = msg.exec();

    if (ret == QMessageBox::No)
        return;

    if (!m_items.contains(current)) {
        qDebug() << Q_FUNC_INFO << " : " << "Invalid Item";
        return;
    }

    if (m_items[current].GetRow() >= 0) {
        if (!m_model->removeRows(m_items[current].GetRow(), 1)) {
            if (m_model->lastError().isValid())
                qDebug() << Q_FUNC_INFO << " : " << m_model->lastError();

            QMessageBox::information(this, "Löschen fehlgeschlagen", "Das Löschen des ausgewählten Elements ist fehlgeschlagen.", QMessageBox::Ok, QMessageBox::Ok);
            return;
        }
    }
    current->setHidden(true);

    m_items.remove(current);
    ui->liste->removeItemWidget(current);

    if(current)
        delete current; // Bin nicht sicher ob ich das darf
}

void MainWindow::SearchClicked() {
    ShowItems(ui->textSearch->text());
}

void MainWindow::SearchTextChanged(const QString& ) {
    if (ui->textSearch->text().isEmpty()) {
        QListWidgetItem* selection = ui->liste->currentItem();

        ShowItems(QString());
        ItemChanged(selection);
    }
}
