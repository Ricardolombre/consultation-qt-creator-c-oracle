#ifndef CONSULTATION_H
#define CONSULTATION_H
#include <QString>
#include "connexion/databasemanager.h"

class Consultation
{
public:
    Consultation();
    Consultation(QString _motif, QString _resulta, QString _diagnostique, QString _poids, QString _taille, QString _temperature, QString _frequence, QString _pression, QString _observation, QString _idmedecin, QString _iddossier);
    ~Consultation();

    bool Ajouter(DatabaseManager *db);

private:
    QString motif;
    QString resultat;
    QString diagnostique;
    QString poids;
    QString taille;
    QString temperature;
    QString frequence;
    QString pression;
    QString observation;
    QString idMedecin;
    QString idDossier;
};

#endif // CONSULTATION_H
