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
    void ChangeItemStyle(QListWidgetItem* item, bool anywhere = false);

public slots:
    void ShowAbout();
    bool SaveAll();
    void Add();

    void ItemChanged(QListWidgetItem* item);
    void CommentChanged();
    void NameChanged(const QString& );
    void NumberChanged(const QString& );
    void StyleChanged(const QString& );
    void WriterChanged(const QString& );

private:
    Ui::MainWindow *ui;
    bool m_changed;

    QHash<QListWidgetItem*, Eintrag> m_items;

};

#endif // MAINWINDOW_H
