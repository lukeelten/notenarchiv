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
    connect(ui->files, SIGNAL(currentItemChanged(QTableWidgetItem*,QTableWidgetItem*)), this, SLOT(FileTableChanged(QTableWidgetItem*,QTableWidgetItem*)));

    LoadItems();

    qDebug() << "Time for construction: " << t.elapsed();
}

MainWindow::~MainWindow()
{
    delete ui;
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
    auto iter = m_items.begin();
    QSqlQuery q = DB->GetEmptyQuery();

    for (;iter != m_items.end(); iter++) {
        if (!iter.value().IsChanged())
            continue;

        q.exec(iter.value().GetQueryString());

        if (q.lastError().isValid()) {
            qDebug() << __func__ << " : " << q.lastError();
            return false;
        }

        iter.value().Saved();

        QFont f = iter.key()->font();
        f.setItalic(false);
        iter.key()->setFont(f);
        iter.key()->setText(iter.value().GetName());

    }

    m_changed = false;
    return true;
}

void MainWindow::ItemChanged(QListWidgetItem* item) {
    if (!m_items.contains(item)) {
        qDebug() << __func__ << " Unknown item";
        return;
    }

    ui->textName->setText(m_items[item].GetName());
    ui->textNumber->setText(m_items[item].GetFach());
    ui->textComment->setPlainText(m_items[item].GetComment());
    ui->textStyle->setText(m_items[item].GetStyle());
    ui->textWriter->setText(m_items[item].GetWriter());
}

void MainWindow::LoadItems() {

    QSqlTableModel* model = new QSqlTableModel(this, DB->GetDatabase());

    model->setTable("notenarchiv");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setSort(1, Qt::AscendingOrder);
    model->select();

    if (model->lastError().isValid()) {
        qDebug() << __func__ << " : " << model->lastError();
        return;
    }

    QListWidgetItem* item = nullptr;
    int numrows = model->rowCount();
    QSqlRecord record;

    for (int i = 0; i < numrows; i++) {
        record = model->record(i);

        if (record.isEmpty())
            break;

        item = new QListWidgetItem(record.value("name").toString(), ui->liste);

        Eintrag noten(record);
        m_items.insert(item, std::move(noten));

        if (i == 0)
            ItemChanged(item);

        item = nullptr;
    }

    QTableWidgetItem* item1 = new QTableWidgetItem("Item 1");
    QTableWidgetItem* item2 = new QTableWidgetItem("Item 2");
    QTableWidgetItem* item3 = new QTableWidgetItem("Item 3");

    ui->files->setRowCount(1);

    ui->files->setItem(0, 0, item1);
    ui->files->setItem(0, 1, item2);
    ui->files->setItem(0, 2, item3);

    ui->files->show();

    delete model;
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

    Eintrag e;
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

    if (!m_items[current].GetDeleteQuery().isEmpty()) {
        auto q = DB->GetEmptyQuery();
        qDebug() << __func__ << " : " << "SQL-Query: " << m_items[current].GetDeleteQuery();
        q.exec(m_items[current].GetDeleteQuery());

        if (q.lastError().isValid()) {
            qDebug() << __func__ << " : " << q.lastError();
            return;
        }
    }

    m_items[current].Delete();
    m_items.remove(current);
    ui->liste->removeItemWidget(current);

    if(current)
        delete current; // Bin nicht sicher ob ich das darf
}
