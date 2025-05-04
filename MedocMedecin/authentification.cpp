#include "authentification.h"
#include "ui_authentification.h"
#include "mainwindow.h"
#include "connexion/databasemanager.h"
#include "connexion/databasemanager.h"
#include <QSqlRecord>
#include <QDebug>

Authentification::Authentification(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Authentification)
{
    ui->setupUi(this);
}

Authentification::~Authentification()
{
    delete ui;
}

void Authentification::on_btnconnexion_clicked()
{
    QString username = ui->L_user_name->text();
    QString password = ui->L_mot_de_passe->text();
    QString idmedecin;

    DatabaseManager *dbManager = new DatabaseManager();
    if(dbManager->openDatabase())
    {
        QString Qquery = QString("SELECT * FROM T_LOGINN WHERE username = '"+ username +"' AND password = '"+ password +"'");
        QSqlQuery query = dbManager->executeQuery(Qquery);
        int count = 0;

        while (query.next()) {
            idmedecin = query.value(3).toString();
            count++;
        }

        qDebug() << count;
        if(count <= 0){
            ui->la_erreur->setText("username ou password invalide");
            return;
        }

        MainWindow *mainWindow = new MainWindow(NULL, idmedecin, dbManager);
        mainWindow->show();

        this->close();
    }
}
