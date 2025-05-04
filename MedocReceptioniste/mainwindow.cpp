#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDockWidget>
#include <QListWidget>
#include <QStringListModel>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QSqlRecord>
#include "ajouterpatient.h"
#include "ajouterrdv.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dbManager = new DatabaseManager();
    completer = new QCompleter(this);


    ui->btnGererPayement->setVisible(false);
    ui->btnAdmin->setVisible(false);

    QStringList names = getNamesFromQuery("SELECT NOM||' '||PRENOM as NOMS FROM T_PATIENT");

    QStringListModel *model = new QStringListModel(names, this);

    completer->setModel(model);
    completer->setCompletionMode(QCompleter::PopupCompletion);
    completer->setCaseSensitivity(Qt::CaseInsensitive);

    ui->L_rechercherMedecin->setCompleter(completer);

    ActualiserPatient();
    ActualiserRDV();
}

MainWindow::~MainWindow()
{
    delete ui;
}

QStringList MainWindow::getNamesFromQuery(const QString &query)
{
    QStringList names;
    if(dbManager->openDatabase()){
        QSqlQuery Qquery = dbManager->executeQuery(query);
        if (!Qquery.isActive()) {
            return names;
        }

        while (Qquery.next()) {
            QString name = Qquery.value("NOMS").toString();
            names.append(name);
        }
    }

    return names;
}

void MainWindow::setupTableWidget(const QString &queryString, QTableWidget* tableWidget)
{
    if (dbManager->openDatabase())
    {
        QSqlQuery query = dbManager->executeQuery(queryString);

        if (query.isActive())
        {
            // Vider le tableau avant de le remplir
            tableWidget->clear();
            tableWidget->setRowCount(0);

            // Obtenir le nombre de colonnes et définir les en-têtes
            int columnCount = query.record().count();
            tableWidget->setColumnCount(columnCount);
            for (int i = 0; i < columnCount; ++i) {
                tableWidget->setHorizontalHeaderItem(i, new QTableWidgetItem(query.record().fieldName(i)));
            }

            // Remplir le tableau avec les données
            int row = 0;
            while (query.next()) {
                tableWidget->insertRow(row);
                for (int col = 0; col < columnCount; ++col) {
                    QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
                    item->setFlags(item->flags() & ~Qt::ItemIsEditable); // Rendre l'élément non modifiable
                    tableWidget->setItem(row, col, item);
                }

                ++row;
            }

            // Redimensionner les colonnes pour le contenu
            tableWidget->resizeColumnsToContents();

            // Ajuster la largeur des colonnes pour qu'elles remplissent le tableau
            tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        }
        else
        {
            qDebug() << "La requête n'a pas pu être exécutée";
        }
        dbManager->closeDatabase();
    }
    else
    {
        QMessageBox::critical(this, tr("Erreur"), tr("La base de données n'a pas pu être ouverte"));
    }
}

void MainWindow::ActualiserPatient()
{
    QString query = QString("SELECT NOM, PRENOM, ADRESSE, EMAIL, TEL, FLOOR(MONTHS_BETWEEN(SYSDATE, DATENAISSANCE) / 12) AGE FROM T_PATIENT ORDER BY ID_PATIENT ASC");
    setupTableWidget(query, ui->tablePatient);
}

void MainWindow::ActualiserRDV()
{
    QString query = QString("SELECT DESCRIPTIONRDV DESCRIPTION, TO_CHAR(DATERDV, 'YYYY-MM-DD') DATERDV, TO_CHAR(DATERDV, 'HH24:MI') HEURE, M.NOM||' '||M.PRENOM NOM_DU_MEDECIN,  P.NOM||' '||P.PRENOM NOM_DU_PATIENT FROM T_RDV, T_MEDECIN M, T_PATIENT P WHERE M.ID_MEDECIN = T_RDV.ID_MEDECIN AND P.ID_PATIENT = T_RDV.ID_PATIENT ORDER BY ID_RDV ASC");
    setupTableWidget(query, ui->tableRDV);
}

void MainWindow::on_ajouterPatient_clicked()
{
    AjouterPatient *ajout = new AjouterPatient(dbManager, this);
    ajout->show();

    connect(ajout, &AjouterPatient::clickAddPatient, this, MainWindow::ActualiserPatient);
}

