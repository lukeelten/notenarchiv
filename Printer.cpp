#include <QPrinter>
#include <QPrintDialog>
#include <QString>
#include <QList>
#include <QSqlTableModel>
#include <QTextDocument>
#include <QSqlRecord>
#include <QDebug>
#include <QTime>

#include "Printer.h"
#include "Database.h"

Printer::Printer() : m_printer(new QPrinter(QPrinter::HighResolution)), m_doc(new QTextDocument) {
    m_model = nullptr;

    html_before = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">";
    html_before += "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">";
    html_before += "p, li { white-space: pre-wrap; }";
    html_before += "</style></head>";
    html_before += "<body style=\" font-family:'Tahoma'; font-size:12pt; font-weight:400; font-style:normal;\">";
    // Tabelle
    html_before += "<table width=\"100%\" border=\"1\" cellpadding=\"2\" cellspacing=\"0\" style=\"border-color: #000000; border-style: solid;\">";
    
    html_before += "<thead><tr style=\"font-size: 14pt;\">";
    // Kopf
    html_before += "<th width=\"40%\" align=\"left\"><p><span>Name</span></p></td>";
    html_before += "<th width=\"20%\" align=\"center\"><p><span>Fach-Nr</span></p></td>";
    html_before += "<th width=\"40%\" align=\"left\"><p><span>Komponist</span></p></td>";
    
    html_before += "</tr></thead>";
    
    // Body
    html_before += "<tbody>";
    
    html_row = "<tr><td><p><span>%1</span></p></td><td align=\"center\"><p><span>%2</span></p></td><td><p><span>%3</span></p></td></tr>";

    html_after = "</tbody></table>";
    html_after += "</body></html>";

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

void Printer::InitTableModel() {
    m_model = new QSqlTableModel(0, DB->GetDatabase());
    m_model->setTable("notenarchiv");
    m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_model->setSort(1, Qt::AscendingOrder);
    m_model->select();

    while (m_model->canFetchMore())
        m_model->fetchMore();
}

void Printer::PrepareDocument(QSqlTableModel* model) {
    m_text = html_before;

    int nums = model->rowCount();

    for (int i = 0; i < nums; i++) {
        QSqlRecord rec = model->record(i);

        m_text.append(html_row.arg(rec.value("name").toString(), rec.value("fach").toString(), rec.value("komponist").toString()));
    }

    m_text += html_after;

    m_doc->setHtml(m_text);
}

void Printer::Print(QSqlTableModel* model) {
    if (!PreparePrinter())
        return;

    if (model)
        PrepareDocument(model);
    else {
        InitTableModel();
        PrepareDocument(m_model);
    }

    m_doc->print(m_printer);
}
