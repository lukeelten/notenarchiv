#include "Updater.h"
#include "ui_Updater.h"

Updater::Updater(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Updater)
{
    ui->setupUi(this);
}

Updater::~Updater()
{
    delete ui;
}
