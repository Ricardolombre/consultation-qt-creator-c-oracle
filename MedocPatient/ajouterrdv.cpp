#include "ajouterrdv.h"
#include "ui_ajouterrdv.h"
#include <QStringListModel>
#include <RDV/rdv.h>
#include <QDebug>
#include <QMessageBox>

AjouterRDV::AjouterRDV(DatabaseManager *_db, QString _idPatient, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AjouterRDV)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::CoverWindow);
    dbManager = _db;
    idPatient = _idPatient;

    QString query = QString("SELECT * FROM T_SPECIALITE ORDER BY LIBELLE_SPECIALITE ASC");
    Ajouter(ui->co_specialite, query, "LIBELLE_SPECIALITE");
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
    QString IdSpecialite = QString::number(ui->co_specialite->currentIndex() + 1);
    QString description = ui->T_descriptionRDV->document()->toPlainText();
    QDate dateRDV = ui->D_RDV->date();
    QTime heureRDV = ui->H_RDV->time();
    QString id_medecin = "0";
    int doc_id = 0;

    QString date = "TO_DATE('"+ dateRDV.toString("yyyy-MM-dd") + " " + heureRDV.toString("hh:mm:ss") +"', 'YYYY-MM-DD HH24:MI:SS')";

    if(dbManager->openDatabase())
    {
        QString query = QString("SELECT ID_PATIENT FROM T_RDV WHERE ID_PATIENT = "+ idPatient +" AND DATERDV = "+ date);
        QSqlQuery Qquery = dbManager->executeQuery(query);
        if(Qquery.next())
        {
            QMessageBox::warning(this, "Rendez-vous", "Vous avez déjà un rendez-vous prévu pour cette date");
            return;
        }

        query = QString("SELECT ID_MEDECIN FROM T_MEDECIN WHERE ID_SPECIALITE = "+ IdSpecialite);
        Qquery = dbManager->executeQuery(query);
        if(Qquery.isActive()){
            while(Qquery.next()){
                id_medecin = Qquery.value("ID_MEDECIN").toString();
                query = QString("SELECT ID_MEDECIN FROM T_RDV WHERE ID_MEDECIN = "+ id_medecin +" AND DATERDV = "+ date);
                Qquery = dbManager->executeQuery(query);
                if(!Qquery.next())
                {
                    doc_id = id_medecin.toInt();
                }
            }
        }

        if(doc_id == 0)
        {
            QMessageBox::warning(this, "Rendez-vous", "Aucun médecin de cette spécialité n'est disponible cette date");
            return;
        }

        RDV rdv(dateRDV, heureRDV, description, doc_id, idPatient.toInt());
        if(rdv.Ajouter(dbManager))
        {
            emit clickAddRDV();
            QMessageBox::information(this, "Rendez-vous", "Le rendez-vous a bien été pris");
            this->close();
        }
        else
        {
            QMessageBox::critical(this, "Rendez-vous", "Une erreur s'est produite lors de la prise du rendez-vous");
        }


        dbManager->closeDatabase();
    }
}

void AjouterRDV::Ajouter(QComboBox* CO_par,QString query,QString index)
{
    if(dbManager->openDatabase())
    {
        QSqlQuery queryString = dbManager->executeQuery(query);
        if(queryString.isActive())
        {
            CO_par->clear();
            while(queryString.next())
            {
                QString valeur = queryString.value(index).toString();
                CO_par->addItem(valeur);
            }
        }
        dbManager->closeDatabase();
    }
}
