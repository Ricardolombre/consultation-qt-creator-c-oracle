#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHBoxLayout>
#include <QCursor>
#include "ajouterpatient.h"
#include "ajoutermedecin.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlRecord>

#include "specialite/specialite.h"
#include "medicament/medicament.h"
#include "posologie/posologie.h"
#include "bilan/bilan.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dbManager = new DatabaseManager();

    ActualiserPatient();
    ActualiserMedecin();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnGererPatient_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->btnGererPatient->setStyleSheet(QString("color: rgb(0, 0, 0);"
                                               "background-color: rgb(227, 237, 243);"
                                               "border: none;"));

    ui->btnGererMedecin->setStyleSheet(QString("color: rgb(255, 255, 255);"
                                               "border: none;"));

    ui->btnParametre->setStyleSheet(QString("color: rgb(255, 255, 255);"
                                               "border: none;"));

    ui->btnStatistique->setStyleSheet(QString("color: rgb(255, 255, 255);"
                                               "border: none;"));
}

void MainWindow::on_btnGererMedecin_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->btnGererMedecin->setStyleSheet(QString("color: rgb(0, 0, 0);"
                                               "background-color: rgb(227, 237, 243);"
                                               "border: none;"));

    ui->btnGererPatient->setStyleSheet(QString("color: rgb(255, 255, 255);"
                                               "border: none;"));

    ui->btnParametre->setStyleSheet(QString("color: rgb(255, 255, 255);"
                                               "border: none;"));

    ui->btnStatistique->setStyleSheet(QString("color: rgb(255, 255, 255);"
                                               "border: none;"));
}

