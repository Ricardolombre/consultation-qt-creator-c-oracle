#include "ajoutermedecin.h"
#include "ui_ajoutermedecin.h"

#include "medecin/medecin.h"

#include <QDebug>
#include <QSqlQuery>
#include <QMessageBox>

AjouterMedecin::AjouterMedecin(DatabaseManager *_db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AjouterMedecin)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::CoverWindow);

    db = _db;

    QString query = QString("SELECT * FROM T_SPECIALITE ORDER BY id_specialite ASC");
    Ajouter(ui->C_specialite, query, "libelle_specialite");
}

AjouterMedecin::~AjouterMedecin()
{
    delete ui;
}

void AjouterMedecin::on_femer_clicked()
{
    this->close();
}

void AjouterMedecin::Ajouter(QComboBox* CO_par,QString query,QString index)
{
    if(db->openDatabase())
    {
        QSqlQuery queryString = db->executeQuery(query);
        if(queryString.isActive())
        {
            CO_par->clear();
            while(queryString.next())
            {
                QString valeur = queryString.value(index).toString();
                CO_par->addItem(valeur);
            }
        }
        db->closeDatabase();
    }
}

void AjouterMedecin::on_btnAjoutMedecin_clicked()
{
    QString nom = ui->L_nomM->text();
    QString prenom = ui->L_prenomM->text();
    QString adresse = ui->L_adresseM->text();
    QString tel = ui->L_telM->text();
    QString email = ui->L_emailM->text();
    int id_specialite = ui->C_specialite->currentIndex();

    Medecin medecin(nom, prenom, adresse, email, tel, id_specialite);
    if(medecin.Ajouter(db))
    {
        emit clickAddMedecin();
    }
    else
    {
        QMessageBox::warning(this, "Ajout", "Ce médecin existe déjà dans notre base de données");
        return;
    }

    this->close();
}
