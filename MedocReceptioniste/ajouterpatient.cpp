#include "ajouterpatient.h"
#include "ui_ajouterpatient.h"
#include "patient/patient.h"

AjouterPatient::AjouterPatient(DatabaseManager *_db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AjouterPatient)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::CoverWindow);

    db = _db;
}

AjouterPatient::~AjouterPatient()
{
    delete ui;
}

void AjouterPatient::on_femer_clicked()
{
    this->close();
}

void AjouterPatient::on_btnAjoutPatient_clicked()
{
    QString nom = ui->L_nomP->text();
    QString prenom = ui->L_prenomP->text();
    QString adresse = ui->L_adresseP->text();
    QString tel = ui->L_telP->text();
    QString email = ui->L_emailP->text();
    QDate date = ui->D_naissanceP->date();

    Patient patient(nom, prenom, adresse, email, tel, date);
    patient.Ajouter(db);

    emit clickAddPatient();

    this->close();
}
