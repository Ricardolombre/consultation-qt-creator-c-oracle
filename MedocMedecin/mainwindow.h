#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "connexion/databasemanager.h"
#include "ajouterordonnance.h"
#include "ajouterbilan.h"
#include <QTableWidget>
#include <QComboBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0, QString _idMedecin = "0", DatabaseManager *db = NULL);
    ~MainWindow();

public slots:
    void setupTableWidget(const QString &queryString, QTableWidget* tableWidget);

    void setupTablePatient(const QString &queryString, QTableWidget* tableWidget);

    void actualiserRDV();

    void ActualiserPatient();

    void DetailsPatient(int indice);

    void AfficherConsultation(int indice);

    void enregistrerConsultation(QString _index);

    void chargement();

private slots:
    void on_btnRetourGestPatient_clicked();

    void on_btnRDV_clicked();

    void on_btnListePatient_clicked();

    void on_btnOrdonnance_clicked();

    void on_btnBilan_clicked();

    void on_L_rechercherRDV_textChanged(const QString &arg1);

    void on_PrendreRDV_clicked();


    void on_btnMoncompte_clicked();

    void on_btnChanger_clicked();

    void on_L_rechercherPatient_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;

    DatabaseManager *dbManager;
    AjouterOrdonnance *ajout;
    ajouterBilan *ajoutbilan;

    QString idMedecin;
};

#endif // MAINWINDOW_H
