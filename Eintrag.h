#ifndef EINTRAG_H
#define EINTRAG_H

#include <QString>
#include <QSqlRecord>
#include <QHash>
#include <QTableWidgetItem>

#include "File.h"

class Eintrag
{
public:
    Eintrag();
    Eintrag(const int id, const QString& name, const QString& writer, const QString& style, const QString& comment, const QString& fach);
    Eintrag(const QSqlRecord& rec);

    Eintrag(const Eintrag& ) = default;
    Eintrag& operator = (const Eintrag& ) = default;

    Eintrag(Eintrag&& ) = default;
    Eintrag& operator = (Eintrag&& ) = default;

    int GetId() const { return m_id; }

    QString GetFach() const { return m_fach; }
    void SetFach(const QString& fach) { m_fach = fach; m_changed = true; }

    QString GetName() const { return m_name; }
    void SetName(const QString& name) { m_name = name; m_changed = true; }

    QString GetWriter() const { return m_writer; }
    void SetWriter(const QString& writer) { m_writer = writer; m_changed = true; }

    QString GetStyle() const { return m_style; }
    void SetStyle(const QString& style) { m_style = style; m_changed = true; }

    QString GetComment() const { return m_comment; }
    void SetComment(const QString& comment) { m_comment = comment; m_changed = true; }

    bool IsChanged() const { return m_changed; }

    void Saved() { m_changed = false; m_new = false; }

    File* GetFile() { return &m_files[item]; }

    void ShowFiles(QTableWidget* table);
    void GenerateItems(QTableWidget* table);

    QString GetQueryString() const;
    QString GetDeleteQuery() const;
    void Delete();

private:
    int m_id;
    QString m_fach,m_name, m_writer, m_style, m_comment;

    bool m_new, m_changed;

    QHash<int, File> m_files;
    QHash<int, QHash<int, QTableWidgetItem*>> m_items;
};

#endif // EINTRAG_H
