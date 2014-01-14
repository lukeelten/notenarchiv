#include <QPrinter>
#include <QPrintDialog>
#include <QString>
#include <QList>
#include <QSqlTableModel>
#include <QTextDocument>
#include <QSqlRecord>
#include <QDebug>

#include "Printer.h"
#include "Database.h"

Printer::Printer() : m_printer(new QPrinter(QPrinter::HighResolution)), m_model(new QSqlTableModel(0, DB->GetDatabase())), m_doc(new QTextDocument) {
    html_before = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">";
    html_before += "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">";
    html_before += "p, li { white-space: pre-wrap; } .table th { text-align: left; } .table span { font-size: 14pt; }";
    html_before += "</style></head><body style=\" font-family:'Tahoma'; font-size:14pt; font-weight:400; font-style:normal;\">";
    html_before += "<table class=\"table\"><tr><td style=\"width:40%;\"><p><span>Name</span></p></td><td style=\"width: 20%;\">";
    html_before += "<p><span>Fach-Nr</span></p></td><td style=\"width: 40%;\"><p><span>Komponist</span></p></td></tr>";
    
    html_row = "<tr><td><p><span>%1</span></p></td><td><p><span>%2</span></p></td><td><p><span>%3</span></p></td></tr>";

    html_after = "</table></body></html>";

}

Printer::~Printer() {
    if (m_printer) {
        delete m_printer;
        m_printer = nullptr;
    }

    if (m_model) {
        delete m_model;
        m_model = nullptr;
    }

    if (m_doc) {
        delete m_doc;
        m_doc = nullptr;
    }
}

bool Printer::PreparePrinter() {
    QPrintDialog dia(m_printer);

    dia.setOption(QPrintDialog::None);
    if (dia.exec() == QDialog::Accepted) {
        m_printer->setPageSize(QPrinter::A4);
        m_printer->setOrientation(QPrinter::Portrait);
        m_printer->setPageMargins(2, 2, 2, 2, QPrinter::Millimeter);
        m_printer->setFullPage(true);

        return true;
    }

    return false;
}

void Printer::PrepareTable() {
    m_model->setTable("notenarchiv");
    m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_model->setSort(1, Qt::AscendingOrder);

    m_model->select();

    while (m_model->canFetchMore())
        m_model->fetchMore();
}

void Printer::PrepareDocument() {
    m_text = html_before;

    int nums = m_model->rowCount();
    
    qDebug() << Q_FUNC_INFO << " : " << "Rows:" << nums;

    for (int i = 0; i < nums; i++) {
        QSqlRecord rec = m_model->record(i);

        m_text.append(html_row.arg(rec.value("name").toString(), rec.value("fach").toString(), rec.value("komponist").toString()));
    }

    m_text += html_after;

    m_doc->setHtml(m_text);
}

void Printer::Print() {
    if (!PreparePrinter())
        return;

    PrepareTable();
    PrepareDocument();

    m_doc->print(m_printer);
}
