#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QStringList>
#include <utility>

#include "Singleton.h"


class Database : public Singleton<Database>
{

private:
    // Kopieren verboten
    Database (const Database& ) = delete;
    Database& operator = (const Database& ) = delete;

    // std::move verboten
    Database (const Database&& ) = delete;
    Database& operator = (const Database&& ) = delete;

public:
    virtual ~Database();
    Database();

    bool IsOpen () const { return m_db.isOpen(); }
    QSqlQuery GetEmptyQuery() { return std::move(QSqlQuery(m_db)); }
    QSqlQuery Query(const QString& query) { return std::move(QSqlQuery(query, m_db)); }

    QStringList GetTables() const { return m_db.tables(); }

private:
    void InitTables();

    QSqlDatabase m_db;

    static const QString DRIVER;
    static const QString FILENAME;

};

#define DB Database::Get()

#endif // DATABASE_H
