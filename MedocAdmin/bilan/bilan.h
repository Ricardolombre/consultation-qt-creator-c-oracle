#ifndef BILAN_H
#define BILAN_H
#include "connexion/databasemanager.h"
#include <QString>

class Bilan
{
public:
    Bilan();
    Bilan(QString _libelle_bilan);
    ~Bilan();

    bool Ajouter(DatabaseManager *);
    bool supprimer(DatabaseManager, int);

private:
    QString Libelle_bilan;
};

#endif // BILAN_H
