#include "ajouterordonnance.h"
#include "ui_ajouterordonnance.h"
#include <QComboBox>

#include "Medicament_recommande/medicament_recommande.h"

AjouterOrdonnance::AjouterOrdonnance(DatabaseManager *_db, QString _ordonnance, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AjouterOrdonnance)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::CoverWindow);
    db = _db;

    ordonnance = _ordonnance;
    rowCount = 0;

    QString query = QString("SELECT * FROM T_MEDICAMENT ORDER BY ID_MEDICAMENT ASC");
    Ajouter(ui->CO_medicament, query, "libelle_medicament");

    query = QString("SELECT * FROM T_POSOLOGIE ORDER BY ID_POSOLOGIE ASC");
    Ajouter(ui->CO_posologie, query, "POSOLOGIE");
}

AjouterOrdonnance::~AjouterOrdonnance()
{
    delete ui;
}

void AjouterOrdonnance::on_btnAjouterMedicament_clicked()
{
    add();
}

void AjouterOrdonnance::add()
{
    QString idMedicament = QString::fromStdString(std::to_string(ui->CO_medicament->currentIndex() + 1));
    listmedicament.push_back(idMedicament);

    QString idPosologie = QString::fromStdString(std::to_string(ui->CO_posologie->currentIndex() + 1));
    listposologie.push_back(idPosologie);

    ui->tableWidget->insertRow(rowCount);

    // Ajout des éléments dans la nouvelle ligne
    ui->tableWidget->setItem(rowCount, 0, new QTableWidgetItem(ui->CO_medicament->currentText()));
    ui->tableWidget->setItem(rowCount, 1, new QTableWidgetItem(ui->CO_posologie->currentText()));

    rowCount++;
}

void AjouterOrdonnance::Ajouter(QComboBox* CO_par,QString query,QString index)
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

void AjouterOrdonnance::on_btnSauvegarger_clicked()
{
    int size = listmedicament.size();
    for(int i = 0; i < size; i++)
    {
        Medicament_recommande medoc(ordonnance, listmedicament[i], listposologie[i]);
        list.push_back(medoc);
    }

    this->close();
}

std::vector<Medicament_recommande> AjouterOrdonnance::getMedicament()
{
    return list;
}

void AjouterOrdonnance::on_femer_clicked()
{
    this->close();
}