void MainWindow::on_ajouterRDV_clicked()
{
    AjouterRDV *ajout = new AjouterRDV(dbManager, this);
    ajout->show();

    connect(ajout, &AjouterRDV::clickAddRDV, this, MainWindow::ActualiserRDV);
}

void MainWindow::on_btnGererPayement_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_btnGererRDV_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->btnGererRDV->setStyleSheet(QString("color: rgb(0, 0, 0);"
                                               "background-color: rgb(227, 237, 243);"
                                               "border: none;"));

    ui->btnGererPatient->setStyleSheet(QString("color: rgb(255, 255, 255);"
                                               "border: none;"));
}

void MainWindow::on_btnAdmin_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_btnGererPatient_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->btnGererPatient->setStyleSheet(QString("color: rgb(0, 0, 0);"
                                               "background-color: rgb(227, 237, 243);"
                                               "border: none;"));

    ui->btnGererRDV->setStyleSheet(QString("color: rgb(255, 255, 255);"
                                               "border: none;"));
}

void MainWindow::on_L_rechercherPatient_textChanged(const QString &arg1)
{
    if(ui->ra_nomPatient->isChecked())
    {
        QString query = QString("SELECT NOM, PRENOM, ADRESSE, EMAIL, TEL, FLOOR(MONTHS_BETWEEN(SYSDATE, DATENAISSANCE) / 12) AGE  FROM T_PATIENT WHERE UPPER(NOM) LIKE '"+ arg1.toUpper() +"%' OR UPPER(PRENOM) LIKE '"+ arg1.toUpper() +"%' ORDER BY ID_PATIENT ASC");
        setupTableWidget(query, ui->tablePatient);
    }
    else
    {
        QString query = QString("SELECT NOM, PRENOM, ADRESSE, EMAIL, TEL, FLOOR(MONTHS_BETWEEN(SYSDATE, DATENAISSANCE) / 12) AGE  FROM T_PATIENT WHERE FLOOR(MONTHS_BETWEEN(SYSDATE, DATENAISSANCE) / 12) = "+ arg1 +" ORDER BY ID_PATIENT ASC");
        setupTableWidget(query, ui->tablePatient);
    }
}

void MainWindow::on_L_rechercherRdv_textChanged(const QString &arg1)
{
    if(ui->ra_patient->isChecked())
    {
        QString query = QString("SELECT DESCRIPTIONRDV DESCRIPTION, TO_CHAR(DATERDV, 'YYYY-MM-DD') DATERDV, TO_CHAR(DATERDV, 'HH24:MI') HEURE, M.NOM||' '||M.PRENOM NOM_DU_MEDECIN,  P.NOM||' '||P.PRENOM NOM_DU_PATIENT FROM T_RDV, T_MEDECIN M, T_PATIENT P WHERE M.ID_MEDECIN = T_RDV.ID_MEDECIN AND P.ID_PATIENT = T_RDV.ID_PATIENT AND (UPPER(P.NOM) LIKE '"+ arg1.toUpper() +"%' OR UPPER(P.PRENOM) LIKE '"+ arg1.toUpper() +"%') ORDER BY ID_RDV ASC");
        setupTableWidget(query, ui->tableRDV);
    }
    else
    {
        QString query = QString("SELECT DESCRIPTIONRDV DESCRIPTION, TO_CHAR(DATERDV, 'YYYY-MM-DD') DATERDV, TO_CHAR(DATERDV, 'HH24:MI') HEURE, M.NOM||' '||M.PRENOM NOM_DU_MEDECIN,  P.NOM||' '||P.PRENOM NOM_DU_PATIENT FROM T_RDV, T_MEDECIN M, T_PATIENT P WHERE M.ID_MEDECIN = T_RDV.ID_MEDECIN AND P.ID_PATIENT = T_RDV.ID_PATIENT AND (UPPER(M.NOM) LIKE '"+ arg1.toUpper() +"%' OR UPPER(M.PRENOM) LIKE '"+ arg1.toUpper() +"%') ORDER BY ID_RDV ASC");
        setupTableWidget(query, ui->tableRDV);
    }
}
