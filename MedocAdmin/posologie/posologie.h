#ifndef POSOLOGIE_H
#define POSOLOGIE_H
#include <QString>
#include "connexion/databasemanager.h"

class Posologie
{
public:
    Posologie();
    Posologie(QString _posologie);
    ~Posologie();

    bool Ajouter(DatabaseManager *);
    bool supprimer(DatabaseManager, int);

private:
    QString Libelle_posologie;
};

#endif // POSOLOGIE_H
