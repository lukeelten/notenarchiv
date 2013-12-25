#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private:
    Ui::MainWindow *ui;
    bool m_changed;
};

#endif // MAINWINDOW_H
