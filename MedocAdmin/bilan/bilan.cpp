#include "bilan.h"

#include <QDebug>
#include <QSqlRecord>

Bilan::Bilan()
{

}

Bilan::~Bilan()
{

}

Bilan::Bilan(QString _libelle_bilan)
{
    Libelle_bilan = _libelle_bilan;
}

bool Bilan::Ajouter(DatabaseManager *db)
{
    db->openDatabase();
    QString query = QString("SELECT * FROM T_BILAN WHERE libelle_bilan = "+ Libelle_bilan);
    QSqlQuery Qquery (db->getDatabase());
    int count = 0;

    while (Qquery.next()) {
        count++;
    }

    qDebug() << count;
    if(count > 0){
        return false;
    }

    query = QString("SELECT MAX(id_bilan) AS id FROM T_BILAN");
    Qquery.exec(query);
    QString id;
    while(Qquery.next()){
        int id__int = Qquery.value("id").toInt() + 1;
        id = QString::fromStdString(std::to_string(id__int));
    }

    query = QString("INSERT INTO T_BILAN(id_bilan, libelle_bilan) VALUES ('"+ id +"', '"+ Libelle_bilan +"')");


    if(db->executeQuery(query).isActive()){
        db->closeDatabase();
        return true;
    }

    db->closeDatabase();
    return false;
}
