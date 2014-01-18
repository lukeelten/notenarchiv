#ifndef PRINTER_H
#define PRINTER_H

#include <QString>
#include <QHash>

#include "Eintrag.h"

class QPrinter;
class QSqlTableModel;
class QTextDocument;
class QListWidgetItem;

class Printer
{
public:
    Printer();
    ~Printer();

    // Copy verboten
    Printer (const Printer& ) = delete;
    Printer& operator = (const Printer& ) = delete;

    // Move verboten
    Printer (Printer&& ) = delete;
    Printer& operator = (Printer&& ) = delete;

    QString GetString() const { return m_text; }

    void Print(QSqlTableModel* model = nullptr);

protected:
    bool PreparePrinter();
    void PrepareDocument(QSqlTableModel* model);
    void InitTableModel();

private:
    QPrinter* m_printer;
    QSqlTableModel* m_model;
    QTextDocument* m_doc;

    QString m_text;
    QString html_before, html_after, html_row;
};

#endif // PRINTER_H
