#include "medicament.h"
#include <QDebug>
#include <QSqlRecord>

Medicament::Medicament()
{

}

Medicament::Medicament(QString _medicament)
{
    Libelle_medicament = _medicament;
}

Medicament::~Medicament()
{

}

bool Medicament::Ajouter(DatabaseManager *db)
{
    db->openDatabase();
    QString query = QString("SELECT * FROM T_MEDICAMENT WHERE libelle_medicament = '"+ Libelle_medicament +"'");
    QSqlQuery Qquery (db->getDatabase());
    int count = 0;

    while (Qquery.next()) {
        count++;
    }

    qDebug() << count;
    if(count > 0){
        return false;
    }

    query = QString("SELECT MAX(id_medicament) AS id FROM T_MEDICAMENT");
    Qquery.exec(query);
    QString id;
    while(Qquery.next()){
        int id__int = Qquery.value("id").toInt() + 1;
        id = QString::fromStdString(std::to_string(id__int));
    }

    query = QString("INSERT INTO T_MEDICAMENT(id_medicament, libelle_medicament) VALUES ('"+ id +"', '"+ Libelle_medicament +"')");


    if(db->executeQuery(query).isActive()){
        db->closeDatabase();
        return true;
    }

    db->closeDatabase();
    return false;
}
