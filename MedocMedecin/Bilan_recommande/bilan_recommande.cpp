#include "bilan_recommande.h"
#include "connexion/databasemanager.h"

Bilan_recommande::Bilan_recommande(QString _ordonnance, QString _bilan)
{
    idOrdonnance = _ordonnance;
    idBilan = _bilan;
}

Bilan_recommande::Bilan_recommande()
{

}

Bilan_recommande::~Bilan_recommande()
{

}

bool Bilan_recommande::Ajouter(DatabaseManager *db)
{
    db->openDatabase();
    QString query = QString("SELECT MAX(id_bilan_recommande) AS id FROM T_BILAN_RECOMMANDE");
    QSqlQuery Qquery (db->getDatabase());
    Qquery.exec(query);
    QString id;
    while(Qquery.next()){
        int id__int = Qquery.value("id").toInt() + 1;
        id = QString::fromStdString(std::to_string(id__int));
    }

    query = QString("INSERT INTO T_BILAN_RECOMMANDE(ID_BILAN_RECOMMANDE, ID_CONSULTATION, ID_BILAN) VALUES ('"+ id +"', "+ idOrdonnance +", '"+ idBilan +"')");

    if(db->executeQuery(query).isActive()){
        db->closeDatabase();
        return true;
    }

    db->closeDatabase();
    return false;
}
