#include "consultation.h"
#include <QDebug>

Consultation::Consultation()
{

}

Consultation::~Consultation()
{

}

Consultation::Consultation(QString _motif, QString _resulta, QString _diagnostique, QString _poids, QString _taille, QString _temperature, QString _frequence, QString _pression, QString _observation, QString _idmedecin, QString _iddossier)
{
    motif = _motif;
    resultat = _resulta;
    diagnostique = _diagnostique;
    poids = _poids;
    taille = _taille;
    temperature = _temperature;
    frequence = _frequence;
    pression = _pression;
    observation = _observation;
    idMedecin = _idmedecin;
    idDossier = _iddossier;
}

bool Consultation::Ajouter(DatabaseManager *db)
{
    db->openDatabase();
    QString query = QString("SELECT MAX(ID_CONSULTATION) AS id FROM T_CONSULTATION");
    QSqlQuery Qquery (db->getDatabase());
    Qquery.exec(query);
    QString id;
    while(Qquery.next()){
        int id__int = Qquery.value("id").toInt() + 1;
        id = QString::fromStdString(std::to_string(id__int));
    }

    qDebug() << idDossier;

    query = QString("INSERT INTO T_CONSULTATION(ID_CONSULTATION, MOTIF, DATE_CONSULTATION, DIAGNOSTIQUE_MEDICAL, POIDS, TAILLE, TEMPERATURE, FREQUENCE_CARDIAQUE, PRESSION_ARTERIELLE, RESULTAT_EXAMEN, OBSERVATION, ID_MEDECIN, ID_DOSSIER_MEDICAL) VALUES ('"+ id +"', '"+ motif +"', SYSDATE, '"+ diagnostique +"', '"+ poids +"', '"+ taille +"', '"+ temperature +"', '"+ frequence +"', '"+ pression +"', '"+ resultat +"', '"+ observation +"', '"+ idMedecin +"', '"+ idDossier +"')");

    Qquery.exec(query);
    if(Qquery.isActive()){
        db->closeDatabase();
        return true;
    }

    db->closeDatabase();
    return false;
}
