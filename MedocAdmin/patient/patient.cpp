#include "patient/patient.h"
#include <QDebug>

Patient::Patient(QString __nom, QString __prenom, QString __adresse, QString __mail, QString __tel, QDate __date)
{
    nom = __nom;
    prenom = __prenom;
    adresse = __adresse;
    mail = __mail;
    tel = __tel;
    date = __date;
}

Patient::Patient()
{

}

bool Patient::Ajouter(DatabaseManager *db)
{
    db->openDatabase();
    QString query = QString("SELECT MAX(id_patient) AS id FROM T_PATIENT");
    QSqlQuery Qquery (db->getDatabase());
    Qquery.exec(query);
    QString id;
    while(Qquery.next()){
        int id__int = Qquery.value("id").toInt() + 1;
        id = QString::fromStdString(std::to_string(id__int));
    }

    QString dateN = "TO_DATE('" + date.toString("yyyy-MM-dd") + "', 'YYYY-MM-DD')";
    query = QString("SELECT NOM, PRENOM FROM T_PATIENT WHERE NOM = '"+ nom +"' AND PRENOM = '"+ prenom +"'");
    Qquery.exec(query);
    int count = 0;

    while (Qquery.next()) {
        count++;
    }

    qDebug() << count;
    if(count > 0){
        return false;
    }

    qDebug() << dateN;
    query = QString("INSERT INTO T_PATIENT(id_patient, nom, prenom, adresse, tel, email, datenaissance) VALUES ('"+ id +"', '"+ nom +"', '"+ prenom +"', '"+ adresse +"', '"+ tel +"', '"+ mail +"',"+ dateN +")");

    if(db->executeQuery(query).isActive()){
        query = QString("INSERT INTO T_LOGIN(USERNAME, PASSWORD, ID_PATIENT) VALUES ('"+ prenom +"@"+ id +"', '"+ prenom +"@"+ id +"', '"+ id +"')");
        Qquery.exec(query);

        query = QString("SELECT MAX(id_dossier_medical) AS id FROM T_DOSSIER_MEDICAL");
        Qquery.exec(query);
        query = QString("INSERT INTO T_DOSSIER_MEDICAL(ID_DOSSIER_MEDICAL, ID_PATIENT) VALUES ('"+ id +"', '"+ id +"')");
        Qquery.exec(query);


        db->closeDatabase();
        return true;
    }

    db->closeDatabase();
    return false;
}

Patient::~Patient()
{

}
