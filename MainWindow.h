#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>

class QSqlTableModel;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *);

public slots:
    void ShowAbout();
    bool SaveCurrent();
    bool SaveAll();

    void ItemChanged(QModelIndex index);

private:
    Ui::MainWindow *ui;
    bool m_changed;
    QSqlTableModel *m_model;
};

#endif // MAINWINDOW_H
