#ifndef AJOUTERPATIENT_H
#define AJOUTERPATIENT_H

#include <QDialog>
#include "connexion/databasemanager.h"

namespace Ui {
class AjouterPatient;
}

class AjouterPatient : public QDialog
{
    Q_OBJECT

public:
    explicit AjouterPatient(DatabaseManager *_db, QWidget *parent = NULL);
    ~AjouterPatient();

private slots:
    void on_femer_clicked();

    void on_btnAjoutPatient_clicked();

signals:
    void clickAddPatient();


private:
    Ui::AjouterPatient *ui;
    DatabaseManager *db;
};

#endif // AJOUTERPATIENT_H
