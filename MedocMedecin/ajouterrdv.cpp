#include "ajouterrdv.h"
#include "ui_ajouterrdv.h"
#include <QStringListModel>
#include <RDV/rdv.h>
#include <QDebug>
#include <QMessageBox>

AjouterRDV::AjouterRDV(DatabaseManager *_db, QString _idMedecin, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AjouterRDV)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::CoverWindow);
    dbManager = _db;
    completer = new QCompleter(this);
    idMedecin = _idMedecin;

    QStringList noms = getNamesFromQuery("SELECT NOM||' '||PRENOM as NOMS FROM T_PATIENT", "NOMS");

    QStringListModel *model = new QStringListModel(noms, this);

    completer->setModel(model);
    completer->setCompletionMode(QCompleter::PopupCompletion);
    completer->setCaseSensitivity(Qt::CaseInsensitive);

    ui->L_nomP->setCompleter(completer);
}

AjouterRDV::~AjouterRDV()
{
    delete ui;
}

void AjouterRDV::on_femer_clicked()
{
    this->close();
}

void AjouterRDV::on_btnAjoutRDV_clicked()
{
    QString nomP = ui->L_nomP->text();
    QString description = ui->T_descriptionRDV->document()->toPlainText();
    QDate dateRDV = ui->D_RDV->date();
    QTime heureRDV = ui->H_RDV->time();
    int id_patient = -1;

    if(dbManager->openDatabase())
    {
        QString query = QString("SELECT ID_PATIENT FROM T_PATIENT WHERE NOM||' '||PRENOM = '"+ nomP +"'");
        QSqlQuery Qquery = dbManager->executeQuery(query);
        if(Qquery.isActive()){
            while(Qquery.next()){
                id_patient = Qquery.value("ID_PATIENT").toInt();
            }
        }

        if(id_patient == -1){
            QMessageBox::warning(this, "Rendez-vous", "Les informations du patient ne sont pas valide, veuillez recommencer");
            return;
        }

        QString date = "TO_DATE('"+ dateRDV.toString("yyyy-MM-dd") + " " + heureRDV.toString("hh:mm:ss") +"', 'YYYY-MM-DD HH24:MI:SS')";

        query = QString("SELECT ID_PATIENT FROM T_RDV WHERE ID_PATIENT = "+ QString::number(id_patient) +" AND DATERDV = "+ date);
        Qquery = dbManager->executeQuery(query);
        if(Qquery.next())
        {
            QMessageBox::warning(this, "Rendez-vous", "Le patient a déjà un rendez-vous prévu pour cette date");
            return;
        }

        query = QString("SELECT ID_MEDECIN FROM T_RDV WHERE ID_MEDECIN = "+ idMedecin +" AND DATERDV = "+ date);
        Qquery = dbManager->executeQuery(query);
        if(Qquery.next())
        {
            QMessageBox::warning(this, "Rendez-vous", "Vous avez déjà un rendez-vous prévu pour cette date");
            return;
        }

        RDV rdv(dateRDV, heureRDV, description, idMedecin.toInt(), id_patient);
        if(rdv.Ajouter(dbManager))
        {
            QMessageBox::information(this, "Rendez-vous", "Le rendez-vous a été pris avec succès");
            emit clickAddRDV();
        }



        dbManager->closeDatabase();

        this->close();
    }

}

QStringList AjouterRDV::getNamesFromQuery(const QString &query, const QString &value)
{
    QStringList names;
    if(dbManager->openDatabase()){
        QSqlQuery Qquery = dbManager->executeQuery(query);
        if (!Qquery.isActive()) {
            return names;
        }

        while (Qquery.next()) {
            QString name = Qquery.value(value).toString();
            names.append(name);
        }
    }

    return names;
}
