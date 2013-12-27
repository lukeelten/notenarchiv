#include <QString>
#include <QSqlRecord>
#include <QVariant>

#include "File.h"

File::File() : m_id(-1), m_notenid(-1), m_instrument(), m_part(), m_file(), m_changed(false), m_new(true), m_parentChanged(nullptr) {
}

File::File(const QSqlRecord &rec, bool *parentChanged) : m_id(rec.value("id").toInt()),
    m_notenid(rec.value("noten_id").toInt()),
    m_instrument(rec.value("instrument").toString()),
    m_part(rec.value("stimme").toString()),
    m_file(rec.value("file").toString()),
    m_changed(false),
    m_new(false)
{
    m_parentChanged = parentChanged;
}

QString File::GetQueryString() const {
    return "";
}

QString File::GetDeleteQuery() const {
    return "";
}
