#ifndef FILE_H
#define FILE_H

#include <QString>
#include <QSqlRecord>

class File
{

    friend class Eintrag;
public:
    File();
    File(const QSqlRecord& rec, bool* parentChanged = nullptr);

    File(const File& ) = default;
    File& operator = (const File& ) = default;

    File(File&& ) = default;
    File& operator = (File&& ) = default;

    QString GetInstrument() const { return m_instrument; }
    void SetInstrument(const QString& instrument) { m_instrument = instrument; Changed(); }

    QString GetPart() const { return m_part; }
    void SetPart(const QString& part) { m_part = part; Changed(); }

    QString GetFile() const { return m_file; }
    void SetFile(const QString& file) { m_file = file; Changed(); }

    bool IsChanged() const { return m_changed; }
    void Saved() { m_changed = false; m_new = false; }

    void SetNotenId(int notenid) { m_notenid = notenid; Changed(); }

    QString GetQueryString() const;
    QString GetDeleteQuery() const;

private:
    inline void Changed() { m_changed = true; *m_parentChanged = true; }

    int m_id, m_notenid;
    QString m_instrument, m_part, m_file;

    bool m_changed, m_new;
    bool* m_parentChanged;
};

#endif // FILE_H
