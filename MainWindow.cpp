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
                qDebug() << __func__ << " : " << m_model->lastError();

            return false;
        }
    }

    m_changed = false;
    return true;
}

void MainWindow::ItemChanged(QListWidgetItem* item) {
    if (!m_items.contains(item)) {
        qDebug() << __func__ << " : " << " Unknown item";
        return;
    }

    ui->textName->setText(m_items[item].GetName());
    ui->textNumber->setText(m_items[item].GetFach());
    ui->textComment->setPlainText(m_items[item].GetComment());
    ui->textStyle->setText(m_items[item].GetStyle());
    ui->textWriter->setText(m_items[item].GetWriter());
}

void MainWindow::LoadItems() {
    ui->liste->clear();

    m_model->setSort(1, Qt::AscendingOrder);
    m_model->select();

    if (m_model->lastError().isValid()) {
        qDebug() << __func__ << " : " << m_model->lastError();
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

        if (i == 0)
            ItemChanged(item);

        item = nullptr;
    }

    qDebug() << sizeof(m_items);
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
        qDebug() << __func__ << " : " << "Invalid Item";
        return;
    }

    if (m_items[current].GetRow() >= 0) {
        if (!m_model->removeRows(m_items[current].GetRow(), 1)) {
            if (m_model->lastError().isValid())
                qDebug() << __func__ << " : " << m_model->lastError();

            QMessageBox::information(this, "Löschen fehlgeschlagen", "Das Löschen des ausgewählten Elements ist fehlgeschlagen.", QMessageBox::Ok, QMessageBox::Ok);
            return;
        }
    }

    m_items.remove(current);
    ui->liste->removeItemWidget(current);

    if(current)
        delete current; // Bin nicht sicher ob ich das darf
}
