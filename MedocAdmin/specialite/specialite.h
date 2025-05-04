#ifndef SPECIALITE_H
#define SPECIALITE_H
#include <QString>
#include <connexion/databasemanager.h>

class Specialite
{
public:
    Specialite();
    Specialite(QString _specialite);
    ~Specialite();

    bool Ajouter(DatabaseManager *);
    bool supprimer(DatabaseManager, int);

private:
    QString Libelle_specialite;
};

#endif // SPECIALITE_H
