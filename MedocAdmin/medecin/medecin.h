#ifndef MEDECIN_H
#define MEDECIN_H

#include <QString>
#include <connexion/databasemanager.h>

class Medecin
{
public:
    Medecin();
    Medecin(QString __nom, QString __prenom, QString __adresse, QString __mail, QString __tel, int __id_specialite);

    bool Ajouter(DatabaseManager *);
    bool supprimer(DatabaseManager, int);

private:
    QString nom;
    QString prenom;
    QString adresse;
    QString mail;
    QString tel;
    int id_specialite;
};

#endif // MEDECIN_H
