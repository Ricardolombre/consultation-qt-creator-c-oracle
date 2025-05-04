#include "databasemanager.h"
#include <QDebug>
#include <QSqlError>

DatabaseManager::DatabaseManager()
{
    db = QSqlDatabase::addDatabase("QODBC");
    db.setUserName("ricardo");
    db.setDatabaseName("ricardo");
    db.setPassword("ricardo");
}


DatabaseManager::~DatabaseManager()
{

}

bool DatabaseManager::openDatabase()
{
    if (!db.open())
    {
        return false;
    }
    return true;
}

void DatabaseManager::closeDatabase()
{
    db.close();
}

QSqlQuery DatabaseManager::executeQuery(const QString& queryString)
{
    QSqlQuery query(db);
    if (!query.exec(queryString))
    {
        qDebug() << "Erreur d'exécution de la requête:" + query.lastError().text();
    }

    return query;
}

QSqlDatabase DatabaseManager::getDatabase()
{
    return db;
}
