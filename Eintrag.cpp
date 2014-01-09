#include <QSqlRecord>
#include <QString>
#include <QDebug>

#include "Eintrag.h"

Eintrag::Eintrag() : m_changed(false), m_row(-2) {
    qDebug() << __func__ << " : " << "call of invalud constructor";
}

Eintrag::Eintrag(const QSqlRecord& empty_rec) : m_rec(empty_rec), m_changed(false), m_row(-1) {
    m_rec.clearValues();
    if (m_rec.contains("id"))
        m_rec.remove(m_rec.indexOf("id"));
}

Eintrag::Eintrag(const QString& name, const QString& writer, const QString& style, const QString& comment, const QString& fach, const QSqlRecord& empty_rec) : m_rec(empty_rec), m_changed(false), m_row(-1) {
    m_rec.clearValues();

    if (m_rec.contains("id"))
        m_rec.remove(m_rec.indexOf("id"));

    m_rec.setValue("name", name);
    m_rec.setValue("komponist", writer);
    m_rec.setValue("richtung", style);
    m_rec.setValue("bemerkung", comment);
    m_rec.setValue("fach", fach);
}

Eintrag::Eintrag(const QSqlRecord& rec, const int row) : m_rec(rec), m_changed(false), m_row(row) {
}

int Eintrag::GetId() const {
    if (m_rec.isNull("id"))
        return -1;

    return m_rec.value("id").toInt();
}

