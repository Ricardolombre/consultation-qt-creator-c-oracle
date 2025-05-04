#include "ajouterbilan.h"
#include "ui_ajouterbilan.h"

ajouterBilan::ajouterBilan(DatabaseManager *_db, QString _ordonnance, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ajouterBilan)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::CoverWindow);
    db = _db;

    ordonnance = _ordonnance;
    rowCount = 0;

    QString query = QString("SELECT * FROM T_BILAN ORDER BY ID_BILAN ASC");
    Ajouter(ui->CO_bilan, query, "libelle_bilan");
}

ajouterBilan::~ajouterBilan()
{
    delete ui;
}

void ajouterBilan::on_btnAjouterBilan_clicked()
{
    add();
}

void ajouterBilan::Ajouter(QComboBox* CO_par,QString query,QString index)
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

void ajouterBilan::add()
{
    QString idBilan = QString::fromStdString(std::to_string(ui->CO_bilan->currentIndex() + 1));
    listBilan.push_back(idBilan);

    ui->tableWidget->insertRow(rowCount);

    // Ajout des éléments dans la nouvelle ligne
    ui->tableWidget->setItem(rowCount, 0, new QTableWidgetItem(ui->CO_bilan->currentText()));

    rowCount++;
}

std::vector<Bilan_recommande> ajouterBilan::getBilan()
{
    return list;
}

void ajouterBilan::on_btnSauvegarger_clicked()
{
    int size = listBilan.size();
    for(int i = 0; i < size; i++)
    {
        Bilan_recommande bilan(ordonnance, listBilan[i]);
        list.push_back(bilan);
    }

    this->close();
}

void ajouterBilan::on_femer_clicked()
{
    this->close();
}
