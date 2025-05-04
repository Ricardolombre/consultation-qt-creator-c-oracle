#ifndef AJOUTERBILAN_H
#define AJOUTERBILAN_H

#include <QDialog>
#include <QComboBox>
#include "connexion/databasemanager.h"
#include "Bilan_recommande/bilan_recommande.h"

namespace Ui {
class ajouterBilan;
}

class ajouterBilan : public QDialog
{
    Q_OBJECT

public:
    explicit ajouterBilan(DatabaseManager *_db, QString _ordonnance, QWidget *parent = 0);
    ~ajouterBilan();

    std::vector<Bilan_recommande> getBilan();

private slots:
    void on_btnAjouterBilan_clicked();

    void Ajouter(QComboBox* CO_par,QString query,QString index);

    void add();

    void on_btnSauvegarger_clicked();

    void on_femer_clicked();

private:
    Ui::ajouterBilan *ui;
    QString ordonnance;
    std::vector<QString> listBilan;
    std::vector<Bilan_recommande> list;

    DatabaseManager *db;
    int rowCount;
};

#endif // AJOUTERBILAN_H
