#ifndef PATIENT_H
#define PATIENT_H
#include <QString>
#include <QDate>
#include <connexion/databasemanager.h>

class Patient
{
public:
    Patient();
    ~Patient();
    Patient(QString __nom, QString __prenom, QString __adresse, QString __mail, QString __tel, QDate __date);

    bool Ajouter(DatabaseManager *);
    bool supprimer(DatabaseManager, int);

private:
    QString nom;
    QString prenom;
    QString adresse;
    QString mail;
    QString tel;
    QDate date;
};

#endif // PATIENT_H
