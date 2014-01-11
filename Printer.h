#ifndef PRINTER_H
#define PRINTER_H

#include <QString>

class QPrinter;
class QSqlTableModel;
class QTextDocument;

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

//protected:
    void PrepareTable();
    bool PreparePrinter();
    void PrepareDocument();

private:
    QPrinter* m_printer;
    QSqlTableModel* m_model;
    QTextDocument* m_doc;

    QString m_text;

    QString html_before, html_after, html_row;
};

#endif // PRINTER_H
