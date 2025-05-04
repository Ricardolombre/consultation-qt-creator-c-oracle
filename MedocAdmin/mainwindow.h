#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <connexion/databasemanager.h>
#include <QTableWidget>
#include <QComboBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnGererPatient_clicked();

    void on_btnGererMedecin_clicked();

    void on_ajouterPatient_clicked();

    void ActualiserPatient();

    void ActualiserMedecin();

    void setupTableWidget(const QString &queryString, QTableWidget* tableWidget);

    void setupTableMedecin(const QString &queryString, QTableWidget* tableWidget);

    void setupTable(const QString &queryString, QTableWidget* tableWidget);

    void Ajouter(QComboBox* CO_par, QString query, int index);

    void on_L_rechercherPatient_textChanged(const QString &arg1);

    void on_btnAjouterMedecin_clicked();

    void on_L_rechercherMedecin_textChanged(const QString &arg1);

    void DetailsPatient(int indice);

    void AfficherConsultation(int indice);

    void on_btnRetourGestPatient_clicked();

    void on_btn_nettoyerPatient_clicked();

    void on_btn_nettoyer_clicked();

    void on_btnAjouterSpecialite_clicked();

    void existe_deja();

    void on_btnAjouterMedicament_clicked();

    void on_btnAjouterPosologie_clicked();

    void on_btnAjouterBilan_clicked();

    void on_btnParametre_clicked();

    void on_btnStatistique_clicked();

    void statistique();

private:
    Ui::MainWindow *ui;

    DatabaseManager *dbManager;
};

#endif // MAINWINDOW_H
