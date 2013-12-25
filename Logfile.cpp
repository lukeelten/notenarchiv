#include <QString>
#include <QSqlQuery>
#include <QHash>
#include <QDebug>
#include <QVariant>

#include "Logfile.h"

const QString Logfile::QUERY = "INSERT INTO log (msg, level) VALUES (?, ?)";
const QHash<Logfile::LOGLEVEL, QVariant> Logfile::TRANSLATES = { {Logfile::DEBUG, "Debug"}, {Logfile::INFO, "Info"}, {Logfile::WARN, "Info"}, {Logfile::ERROR, "Info"}, {Logfile::FATAL, "Info"} };


Logfile::Logfile() : m_query(DB->GetEmptyQuery()) {
    m_query.exec("CREATE TABLE \"log\" (\"id\" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , \"msg\" TEXT, \"level\" VARCHAR NOT NULL  DEFAULT info, \"time\" DATETIME NOT NULL  DEFAULT CURRENT_TIMESTAMP)");
    if (m_query.lastError().isValid())
        qDebug() << "Logfile Konstruktor:" << m_query.lastError();
}

void Logfile::Write(const QString& msg, LOGLEVEL level) {
    //m_query.prepare(Logfile::QUERY);

    //m_query.addBindValue("Dies ist ein Test");
    //m_query.addBindValue("info");

    m_query.exec("insert into log (msg, level) values (\"dies ist ein test\", \"info\")");

    if (m_query.lastError().isValid())
        qDebug() << "Logfile:" << m_query.lastError();
}
