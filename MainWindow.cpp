#include <QMessageBox>

#include "AboutDialog.h"
#include "Database.h"
#include "Logfile.h"

#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_changed(true)
{
    ui->setupUi(this);

    // Manuelle konnektierungen
    connect(ui->menuAbout, SIGNAL(triggered()), this, SLOT(ShowAbout()));
    connect(ui->toolbarSave, SIGNAL(triggered()), this, SLOT(SaveCurrent()));
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

    int ret = QMessageBox::Cancel;

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

    if (close) { // Wichtiger Punkt!!! Singletons löschen
        event->accept();
        DB->Del();
        Log->Del();
    } else {
        event->ignore();
    }
}

bool MainWindow::SaveAll() {
    Log->Info("Dies ist ein Test");

    return true;
}

bool MainWindow::SaveCurrent() {
    return true;
}
