#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QSqlTableModel>
#include <QHash>
#include <QDebug>

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
    void ShowItems(const QString& filter);

public slots:
    void ShowAbout();
    bool SaveAll();
    void Add();

    void ItemChanged(QListWidgetItem* item = nullptr);
    void CurrentItemChanged(QListWidgetItem* item, QListWidgetItem* ) { ItemChanged(item); }
    void CommentChanged();
    void NameChanged(const QString& );
    void NumberChanged(const QString& );
    void StyleChanged(const QString& );
    void WriterChanged(const QString& );
    void SearchClicked();
    void SearchTextChanged(const QString& );

    void ItemDelete();

    void FileTableChanged(QTableWidgetItem* current, QTableWidgetItem* ) { qDebug() << __func__ << " : " << current; }

private:
    Ui::MainWindow *ui;
    bool m_changed;

    QHash<QListWidgetItem*, Eintrag> m_items;
    QSqlTableModel* m_model;
};

#endif // MAINWINDOW_H
