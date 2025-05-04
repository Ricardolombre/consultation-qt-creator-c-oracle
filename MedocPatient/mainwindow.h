#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "connexion/databasemanager.h"
#include <QTableWidget>
#include <QComboBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0, QString patient = "0", QString dossier = "0", DatabaseManager *_dbManager = NULL);
    ~MainWindow();

public slots:
    void setupTableWidget(const QString &queryString, QTableWidget* tableWidget);

    void actualiserRDV();

    void chargement();

    void AfficherConsultation(int indice);

private slots:
    void on_btnDossierMedical_clicked();

    void on_btnRetourGestPatient_clicked();

    void on_btnRDV_clicked();

    void on_L_rechercherMedecin_textChanged(const QString &arg1);

    void on_btn_nettoyerMedecin_clicked();

    void on_PrendreRDV_clicked();

    void on_btnChanger_clicked();

    void on_btnMonCompte_clicked();

private:
    Ui::MainWindow *ui;
    QString indexPatient;
    QString indexDossier;

    DatabaseManager *dbManager;
};

#endif // MAINWINDOW_H
