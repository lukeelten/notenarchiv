#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QHash>

#include "Eintrag.h"

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
    void LoadItems();

public slots:
    void ShowAbout();
    bool SaveCurrent();
    bool SaveAll();

    void ItemChanged(QListWidgetItem* item);

private:
    Ui::MainWindow *ui;
    bool m_changed;

    QHash<QListWidgetItem*, Eintrag> m_items;

};

#endif // MAINWINDOW_H
