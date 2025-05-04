#ifndef MEDICAMENT_RECOMMANDE_H
#define MEDICAMENT_RECOMMANDE_H
#include <QString>
#include "connexion/databasemanager.h"

class Medicament_recommande
{
public:
    Medicament_recommande();
    Medicament_recommande(QString _ordonnance, QString _medicament, QString _posologie);
    ~Medicament_recommande();

    bool Ajouter(DatabaseManager *db);

private:
    QString idOrdonnance;
    QString idMedicament;
    QString idPosologie;
};

#endif // MEDICAMENT_RECOMMANDE_H
