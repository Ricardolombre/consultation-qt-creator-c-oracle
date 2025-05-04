#include "rdv.h"
#include <QDebug>

RDV::RDV()
{

}

RDV::RDV(QDate __dateRDV, QTime __heureRDV, QString __descriptionRDV, int __id_medecin, int __id_patient)
{
    dateRDV = __dateRDV;
    heureRDV = __heureRDV;
    descriptionRDV = __descriptionRDV;
    id_medecin = __id_medecin;
    id_patient = __id_patient;
}

RDV::~RDV()
{

}

bool RDV::Ajouter(DatabaseManager *db)
{
    db->openDatabase();
    QString query = QString("SELECT MAX(id_RDV) AS id FROM T_RDV");
    QSqlQuery Qquery (db->getDatabase());
    Qquery.exec(query);
    QString id;
    while(Qquery.next()){
        int id__int = Qquery.value("id").toInt() + 1;
        id = QString::fromStdString(std::to_string(id__int));
    }

    QString date = "TO_DATE('"+ dateRDV.toString("yyyy-MM-dd") + " " + heureRDV.toString("hh:mm:ss") +"', 'YYYY-MM-DD HH24:MI:SS')";
    query = QString("INSERT INTO T_RDV VALUES ('"+ id +"', "+ date +", '"+ descriptionRDV +"', '"+ QString::fromStdString(std::to_string(id_medecin)) +"', '"+ QString::fromStdString(std::to_string(id_patient)) +"')");

    if(db->executeQuery(query).isValid()){
        db->closeDatabase();
        return true;
    }

    db->closeDatabase();
    return false;
}
