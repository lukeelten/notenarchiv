#include <QString>

#include "AboutDialog.h"
#include "ui_AboutDialog.h"

extern QString GetVersion(); // defined in main.cpp

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    QString html = ui->textEdit->toHtml();

    html.replace("VX.X.X", GetVersion());
    ui->textEdit->setHtml(html);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
