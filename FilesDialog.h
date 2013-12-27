#ifndef FILESDIALOG_H
#define FILESDIALOG_H

#include <QDialog>

namespace Ui {
class FilesDialog;
}

class FilesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FilesDialog(QWidget *parent = 0);
    ~FilesDialog();

    void Set

private:
    Ui::FilesDialog *ui;
};

#endif // FILESDIALOG_H
