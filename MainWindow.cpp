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
    connect(ui->toolbarSave, SIGNAL(triggered()), this, SLOT(SaveCurrent()));
    connect(ui->liste, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(ItemChanged(QListWidgetItem*)));

    LoadItems();

    qDebug() << "Time Elapsed: " << t.elapsed();
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

    return true;
}

bool MainWindow::SaveCurrent() {
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

    qDebug() << __func__ << " : " << model->query().lastQuery();

    delete model;
}
