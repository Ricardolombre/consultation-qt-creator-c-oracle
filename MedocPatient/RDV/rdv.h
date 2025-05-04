#ifndef RDV_H
#define RDV_H
#include <QDate>
#include <QTime>
#include "connexion/databasemanager.h"

class RDV
{
public:
    RDV();
    RDV(QDate __dateRDV, QTime __heureRDV, QString __descriptionRDV, int __id_medecin, int __id_patient);

    ~RDV();

    bool Ajouter(DatabaseManager *);
    bool supprimer(DatabaseManager *, int);

private:
    QDate dateRDV;
    QTime heureRDV;
    QString descriptionRDV;
    int id_medecin;
    int id_patient;
};

#endif // RDV_H