void MainWindow::on_ajouterPatient_clicked()
{
    AjouterPatient *ajout = new AjouterPatient(dbManager, this);
    ajout->show();

    connect(ajout, &AjouterPatient::clickAddPatient, this, MainWindow::ActualiserPatient);
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
            tableWidget->setColumnCount(columnCount + 1); // Ajouter une colonne pour les actions
            for (int i = 0; i < columnCount; ++i) {
                tableWidget->setHorizontalHeaderItem(i, new QTableWidgetItem(query.record().fieldName(i)));
            }
            tableWidget->setHorizontalHeaderItem(columnCount, new QTableWidgetItem("Action"));

            // Remplir le tableau avec les données
            int row = 0;
            while (query.next()) {
                tableWidget->insertRow(row);
                for (int col = 0; col < columnCount; ++col) {
                    QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
                    item->setFlags(item->flags() & ~Qt::ItemIsEditable); // Rendre l'élément non modifiable
                    tableWidget->setItem(row, col, item);
                }

                // Ajouter les boutons dans la colonne "Action"
                QWidget *actionWidget = new QWidget();
                QPushButton *editButton = new QPushButton("Details");
                editButton->setMinimumHeight(30);
                QPushButton *deleteButton = new QPushButton("Supprimer");
                deleteButton->setMinimumHeight(30);
                deleteButton->setStyleSheet(QString("border: none;"
                                                    "background-color: red;"));
                QHBoxLayout *layout = new QHBoxLayout(actionWidget);
                layout->addWidget(editButton);
                layout->addWidget(deleteButton);
                layout->setContentsMargins(0, 0, 0, 0);
                layout->setSpacing(0);
                actionWidget->setLayout(layout);
                tableWidget->setCellWidget(row, columnCount, actionWidget);

                // Connecter les boutons à des slots pour les actions
                int indice = query.value(0).toInt();
                connect(editButton, &QPushButton::clicked, [this, indice]() {
                    DetailsPatient(indice);
                });

                connect(deleteButton, &QPushButton::clicked, [this, indice]() {
                    int question = QMessageBox::warning(this,QString("Suppression d'un patient"),QString("Voulez-vous vraiment supprimer ce patient ?"), QMessageBox::Yes, QMessageBox::No);
                    if(question == QMessageBox::Yes){
                        if(dbManager->openDatabase()){
                            QString query = QString("DELETE FROM T_PATIENT WHERE id_patient = "+ QString::fromStdString(std::to_string(indice)));
                            dbManager->executeQuery(query);
                            ActualiserPatient();
                        }
                    }
                });

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


void MainWindow::setupTableMedecin(const QString &queryString, QTableWidget* tableWidget)
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
            tableWidget->setColumnCount(columnCount + 1); // Ajouter une colonne pour les actions
            for (int i = 0; i < columnCount; ++i) {
                tableWidget->setHorizontalHeaderItem(i, new QTableWidgetItem(query.record().fieldName(i)));
            }
            tableWidget->setHorizontalHeaderItem(columnCount, new QTableWidgetItem("Action"));

            // Remplir le tableau avec les données
            int row = 0;
            while (query.next()) {
                tableWidget->insertRow(row);
                for (int col = 0; col < columnCount; ++col) {
                    QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
                    item->setFlags(item->flags() & ~Qt::ItemIsEditable); // Rendre l'élément non modifiable
                    tableWidget->setItem(row, col, item);
                }

                // Ajouter les boutons dans la colonne "Action"
                QWidget *actionWidget = new QWidget();
                QPushButton *deleteButton = new QPushButton("Supprimer");
                deleteButton->setMinimumHeight(30);
                deleteButton->setStyleSheet(QString("border: none;"
                                                    "background-color: red;"));
                QHBoxLayout *layout = new QHBoxLayout(actionWidget);
                layout->addWidget(deleteButton);
                layout->setContentsMargins(0, 0, 0, 0);
                layout->setSpacing(0);
                actionWidget->setLayout(layout);
                tableWidget->setCellWidget(row, columnCount, actionWidget);

                // Connecter les boutons à des slots pour les actions
                int indice = query.value(0).toInt();

                connect(deleteButton, &QPushButton::clicked, [this, indice]() {
                    int question = QMessageBox::warning(this,QString("Suppression d'un medecin"),QString("Voulez-vous vraiment supprimer ce medecin ?"), QMessageBox::Yes, QMessageBox::No);
                    if(question == QMessageBox::Yes){
                        if(dbManager->openDatabase()){
                            QString query = QString("DELETE FROM T_MEDECIN WHERE id_medecin = "+ QString::fromStdString(std::to_string(indice)));
                            dbManager->executeQuery(query);
                            ActualiserMedecin();
                        }
                    }
                });

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

void MainWindow::setupTable(const QString &queryString, QTableWidget* tableWidget)
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

void MainWindow::Ajouter(QComboBox* CO_par,QString query,int index)
{
    if(dbManager->openDatabase())
    {
        QSqlQuery queryString = dbManager->executeQuery(query);
        if(dbManager->executeQuery(query).isActive())
        {
            CO_par->clear();
            while (queryString.next())
            {
                QString valeur = queryString.value(index).toString();
                qDebug() << valeur;
                CO_par->addItem(valeur);
            }
        }
        dbManager->closeDatabase();
    }
}

void MainWindow::on_L_rechercherPatient_textChanged(const QString &arg1)
{
    if(ui->ra_nomPatient->isChecked())
    {
        QString query = QString("SELECT ID_PATIENT ID, NOM, PRENOM, ADRESSE, EMAIL, TEL, FLOOR(MONTHS_BETWEEN(SYSDATE, DATENAISSANCE) / 12) AGE  FROM T_PATIENT WHERE UPPER(NOM) LIKE '"+ arg1.toUpper() +"%' OR UPPER(PRENOM) LIKE '"+ arg1.toUpper() +"%' ORDER BY ID_PATIENT ASC");
        setupTableWidget(query, ui->tablePatient);
    }
    else
    {
        QString query = QString("SELECT ID_PATIENT ID, NOM, PRENOM, ADRESSE, EMAIL, TEL, FLOOR(MONTHS_BETWEEN(SYSDATE, DATENAISSANCE) / 12) AGE  FROM T_PATIENT WHERE FLOOR(MONTHS_BETWEEN(SYSDATE, DATENAISSANCE) / 12) = "+ arg1 +" ORDER BY ID_PATIENT ASC");
        setupTableWidget(query, ui->tablePatient);
    }
}

void MainWindow::ActualiserPatient()
{
    QString query = QString("SELECT ID_PATIENT ID, NOM, PRENOM, ADRESSE, EMAIL, TEL, FLOOR(MONTHS_BETWEEN(SYSDATE, DATENAISSANCE) / 12) AGE FROM T_PATIENT ORDER BY ID_PATIENT ASC");
    setupTableWidget(query, ui->tablePatient);
}

void MainWindow::ActualiserMedecin()
{
    QString query = QString("SELECT m.id_medecin, m.nom, m.prenom, m.email, m.adresse, m.tel, s.libelle_specialite FROM t_medecin m, t_specialite s WHERE m.id_specialite = s.id_specialite ORDER BY ID_MEDECIN ASC");
    setupTableMedecin(query, ui->tableMedecin);
}

void MainWindow::on_btnAjouterMedecin_clicked()
{
    AjouterMedecin *ajout = new AjouterMedecin(dbManager, this);
    ajout->show();

    connect(ajout, &AjouterMedecin::clickAddMedecin, this, MainWindow::ActualiserMedecin);
}


void MainWindow::on_L_rechercherMedecin_textChanged(const QString &arg1)
{
    if(ui->R_nomM->isChecked())
    {
        QString query = QString("SELECT m.id_medecin, m.nom, m.prenom, m.email, m.adresse, m.tel, s.libelle_specialite FROM t_medecin m, t_specialite s WHERE m.id_specialite = s.id_specialite AND (UPPER(NOM) LIKE '"+ arg1.toUpper() +"%' OR UPPER(PRENOM) LIKE '"+ arg1.toUpper() +"%') ORDER BY ID_MEDECIN ASC");
        setupTableMedecin(query, ui->tableMedecin);
    }
    else
    {
        QString query = QString("SELECT m.id_medecin, m.nom, m.prenom, m.email, m.adresse, m.tel, s.libelle_specialite FROM t_medecin m, t_specialite s WHERE m.id_specialite = s.id_specialite AND UPPER(s.libelle_specialite) LIKE '"+ arg1.toUpper() +"%' ORDER BY ID_MEDECIN ASC");
        setupTableMedecin(query, ui->tableMedecin);
    }
}

void MainWindow::DetailsPatient(int indice)
{
    ui->L_motif->clear();
    ui->L_diag->clear();
    ui->L_poids->clear();
    ui->L_taillee->clear();
    ui->L_temperature->clear();
    ui->L_frequence_cardiaque->clear();
    ui->L_pression_arterielle->clear();
    ui->L_exam->clear();
    ui->L_observation->clear();
    ui->L_Imc->clear();

    if(dbManager->openDatabase())
    {
        QString index = QString::fromStdString(std::to_string(indice));
        QString Qquery = QString("SELECT ID_PATIENT ID, NOM, PRENOM, ADRESSE, EMAIL, TEL, FLOOR(MONTHS_BETWEEN(SYSDATE, DATENAISSANCE) / 12) AGE FROM T_PATIENT WHERE ID_PATIENT = "+ index);
        QSqlQuery query = dbManager->executeQuery(Qquery);

        while(query.next())
        {
            ui->LA_nom->setText(query.value(1).toString());
            ui->LA_prenom->setText(query.value(2).toString());
            ui->LA_adresse->setText(query.value(3).toString());
            ui->LA_email->setText(query.value(4).toString());
            ui->LA_tel->setText(query.value(5).toString());
            ui->LA_age->setText(query.value(6).toString());

            ui->stackedWidget->setCurrentIndex(2);
        }

        QLayout* layout = ui->framebtn->layout();
        if (layout != nullptr) {
            QLayoutItem* item;
            while ((item = layout->takeAt(0)) != nullptr) {
                delete item->widget();  // Supprime le widget
                delete item;            // Supprime l'élément de layout
            }
            delete layout;  // Supprime le layout
        }

        //modifier ID_DOSSIER_MEDICAL
        Qquery = QString("SELECT * FROM T_CONSULTATION WHERE ID_DOSSIER_MEDICAL = "+ index);
        query = dbManager->executeQuery(Qquery);

        QVBoxLayout *L = new QVBoxLayout(ui->framebtn);

        while(query.next())
        {
            QPushButton *btn = new QPushButton("Consulation " + query.value(2).toString());
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

void MainWindow::on_btnRetourGestPatient_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_btn_nettoyerPatient_clicked()
{
    ui->L_rechercherPatient->clear();
}

void MainWindow::on_btn_nettoyer_clicked()
{
    ui->L_rechercherMedecin->clear();
}

void MainWindow::AfficherConsultation(int indice)
{
    if(dbManager->openDatabase())
    {
        QString index = QString::fromStdString(std::to_string(indice));
        QString Qquery = QString("SELECT * FROM T_CONSULTATION WHERE ID_CONSULTATION = " + index);

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

            int imc = query.value(4).toInt() / query.value(5).toInt();
            ui->L_Imc->setText(QString::fromStdString(std::to_string(imc)));
        }

        Qquery = QString("SELECT LIBELLE_MEDICAMENT MEDICAMENT, POSOLOGIE FROM T_MEDICAMENT_RECOMMANDE, T_MEDICAMENT, T_POSOLOGIE WHERE T_MEDICAMENT_RECOMMANDE.ID_MEDICAMENT = T_MEDICAMENT.ID_MEDICAMENT AND T_MEDICAMENT_RECOMMANDE.ID_POSOLOGIE = T_POSOLOGIE.ID_POSOLOGIE AND T_MEDICAMENT_RECOMMANDE.ID_CONSULTATION = "+ index );
        setupTable(Qquery, ui->tableOrdonnance);

        Qquery = QString("SELECT LIBELLE_BILAN BILAN FROM T_BILAN_RECOMMANDE, T_BILAN WHERE T_BILAN_RECOMMANDE.ID_BILAN = T_BILAN.ID_BILAN AND ID_CONSULTATION = "+ index);
        setupTable(Qquery, ui->tableBilan);
    }
}

void MainWindow::on_btnAjouterSpecialite_clicked()
{
    QString lib = ui->L_specialite->text();

    Specialite *specialite = new Specialite(lib);
    if(specialite->Ajouter(dbManager))
    {
        QMessageBox::information(this, "Ajout", "La spécialité a été ajouté avec succès");
        ui->L_specialite->clear();
    }
    else
    {
        existe_deja();
    }

    delete specialite;
}

void MainWindow::existe_deja()
{
    QMessageBox::warning(this, "Verification" ,"La valeur que vous voulez enregistrer existe déjà");
}

void MainWindow::on_btnAjouterMedicament_clicked()
{
    QString lib = ui->L_medicalent->text();

    Medicament *medicament = new Medicament(lib);
    if(medicament->Ajouter(dbManager))
    {
        QMessageBox::information(this, "Ajout", "Le médicament a été ajouté avec succès");
        ui->L_medicalent->clear();
    }
    else
    {
        existe_deja();
    }

    delete medicament;
}

void MainWindow::on_btnAjouterPosologie_clicked()
{
    QString lib = ui->L_posologie->text();

    Posologie *posologie = new Posologie(lib);
    if(!posologie->Ajouter(dbManager))
    {
        QMessageBox::information(this, "Ajout", "La posologie a été ajouté avec succès");
        ui->L_posologie->clear();
    }
    else
    {
        existe_deja();
    }

    delete posologie;
}

void MainWindow::on_btnAjouterBilan_clicked()
{
    QString lib = ui->L_bilan->text();

    Bilan *bilan = new Bilan(lib);
    if(bilan->Ajouter(dbManager))
    {
        QMessageBox::information(this, "Ajout", "Le bilan a été ajouté avec succès");
        ui->L_bilan->clear();
    }
    else
    {
        existe_deja();
    }

    delete bilan;
}

void MainWindow::on_btnParametre_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    ui->btnParametre->setStyleSheet(QString("color: rgb(0, 0, 0);"
                                               "background-color: rgb(227, 237, 243);"
                                               "border: none;"));

    ui->btnGererPatient->setStyleSheet(QString("color: rgb(255, 255, 255);"
                                               "border: none;"));

    ui->btnGererMedecin->setStyleSheet(QString("color: rgb(255, 255, 255);"
                                               "border: none;"));

    ui->btnStatistique->setStyleSheet(QString("color: rgb(255, 255, 255);"
                                               "border: none;"));
}

void MainWindow::on_btnStatistique_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
    ui->btnStatistique->setStyleSheet(QString("color: rgb(0, 0, 0);"
                                               "background-color: rgb(227, 237, 243);"
                                               "border: none;"));

    ui->btnGererPatient->setStyleSheet(QString("color: rgb(255, 255, 255);"
                                               "border: none;"));

    ui->btnParametre->setStyleSheet(QString("color: rgb(255, 255, 255);"
                                               "border: none;"));

    ui->btnGererMedecin->setStyleSheet(QString("color: rgb(255, 255, 255);"
                                               "border: none;"));
    statistique();
}

void MainWindow::statistique()
{
    QString Qquery = QString("SELECT COUNT(ID_MEDECIN) FROM T_MEDECIN");
    if(dbManager->openDatabase())
    {
        QSqlQuery query = dbManager->executeQuery(Qquery);
        while(query.next())
        {
            ui->la_nombre_medecin->setText(query.value(0).toString());
        }

        Qquery = QString("SELECT COUNT(ID_PATIENT) FROM T_PATIENT");
        query = dbManager->executeQuery(Qquery);
        while(query.next())
        {
            ui->la_nombre_patient->setText(query.value(0).toString());
        }

        Qquery = QString("SELECT COUNT(ID_RDV) FROM T_RDV");
        query = dbManager->executeQuery(Qquery);
        while(query.next())
        {
            ui->la_nombre_rdv->setText(query.value(0).toString());
        }

        Qquery = QString("SELECT COUNT(ID_CONSULTATION) FROM T_CONSULTATION");
        query = dbManager->executeQuery(Qquery);
        while(query.next())
        {
            ui->la_nombre_consultation->setText(query.value(0).toString());
        }
    }


}
