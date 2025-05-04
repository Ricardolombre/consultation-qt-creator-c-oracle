#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QMessageBox>
#include <QSqlRecord>
#include <QDate>

#include "consultation/consultation.h"
#include "Medicament_recommande/medicament_recommande.h"
#include "ajouterrdv.h"

MainWindow::MainWindow(QWidget *parent, QString _idMedecin, DatabaseManager *db) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    dbManager = db;

    idMedecin = _idMedecin;

    actualiserRDV();
    ActualiserPatient();
    chargement();

    ajout = new AjouterOrdonnance(dbManager, "0", this);
    ajoutbilan = new ajouterBilan(dbManager, "0", this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupTablePatient(const QString &queryString, QTableWidget* tableWidget)
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

                QHBoxLayout *layout = new QHBoxLayout(actionWidget);
                layout->addWidget(editButton);
                layout->setContentsMargins(0, 0, 0, 0);
                layout->setSpacing(0);
                actionWidget->setLayout(layout);
                tableWidget->setCellWidget(row, columnCount, actionWidget);

                // Connecter les boutons à des slots pour les actions
                int indice = query.value(0).toInt();
                connect(editButton, &QPushButton::clicked, [this, indice]() {
                    DetailsPatient(indice);
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
    QString query = QString("SELECT DESCRIPTIONRDV DESCRIPTION, TO_CHAR(DATERDV, 'YYYY-MM-DD') DATERDV, TO_CHAR(DATERDV, 'HH24:MI') HEURE, NOM||' '||PRENOM PATIENT FROM T_RDV, T_PATIENT WHERE T_PATIENT.ID_PATIENT = T_RDV.ID_PATIENT AND T_RDV.ID_MEDECIN = "+ idMedecin);
    setupTableWidget(query, ui->tableRDV);
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

        connect(ui->btnRetourGestPatient_2, &QPushButton::clicked, [this, index](){
            enregistrerConsultation(index);
        });

        QVBoxLayout *L = new QVBoxLayout(ui->framebtn);

        while(query.next())
        {
            QDate date = query.value(2).toDate();

            QPushButton *btn = new QPushButton("Consulation " + date.toString("dd/MM/yyyy"));
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

void MainWindow::ActualiserPatient()
{
    QString query = QString("SELECT ID_PATIENT ID, NOM, PRENOM, ADRESSE, EMAIL, TEL, FLOOR(MONTHS_BETWEEN(SYSDATE, DATENAISSANCE) / 12) AGE FROM T_PATIENT ORDER BY ID_PATIENT ASC");
    setupTablePatient(query, ui->tablePatient);
}

void MainWindow::AfficherConsultation(int indice)
{
    if(dbManager->openDatabase())
    {
        QString index = QString::fromStdString(std::to_string(indice));
        QString Qquery = QString("SELECT ID_CONSULTATION, MOTIF, TO_CHAR(DATE_CONSULTATION, 'YYYY-MM-DD') DATECONSULTATION, DIAGNOSTIQUE_MEDICAL, POIDS, TAILLE, TEMPERATURE, FREQUENCE_CARDIAQUE, PRESSION_ARTERIELLE, RESULTAT_EXAMEN, OBSERVATION, NOM||' '||PRENOM FROM T_CONSULTATION, T_MEDECIN WHERE T_CONSULTATION.ID_MEDECIN = T_MEDECIN.ID_MEDECIN AND ID_CONSULTATION = " + index);

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

void MainWindow::on_btnRetourGestPatient_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->btnRDV->setStyleSheet(QString("color: rgb(0, 0, 0);"
                                               "background-color: rgb(227, 237, 243);"
                                               "border: none;"));

    ui->btnListePatient->setStyleSheet(QString("color: rgb(255, 255, 255);"
                                               "border: none;"));

    ui->btnMoncompte->setStyleSheet(QString("color: rgb(255, 255, 255);"
                                            "border: none;"));
}

void MainWindow::on_btnRDV_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->btnRDV->setStyleSheet(QString("color: rgb(0, 0, 0);"
                                               "background-color: rgb(227, 237, 243);"
                                               "border: none;"));

    ui->btnListePatient->setStyleSheet(QString("color: rgb(255, 255, 255);"
                                               "border: none;"));

    ui->btnMoncompte->setStyleSheet(QString("color: rgb(255, 255, 255);"
                                            "border: none;"));
}

void MainWindow::on_btnListePatient_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->btnListePatient->setStyleSheet(QString("color: rgb(0, 0, 0);"
                                               "background-color: rgb(227, 237, 243);"
                                               "border: none;"));

    ui->btnRDV->setStyleSheet(QString("color: rgb(255, 255, 255);"
                                               "border: none;"));

    ui->btnMoncompte->setStyleSheet(QString("color: rgb(255, 255, 255);"
                                            "border: none;"));
}

//btn enregistrer consultation
////////////////////////

void MainWindow::enregistrerConsultation(QString _index)
{
    QString motif = ui->LE_motif->text();
    QString resultat = ui->T_resultat_examen->document()->toPlainText();
    QString diagnostique = ui->T_diagnostique->document()->toPlainText();
    QString poids = ui->LE_poids->text();
    QString taille = ui->LE_taille->text();
    QString temperature = ui->LE_temperature->text();
    QString frequence = ui->LE_frequence_cardiaque->text();
    QString pression = ui->LE_pression_arterielle->text();
    QString observation = ui->LE_observation->text();

    Consultation consutation(motif, resultat, diagnostique,poids, taille, temperature, frequence, pression, observation, "1", _index);
    if(consutation.Ajouter(dbManager))
    {
        QMessageBox::information(this, "Ajout", "La consultation a bien été enregistré");
        ui->LE_motif->clear();
        ui->T_resultat_examen->clear();
        ui->T_diagnostique->clear();
        ui->LE_poids->clear();
        ui->LE_taille->clear();
        ui->LE_temperature->clear();
        ui->LE_frequence_cardiaque->clear();
        ui->LE_pression_arterielle->clear();
        ui->LE_observation->clear();
        ui->LE_imc->clear();

        std::vector<Medicament_recommande> medo = ajout->getMedicament();
        int size = medo.size();
        for(int i = 0; i < size; i++)
        {
            medo[i].Ajouter(dbManager);
        }

        std::vector<Bilan_recommande> bilan = ajoutbilan->getBilan();
        size = bilan.size();
        for(int i = 0; i < size; i++)
        {
            bilan[i].Ajouter(dbManager);
        }
    }
    else
    {
        QMessageBox::critical(this, "Erreur", "Une erreur s'est produite lors de l'ajout de la consultation");
    }
}

void MainWindow::on_btnOrdonnance_clicked()
{
    if(dbManager->openDatabase())
    {
        QString query = QString("SELECT MAX(ID_CONSULTATION) AS id FROM T_CONSULTATION");
        QSqlQuery Qquery = dbManager->executeQuery(query);
        QString id;
        while(Qquery.next()){
            int id__int = Qquery.value("id").toInt() + 1;
            id = QString::fromStdString(std::to_string(id__int));
        }

        delete ajout;

        ajout = new AjouterOrdonnance(dbManager, id, this);
        ajout->show();
    }
}

void MainWindow::on_btnBilan_clicked()
{
    if(dbManager->openDatabase())
    {
        QString query = QString("SELECT MAX(ID_CONSULTATION) AS id FROM T_CONSULTATION");
        QSqlQuery Qquery = dbManager->executeQuery(query);
        QString id;
        while(Qquery.next()){
            int id__int = Qquery.value("id").toInt() + 1;
            id = QString::fromStdString(std::to_string(id__int));
        }

        delete ajoutbilan;

        ajoutbilan = new ajouterBilan(dbManager, id, this);
        ajoutbilan->show();
    }
}


void MainWindow::on_L_rechercherRDV_textChanged(const QString &arg1)
{
    QString query = QString("SELECT DESCRIPTIONRDV DESCRIPTION, TO_CHAR(DATERDV, 'YYYY-MM-DD') DATERDV, TO_CHAR(DATERDV, 'HH24:MI') HEURE, NOM||' '||PRENOM PATIENT FROM T_RDV, T_PATIENT WHERE T_PATIENT.ID_PATIENT = T_RDV.ID_PATIENT AND T_RDV.ID_MEDECIN = "+ idMedecin +" AND (UPPER(NOM) LIKE '"+ arg1.toUpper() +"%' OR UPPER(PRENOM) LIKE '"+ arg1.toUpper() +"')");
    setupTableWidget(query, ui->tableRDV);
}

void MainWindow::on_PrendreRDV_clicked()
{
    AjouterRDV *ajout = new AjouterRDV(dbManager, idMedecin, this);
    ajout->show();

    connect(ajout, &AjouterRDV::clickAddRDV, this, MainWindow::actualiserRDV);
}

void MainWindow::chargement()
{
    if(dbManager->openDatabase())
    {
        QString Qquery = QString("SELECT NOM, PRENOM, ADRESSE, EMAIL, TEL, LIBELLE_SPECIALITE FROM T_MEDECIN, T_SPECIALITE WHERE T_MEDECIN.ID_SPECIALITE = T_SPECIALITE.ID_SPECIALITE AND ID_MEDECIN = "+ idMedecin);
        QSqlQuery query = dbManager->executeQuery(Qquery);

        while(query.next())
        {
            ui->LA__nom->setText(query.value(0).toString() + " " +query.value(1).toString());
            ui->LA_nom_M->setText(query.value(0).toString());
            ui->LA_prenom_M->setText(query.value(1).toString());
            ui->LA_adresse_M->setText(query.value(2).toString());
            ui->LA_email_M->setText(query.value(3).toString());
            ui->LA_tel_M->setText(query.value(4).toString());
            ui->LA_specialite_M->setText(query.value(5).toString());
        }

        dbManager->closeDatabase();
    }
}

void MainWindow::on_btnMoncompte_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    ui->btnMoncompte->setStyleSheet(QString("color: rgb(0, 0, 0);"
                                               "background-color: rgb(227, 237, 243);"
                                               "border: none;"));

    ui->btnListePatient->setStyleSheet(QString("color: rgb(255, 255, 255);"
                                               "border: none;"));

    ui->btnRDV->setStyleSheet(QString("color: rgb(255, 255, 255);"
                                            "border: none;"));
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
            QString Qquery = QString("UPDATE T_LOGINN SET USERNAME = '"+ username +"', PASSWORD = '"+ password +"' WHERE ID_MEDECIN = "+ idMedecin);
            QSqlQuery query = dbManager->executeQuery(Qquery);

            if(query.isActive())
            {
                QMessageBox::information(this, "Changer username/password", "Votre username et password a été modifier");
                dbManager->closeDatabase();
                return;
            }
            QMessageBox::critical(this, "Changer username/password", "Erreur lors de la modification du username et password");
        }
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
