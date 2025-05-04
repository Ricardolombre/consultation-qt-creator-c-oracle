#ifndef BILAN_RECOMMANDE_H
#define BILAN_RECOMMANDE_H
#include <QString>
#include "connexion/databasemanager.h"

class Bilan_recommande
{
public:
    Bilan_recommande();
    ~Bilan_recommande();
    Bilan_recommande(QString _ordonnance, QString _bilan);

    bool Ajouter(DatabaseManager *db);

private:
    QString idOrdonnance;
    QString idBilan;
};

#endif // BILAN_RECOMMANDE_H
