#ifndef AJOUTERMEDECIN_H
#define AJOUTERMEDECIN_H

#include <QDialog>
#include "connexion/databasemanager.h"
#include <QComboBox>

namespace Ui {
class AjouterMedecin;
}

class AjouterMedecin : public QDialog
{
    Q_OBJECT

public:
    explicit AjouterMedecin(DatabaseManager *_db, QWidget *parent = 0);
    ~AjouterMedecin();

private slots:
    void on_femer_clicked();

    void Ajouter(QComboBox* CO_par, QString query, QString index);

    void on_btnAjoutMedecin_clicked();

signals:
    void clickAddMedecin();

private:
    Ui::AjouterMedecin *ui;
    DatabaseManager *db;
};

#endif // AJOUTERMEDECIN_H
