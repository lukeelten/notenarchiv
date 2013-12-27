#include "FilesDialog.h"
#include "ui_FilesDialog.h"

FilesDialog::FilesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FilesDialog)
{
    ui->setupUi(this);
}

FilesDialog::~FilesDialog()
{
    delete ui;
}
