#ifndef MEDICAMENT_H
#define MEDICAMENT_H
#include <QString>
#include "connexion/databasemanager.h"

class Medicament
{
public:
    Medicament();
    Medicament(QString _medicament);
    ~Medicament();

    bool Ajouter(DatabaseManager *);
    bool supprimer(DatabaseManager, int);

private:
    QString Libelle_medicament;
};

#endif // MEDICAMENT_H
