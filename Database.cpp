#include <QSql>
#include <QString>
#include <QDir>
#include <QDebug>
#include <utility>

#include "Database.h"


const QString Database::FILENAME = "Notenarchiv.sqlite";
const QString Database::DRIVER = "QSQLITE";


Database::Database() {
    //m_db = QSqlDatabase::addDatabase(Database::DRIVER);
    //QString path = QDir::currentPath();
    //path.append(QDir::separator()).append(Database::FILENAME);
    //m_db.setDatabaseName(path);
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("Notenarchiv.sqlite");

    m_db.open();

    if (m_db.lastError().isValid())
        qDebug() << "Database: " << m_db.lastError();
}

Database::~Database() {
    if (m_db.isOpen())
        m_db.close();
}
