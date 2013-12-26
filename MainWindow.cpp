#include <QMessageBox>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QModelIndex>
#include <QSqlRecord>
#include <QDebug>

#include "AboutDialog.h"
#include "Database.h"
#include "Logfile.h"

#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_changed(false),
    m_model(new QSqlTableModel(this, DB->GetDatabase()))
{
    ui->setupUi(this);

    // Manuelle konnektierungen
    connect(ui->menuAbout, SIGNAL(triggered()), this, SLOT(ShowAbout()));
    connect(ui->toolbarSave, SIGNAL(triggered()), this, SLOT(SaveCurrent()));
    connect(ui->liste, SIGNAL(clicked(QModelIndex)), this, SLOT(ItemChanged(QModelIndex)));

    //
    QSqlQuery q = DB->Query("SELECT * FROM notenarchiv");
    if (q.lastError().isValid())
        qDebug() << "MainWindow Konstruktor: " << q.lastError();

    qDebug() << "Num Rows: " << q.numRowsAffected();

    // Model laden
    m_model->setTable("notenarchiv");
    m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_model->setFilter("");
    m_model->setSort(1, Qt::AscendingOrder);

    m_model->select();
    m_model->setHeaderData(0, Qt::Horizontal, "ID", Qt::UserRole);
    m_model->setHeaderData(1, Qt::Horizontal, "Name");
    m_model->setHeaderData(2, Qt::Horizontal, "Komponist");
    m_model->setHeaderData(3, Qt::Horizontal, "Musikrichtung");
    m_model->setHeaderData(4, Qt::Horizontal, "Bemerkung");
    m_model->setHeaderData(5, Qt::Horizontal, "Fach-Nr.:");

    ui->liste->setModel(m_model);
    ui->liste->setModelColumn(1);
    ui->liste->show();

}

MainWindow::~MainWindow()
{
    delete m_model;

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

void MainWindow::ItemChanged(QModelIndex index) {
    qDebug() << "Entered ItemChanged";
    QSqlRecord rec = m_model->record(index.row());

    ui->textName->setText(rec.value("name").toString());
    ui->textNumber->setText(rec.value("fach").toString());
    ui->textWriter->setText(rec.value("komponist").toString());
    ui->textStyle->setText(rec.value("richtung").toString());
    ui->textComment->setPlainText(rec.value("bemerkung").toString());
}
