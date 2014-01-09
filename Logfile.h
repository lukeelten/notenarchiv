#ifndef LOGFILE_H
#define LOGFILE_H

#include <QString>
#include <QHash>
#include <QVariant>
#include <QSqlQuery>

#include "Database.h"

#include "Singleton.h"

class Logfile : public Singleton<Logfile>
{
public:
    enum LOGLEVEL {
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL
    };

    virtual ~Logfile() = default;
    Logfile();

private:
    // Kopieren verboten
    Logfile (const Logfile& ) = delete;
    Logfile& operator = (const Logfile& ) = delete;

    // std::move verboten
    Logfile (const Logfile&& ) = delete;
    Logfile& operator = (const Logfile&& ) = delete;

public:
    void Write(const QString& msg, LOGLEVEL level);

    void Debug(const QString& msg) { Write(msg, Logfile::DEBUG); }
    void Info(const QString& msg) { Write(msg, Logfile::INFO); }
    void Warn(const QString& msg) { Write(msg, Logfile::WARN); }
    void Error(const QString& msg) { Write(msg, Logfile::ERROR); }
    void Fatal(const QString& msg) { Write(msg, Logfile::FATAL); }

private:
    static const QString QUERY;
    static const QHash<Logfile::LOGLEVEL, QVariant> TRANSLATES;

    QSqlQuery m_query;

};

#define Log Logfile::Get()

#endif // LOGFILE_H
