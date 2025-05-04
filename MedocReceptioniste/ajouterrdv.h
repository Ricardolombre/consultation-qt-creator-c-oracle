#ifndef AJOUTERRDV_H
#define AJOUTERRDV_H

#include <QDialog>
#include <QCompleter>
#include "connexion/databasemanager.h"

namespace Ui {
class AjouterRDV;
}

class AjouterRDV : public QDialog
{
    Q_OBJECT

public:
    explicit AjouterRDV(DatabaseManager *_db, QWidget *parent = 0);
    ~AjouterRDV();

private slots:
    QStringList getNamesFromQuery(const QString &query, const QString &value);

    void on_femer_clicked();

    void on_btnAjoutRDV_clicked();

signals:
    void clickAddRDV();

private:
    Ui::AjouterRDV *ui;
    DatabaseManager *dbManager;

    QCompleter *completer;
};

#endif // AJOUTERRDV_H
