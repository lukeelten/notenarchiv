#include <QSqlRecord>
#include <QSqlTableModel>
#include <QString>
#include <QVariant>
#include <QTableWidget>
#include <QFileInfo>
#include <QTableWidgetItem>
#include <QTime>
#include <utility>

#include "Eintrag.h"
#include "Database.h"

Eintrag::Eintrag() : m_id(-1), m_fach(), m_name(), m_writer(), m_style(), m_comment(), m_new(true), m_changed(false) {
    LoadFiles();
}

Eintrag::Eintrag(const int id, const QString &name, const QString &writer, const QString &style, const QString &comment, const QString& fach) : m_id(id),
    m_fach(fach), m_name(name), m_writer(writer), m_style(style), m_comment(comment), m_new(false), m_changed(false)
{
    LoadFiles();
}

Eintrag::Eintrag(const QSqlRecord& rec) : m_id(rec.value("id").toInt()),
    m_fach(rec.value("fach").toString()),
    m_name(rec.value("name").toString()),
    m_writer(rec.value("komponist").toString()),
    m_style(rec.value("richtung").toString()),
    m_comment(rec.value("bemerkung").toString()),
    m_new(false),
    m_changed(false)
{
    LoadFiles();
}

QString Eintrag::GetQueryString() const {
    QString q;

    if (m_new) {
        q = "INSERT INTO notenarchiv (name, komponist, richtung, bemerkung, fach) VALUES (\"" + m_name + "\", \"" + m_writer + "\", \"" + m_style + "\", \"" + m_comment + "\", \"" + m_fach + "\")";
    } else {
        q = "UPDATE notenarchiv SET name = \"" + m_name + "\", komponist = \"" + m_writer + "\", richtung = \"" + m_style + "\", bemerkung = \"" + m_comment + "\", fach = \"" + m_fach + "\" WHERE id = ";
        q.append(QVariant(m_id).toString());
    }

    return std::move(q);
}

QString Eintrag::GetDeleteQuery() const {
    if (m_new)
        return QString();

    QString q =  "DELETE FROM notenarchiv WHERE id = ";
    q.append(QVariant(m_id).toString());

    return std::move(q);
}

void Eintrag::ShowFiles(QTableWidget *table) {
    table->clear();
    table->setRowCount(m_items.size());

    if (m_items.size() <= 0 || m_items.isEmpty())
        return;

    for (auto row = m_items.begin(); row != m_items.end(); row++) {
        for (auto column = row.value().begin(); column != row.value().end(); column++) {

            table->setItem(row.key(), column.key(), column.value());

        }
    }
}

void Eintrag::GenerateItems(QTableWidget *table) {
    QTime t = QTime::currentTime();

    if (table)
        table->clear(); // Sicherstellen das TableWidget nicht auf die zu zerstörenden Items zugreift
    Delete();

    QTableWidgetItem* item = nullptr;
    QMap<int, QTableWidgetItem*> column;
    int row = 0;

    for (auto ele = m_files.begin(); ele != m_files.end(); ele++, row++) {
        item = new QTableWidgetItem(ele.value().GetInstrument());
        item->setData(Qt::UserRole, ele.key());
        column.insert(0, item);

        item = new QTableWidgetItem(ele.value().GetPart());
        item->setData(Qt::UserRole, ele.key());
        column.insert(1, item);

        QFileInfo info(ele.value().GetFile());
        item = new QTableWidgetItem(info.fileName());
        item->setData(Qt::UserRole, ele.key());
        column.insert(2, item);

        m_items.insert(row, column);
        column.clear();

        item = nullptr;
    }

    qDebug() << __func__ << " : " << t.elapsed();
}

void Eintrag::Delete() {
    if (!m_items.isEmpty()) {
        for (auto row = m_items.begin(); row != m_items.end(); row++) {
            for (auto column = row.value().begin(); column != row.value().end(); column++) {
                if (column.value()) {
                    delete column.value();
                    column.value() = nullptr;
                }
            }
        }
        m_items.clear();
    }
}

void Eintrag::LoadFiles() {
    QSqlTableModel *model = new QSqlTableModel(nullptr, DB->GetDatabase());

    model->setTable("files");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setFilter("noten_id = " + QVariant(m_id).toString());
    model->setSort(2, Qt::AscendingOrder);
    model->select();

    if (model->lastError().isValid()) {
        qDebug() << __func__ << " : " << model->lastError();
        delete model;
        return;
    }
    if (model->rowCount() == 0) {
        delete model;
        return;
    }

    QSqlRecord rec;
    int rows = model->rowCount();

    for (int i = 0; i < rows; i++) {
        rec = model->record(i);

        if (rec.isEmpty())
            break;

        File f(rec);
        m_files.insert(i, std::move(f));
    }

}
