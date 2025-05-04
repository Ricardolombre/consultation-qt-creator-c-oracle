#include "specialite.h"
#include <QDebug>
#include <QSqlRecord>

Specialite::Specialite()
{

}

Specialite::~Specialite()
{

}

Specialite::Specialite(QString _specialite)
{
    Libelle_specialite = _specialite;
}

bool Specialite::Ajouter(DatabaseManager *db)
{
    db->openDatabase();
    QString query = QString("SELECT * FROM T_SPECIALITE WHERE libelle_specialite = '"+ Libelle_specialite +"'");
    QSqlQuery Qquery (db->getDatabase());
    int count = 0;

    while (Qquery.next()) {
        count++;
    }

    qDebug() << count;
    if(count > 0){
        return false;
    }

    query = QString("SELECT MAX(id_specialite) AS id FROM T_SPECIALITE");
    Qquery.exec(query);
    QString id;
    while(Qquery.next()){
        int id__int = Qquery.value("id").toInt() + 1;
        id = QString::fromStdString(std::to_string(id__int));
    }

    query = QString("INSERT INTO T_SPECIALITE(id_specialite, libelle_specialite) VALUES ('"+ id +"', '"+ Libelle_specialite +"')");


    if(db->executeQuery(query).isActive()){
        db->closeDatabase();
        return true;
    }

    db->closeDatabase();
    return false;
}
