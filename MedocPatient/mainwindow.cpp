#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QMessageBox>
#include <QSqlRecord>
#include <QDate>

#include "ajouterrdv.h"

MainWindow::MainWindow(QWidget *parent, QString patient, QString dossier, DatabaseManager * _dbManager) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dbManager = _dbManager;
    indexDossier = dossier;
    indexPatient = patient;

    actualiserRDV();
    chargement();
}

MainWindow::~MainWindow()
{
    delete ui;
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
            tableWidget->setColumnCount(columnCount); // Ajouter une colonne pour les actions
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

void MainWindow::actualiserRDV()
{
    QString query = QString("SELECT DESCRIPTIONRDV DESCRIPTION, TO_CHAR(DATERDV, 'YYYY-MM-DD') DATERDV, TO_CHAR(DATERDV, 'HH24:MI') HEURE, NOM||' '||PRENOM NOM_DU_MEDECIN FROM T_RDV, T_MEDECIN WHERE T_MEDECIN.ID_MEDECIN = T_RDV.ID_MEDECIN AND ID_PATIENT = "+ indexPatient +" ORDER BY ID_RDV DESC");
    setupTableWidget(query, ui->tableRDV);
}

void MainWindow::chargement()
{
    if(dbManager->openDatabase())
    {
        QString Qquery = QString("SELECT ID_PATIENT ID, NOM, PRENOM, ADRESSE, EMAIL, TEL, FLOOR(MONTHS_BETWEEN(SYSDATE, DATENAISSANCE) / 12) AGE FROM T_PATIENT WHERE ID_PATIENT = "+ indexPatient);
        QSqlQuery query = dbManager->executeQuery(Qquery);

        while(query.next())
        {
            ui->LA__nom->setText(query.value(2).toString() + " " +query.value(1).toString());
            ui->LA_nom->setText(query.value(1).toString());
            ui->LA_prenom->setText(query.value(2).toString());
            ui->LA_adresse->setText(query.value(3).toString());
            ui->LA_email->setText(query.value(4).toString());
            ui->LA_tel->setText(query.value(5).toString());
            ui->LA_age->setText(query.value(6).toString());
        }

        //modifier ID_DOSSIER_MEDICAL
        Qquery = QString("SELECT * FROM T_CONSULTATION WHERE ID_DOSSIER_MEDICAL = "+ indexDossier);
        query = dbManager->executeQuery(Qquery);

        QVBoxLayout *L = new QVBoxLayout(ui->framebtn);

        while(query.next())
        {
            QDate date = query.value(2).toDate();

            QPushButton *btn = new QPushButton("Consulation " + date.toString("dd/MM/yyyy") );
            btn->setMinimumHeight(50);

            int indice = query.value(0).toInt();
            connect(btn, &QPushButton::clicked, [this, indice]() {
                AfficherConsultation(indice);
            });

            L->addWidget(btn);
        }
        ui->framebtn->setLayout(L);

        dbManager->closeDatabase();
    }
}

void MainWindow::AfficherConsultation(int indice)
{
    if(dbManager->openDatabase())
    {
        QString index = QString::fromStdString(std::to_string(indice));
        QString Qquery = QString("SELECT ID_CONSULTATION, MOTIF, TO_CHAR(DATE_CONSULTATION, 'YYYY-MM-DD') DATE_CONSULTATION, DIAGNOSTIQUE_MEDICAL, POIDS, TAILLE, TEMPERATURE, FREQUENCE_CARDIAQUE, PRESSION_ARTERIELLE, RESULTAT_EXAMEN, OBSERVATION, NOM||' '||PRENOM FROM T_CONSULTATION, T_MEDECIN WHERE T_CONSULTATION.ID_MEDECIN = T_MEDECIN.ID_MEDECIN AND ID_CONSULTATION = " + index);

        QSqlQuery query = dbManager->executeQuery(Qquery);
        while (query.next())
        {
            ui->L_motif->setText(query.value(1).toString());
            ui->L_diag->setText(query.value(3).toString());
            ui->L_poids->setText(query.value(4).toString());
            ui->L_taillee->setText(query.value(5).toString());
            ui->L_temperature->setText(query.value(6).toString());
            ui->L_frequence_cardiaque->setText(query.value(7).toString());
            ui->L_pression_arterielle->setText(query.value(8).toString());
            ui->L_exam->setText(query.value(9).toString());
            ui->L_observation->setText(query.value(10).toString());
            ui->L_nom_medecin_consultation->setText(query.value(11).toString());
            int imc = query.value(4).toInt() / query.value(5).toInt();

            ui->L_Imc->setText(QString::fromStdString(std::to_string(imc)));
        }

        Qquery = QString("SELECT LIBELLE_MEDICAMENT MEDICAMENT, POSOLOGIE FROM T_MEDICAMENT_RECOMMANDE, T_MEDICAMENT, T_POSOLOGIE WHERE T_MEDICAMENT_RECOMMANDE.ID_MEDICAMENT = T_MEDICAMENT.ID_MEDICAMENT AND T_MEDICAMENT_RECOMMANDE.ID_POSOLOGIE = T_POSOLOGIE.ID_POSOLOGIE AND T_MEDICAMENT_RECOMMANDE.ID_CONSULTATION = "+ index);
        setupTableWidget(Qquery, ui->tableOrdonnance);

        Qquery = QString("SELECT LIBELLE_BILAN BILAN FROM T_BILAN_RECOMMANDE, T_BILAN WHERE T_BILAN_RECOMMANDE.ID_BILAN = T_BILAN.ID_BILAN AND ID_CONSULTATION = "+ index);
        setupTableWidget(Qquery, ui->tableBilan);
    }
}

void MainWindow::on_btnDossierMedical_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->btnDossierMedical->setStyleSheet(QString("color: rgb(0, 0, 0);"
                                               "background-color: rgb(227, 237, 243);"
                                               "border: none;"));

    ui->btnRDV->setStyleSheet(QString("color: rgb(255, 255, 255);"
                                               "border: none;"));

    ui->btnMonCompte->setStyleSheet(QString("color: rgb(255, 255, 255);"
                                            "border: none;"));
}

