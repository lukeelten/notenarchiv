#include <QSql>
#include <QString>
#include <QDir>
#include <QDebug>
#include <QFile>
#include <utility>

#include "Database.h"


const QString Database::FILENAME = "Notenarchiv.sqlite";
const QString Database::DRIVER = "QSQLITE";


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
        qDebug() << "Database: " << m_db.lastError();

}

Database::~Database() {
    if (m_db.isOpen())
        m_db.close();
}

void Database::InitTables() {
}
