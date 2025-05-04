#include "medicament_recommande.h"

Medicament_recommande::Medicament_recommande()
{

}

Medicament_recommande::~Medicament_recommande()
{

}

Medicament_recommande::Medicament_recommande(QString _ordonnance, QString _medicament, QString _posologie)
{
    idOrdonnance = _ordonnance;
    idMedicament = _medicament;
    idPosologie = _posologie;
}

bool Medicament_recommande::Ajouter(DatabaseManager *db)
{
    db->openDatabase();
    QString query = QString("SELECT MAX(id_medicament_recommande) AS id FROM T_MEDICAMENT_RECOMMANDE");
    QSqlQuery Qquery (db->getDatabase());
    Qquery.exec(query);
    QString id;
    while(Qquery.next()){
        int id__int = Qquery.value("id").toInt() + 1;
        id = QString::fromStdString(std::to_string(id__int));
    }

    query = QString("INSERT INTO T_MEDICAMENT_RECOMMANDE(ID_MEDICAMENT_RECOMMANDE, ID_CONSULTATION, ID_MEDICAMENT, ID_POSOLOGIE) VALUES ('"+ id +"', "+ idOrdonnance +", '"+ idMedicament +"', '"+ idPosologie +"')");

    if(db->executeQuery(query).isActive()){
        db->closeDatabase();
        return true;
    }

    db->closeDatabase();
    return false;
}
