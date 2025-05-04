#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>

class DatabaseManager {
public:
    DatabaseManager();
    ~DatabaseManager();

    bool openDatabase();
    void closeDatabase();
    QSqlDatabase getDatabase();

    QSqlQuery executeQuery(const QString& queryString);

private:
    QSqlDatabase db;
};


#endif // DATABASEMANAGER_H
