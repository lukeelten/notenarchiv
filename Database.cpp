#include <QSql>
#include <QString>
#include <QDir>
#include <QDebug>
#include <QFile>
#include <utility>

#include "Database.h"


const QString Database::FILENAME = "Notenarchiv.sqlite";
const QString Database::DRIVER = "QSQLITE";

//#define CONSOLE( msg ) qDebug() << __func__ << msg;

Database::Database() {
    //m_db = QSqlDatabase::addDatabase(Database::DRIVER);
    //QString path = QDir::currentPath();
    //path.append(QDir::separator()).append(Database::FILENAME);
    //m_db.setDatabaseName(path);

    qDebug() << "Pfad: " << QDir::currentPath();

    m_db = QSqlDatabase::addDatabase(Database::DRIVER);
    m_db.setDatabaseName(Database::FILENAME);

    if (QFile::exists(Database::FILENAME))
        qDebug() << "Datenbank existiert";
    else
        qDebug() << "Datenbank fehlt";

    m_db.open();

    if (m_db.lastError().isValid())
        qDebug() << __func__ << m_db.lastError();
    
    
    QSqlQuery q(m_db);
    q.exec("SELECT id FROM notenarchiv LIMIT 1");
    
    if (q.lastError().isValid())
        InitNotenarchiv();
    
    q.exec("SELECT id FROM log LIMIT 1");
    if (q.lastError().isValid())
        InitLog();

}

Database::~Database() {
    if (m_db.isOpen())
        m_db.close();
}

void Database::InitNotenarchiv() {
    QSqlQuery q("CREATE TABLE \"notenarchiv\" (\"id\" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , \"name\" VARCHAR, \"komponist\" VARCHAR, \"richtung\" VARCHAR, \"bemerkung\" TEXT, \"fach\" VARCHAR)", m_db); // Create notenarchiv
    
    if (q.lastError().isValid()) {
        qDebug() << __func__ << q.lastError();
    }
    
    qDebug() << __func__;
}

void Database::InitLog() {
    QSqlQuery q("CREATE TABLE \"log\" (\"id\" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , \"msg\" TEXT, \"level\" VARCHAR NOT NULL  DEFAULT info, \"time\" DATETIME NOT NULL  DEFAULT CURRENT_TIMESTAMP)", m_db); // Create log
    
    if (q.lastError().isValid()) {
        qDebug() << __func__ << q.lastError();
        return;
    }
    
    qDebug() << __func__;

}
