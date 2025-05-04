#include "posologie.h"
#include <QDebug>
#include <QSqlRecord>

Posologie::Posologie()
{

}

Posologie::~Posologie()
{

}

Posologie::Posologie(QString _posologie)
{
    Libelle_posologie = _posologie;
}

bool Posologie::Ajouter(DatabaseManager *db)
{
    db->openDatabase();
    QString query = QString("SELECT * FROM T_POSOLOGIE WHERE id_posologie = '"+ Libelle_posologie +"'");
    QSqlQuery Qquery (db->getDatabase());
    int count = 0;

    while (Qquery.next()) {
        count++;
    }

    qDebug() << count;
    if(count > 0){
        return false;
    }

    query = QString("SELECT MAX(id_posologie) AS id FROM T_POSOLOGIE");
    Qquery.exec(query);
    QString id;
    while(Qquery.next()){
        int id__int = Qquery.value("id").toInt() + 1;
        id = QString::fromStdString(std::to_string(id__int));
    }

    query = QString("INSERT INTO T_POSOLOGIE(id_posologie, id_posologie) VALUES ('"+ id +"', '"+ Libelle_posologie +"')");


    if(db->executeQuery(query).isActive()){
        db->closeDatabase();
        return true;
    }

    db->closeDatabase();
    return false;
}
