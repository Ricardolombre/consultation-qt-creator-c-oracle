#include "medecin.h"

Medecin::Medecin()
{

}

Medecin::Medecin(QString __nom, QString __prenom, QString __adresse, QString __mail, QString __tel, int __id_specialite)
{
    nom = __nom;
    prenom = __prenom;
    adresse = __adresse;
    mail = __mail;
    tel = __tel;
    id_specialite = __id_specialite;
}

bool Medecin::Ajouter(DatabaseManager *db)
{
    db->openDatabase();
    QString query = QString("SELECT MAX(id_medecin) AS id FROM T_MEDECIN");
    QSqlQuery Qquery (db->getDatabase());
    Qquery.exec(query);
    QString id;
    while(Qquery.next()){
        int id__int = Qquery.value("id").toInt() + 1;
        id = QString::fromStdString(std::to_string(id__int));
    }

    query = QString("SELECT NOM, PRENOM FROM T_MEDECIN WHERE NOM = '"+ nom +"' AND PRENOM = '"+ prenom +"'");
    Qquery.exec(query);
    int count = 0;

    while (Qquery.next()) {
        count++;
    }

    if(count > 0){
        return false;
    }

    query = QString("INSERT INTO T_MEDECIN(id_medecin, nom, prenom, adresse, tel, email, id_specialite) VALUES ("+ id +",'"+ nom +"', '"+ prenom +"', '"+ adresse +"', '"+ tel +"', '"+ mail +"', '"+ QString::fromStdString(std::to_string(id_specialite)) +"')");

    if(db->executeQuery(query).isActive()){
        query = QString("INSERT INTO T_LOGINN(USERNAME, PASSWORD, ID_MEDECIN) VALUES ('"+ prenom +"@"+ id +"', '"+ prenom +"@"+ id +"', '"+ id +"')");
        Qquery.exec(query);

        db->closeDatabase();
        return true;
    }

    db->closeDatabase();
    return false;
}
