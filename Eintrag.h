#ifndef EINTRAG_H
#define EINTRAG_H

#include <QString>
#include <QSqlRecord>
#include <QVariant>

#include "util.h"

class Eintrag
{
public:
    Eintrag();
    ~Eintrag() = default;

    Eintrag(const QSqlRecord& empty_rec);
    Eintrag(const QString& name, const QString& writer, const QString& style, const QString& comment, const QString& fach, const QSqlRecord& empty_rec);
    Eintrag(const QSqlRecord& rec, const int row);

    Eintrag(const Eintrag& ) = default;
    Eintrag& operator = (const Eintrag& ) = default;

    Eintrag(Eintrag&& ) = default;
    Eintrag& operator = (Eintrag&& ) = default;

    int GetId() const;

    QString GetFach() const { return util::toString(m_rec.value("fach")); }
    void SetFach(const QString& fach) { m_rec.setValue("fach", fach); m_changed = true; }

    QString GetName() const { return util::toString(m_rec.value("name")); }
    void SetName(const QString& name) { m_rec.setValue("name", name); m_changed = true; }

    QString GetWriter() const { return util::toString(m_rec.value("komponist"); }
    void SetWriter(const QString& writer) { m_rec.setValue("komponist", writer); m_changed = true; }

    QString GetStyle() const { return util::toString(m_rec.value("richtung"); }
    void SetStyle(const QString& style) { m_rec.setValue("richtung", style); m_changed = true; }

    QString GetComment() const { return util::toString(m_rec.value("bemerkung"); }
    void SetComment(const QString& comment) { m_rec.setValue("bemerkung", comment); m_changed = true; }

    bool IsChanged() const { return m_changed; }

    void Saved() { m_changed = false; }

    QSqlRecord GetRecord() const { return m_rec; }
    int GetRow() const { return m_row; }
    void SetRecord(const QSqlRecord& rec, const int row) { m_rec = rec; m_changed = false; m_row = row; }

private:
    QSqlRecord m_rec;

    bool m_changed;
    int m_row;
};

#endif // EINTRAG_H
