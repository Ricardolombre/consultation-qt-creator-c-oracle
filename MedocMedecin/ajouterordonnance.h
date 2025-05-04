#ifndef AJOUTERORDONNANCE_H
#define AJOUTERORDONNANCE_H

#include <QDialog>
#include <QComboBox>
#include "connexion/databasemanager.h"
#include "Medicament_recommande/medicament_recommande.h"

namespace Ui {
class AjouterOrdonnance;
}

class AjouterOrdonnance : public QDialog
{
    Q_OBJECT

public:
    explicit AjouterOrdonnance(DatabaseManager *_db, QString _ordonnance, QWidget *parent = 0);
    ~AjouterOrdonnance();
    std::vector<Medicament_recommande> getMedicament();

private slots:
    void on_btnAjouterMedicament_clicked();

    void add();

    void Ajouter(QComboBox* CO_par,QString query,QString index);

    void on_btnSauvegarger_clicked();

    void on_femer_clicked();

private:
    Ui::AjouterOrdonnance *ui;
    QString ordonnance;
    std::vector<QString> listmedicament;
    std::vector<QString> listposologie;
    std::vector<Medicament_recommande> list;

    DatabaseManager *db;
    int rowCount;
};

#endif // AJOUTERORDONNANCE_H