void MainWindow::on_btnRetourGestPatient_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->btnRDV->setStyleSheet(QString("color: rgb(0, 0, 0);"
                                               "background-color: rgb(227, 237, 243);"
                                               "border: none;"));

    ui->btnDossierMedical->setStyleSheet(QString("color: rgb(255, 255, 255);"
                                               "border: none;"));

    ui->btnMonCompte->setStyleSheet(QString("color: rgb(255, 255, 255);"
                                            "border: none;"));
}

void MainWindow::on_btnRDV_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->btnRDV->setStyleSheet(QString("color: rgb(0, 0, 0);"
                                               "background-color: rgb(227, 237, 243);"
                                               "border: none;"));

    ui->btnDossierMedical->setStyleSheet(QString("color: rgb(255, 255, 255);"
                                               "border: none;"));

    ui->btnMonCompte->setStyleSheet(QString("color: rgb(255, 255, 255);"
                                            "border: none;"));
}

void MainWindow::on_L_rechercherMedecin_textChanged(const QString &arg1)
{
    QString query = QString("SELECT DESCRIPTIONRDV DESCRIPTION, TO_CHAR(DATERDV, 'YYYY-MM-DD') DATERDV, TO_CHAR(DATERDV, 'HH24:MI') HEURE, NOM||' '||PRENOM NOM_DU_MEDECIN FROM T_RDV, T_MEDECIN WHERE T_MEDECIN.ID_MEDECIN = T_RDV.ID_MEDECIN AND ID_PATIENT = "+ indexPatient +" AND (UPPER(NOM) LIKE '"+ arg1.toUpper() +"%' OR UPPER(PRENOM) LIKE '"+ arg1.toUpper() +"%')");
    setupTableWidget(query, ui->tableRDV);
}

void MainWindow::on_btn_nettoyerMedecin_clicked()
{
    ui->L_rechercherMedecin->clear();
}

void MainWindow::on_PrendreRDV_clicked()
{
    AjouterRDV *ajout = new AjouterRDV(dbManager, indexPatient, this);
    ajout->show();

    connect(ajout, &AjouterRDV::clickAddRDV, this, MainWindow::actualiserRDV);
}

void MainWindow::on_btnChanger_clicked()
{
    QString username = ui->L_username->text();
    QString password = ui->L_password->text();
    QString confirm = ui->L_password_confirm->text();
    if(username.isEmpty() || password.isEmpty() || confirm.isEmpty())
    {
        QMessageBox::warning(this, "Changer username/password", "Compléter tous les champs manquants");
        return;
    }

    if(!dbManager->openDatabase())
    {
        qDebug() << "Erreur lors de l'ouverture de la base de données";
        return;
    }

    if(password == confirm)
    {
        int question = QMessageBox::question(this, "Changer username/password", "Voulez-vous vraiment changer votre username et password ?", QMessageBox::Yes, QMessageBox::No);
        if(question == QMessageBox::Yes)
        {
            QString Qquery = QString("UPDATE T_LOGIN SET USERNAME = '"+ username +"', PASSWORD = '"+ password +"' WHERE ID_PATIENT = "+ indexPatient);
            QSqlQuery query = dbManager->executeQuery(Qquery);

            if(query.isActive())
            {
                QMessageBox::information(this, "Changer username/password", "Votre username et password a été modifier");
                ui->L_username->clear();
                ui->L_password->clear();
                ui->L_password_confirm->clear();
                dbManager->closeDatabase();
                return;
            }
            QMessageBox::critical(this, "Changer username/password", "Erreur lors de la modification du username et password");
        }
    }
}

void MainWindow::on_btnMonCompte_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->btnMonCompte->setStyleSheet(QString("color: rgb(0, 0, 0);"
                                               "background-color: rgb(227, 237, 243);"
                                               "border: none;"));

    ui->btnDossierMedical->setStyleSheet(QString("color: rgb(255, 255, 255);"
                                               "border: none;"));

    ui->btnRDV->setStyleSheet(QString("color: rgb(255, 255, 255);"
                                            "border: none;"));
}
