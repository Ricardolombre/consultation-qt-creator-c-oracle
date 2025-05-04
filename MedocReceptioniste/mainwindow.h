#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCompleter>
#include <QTableWidget>
#include "connexion/databasemanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    QStringList getNamesFromQuery(const QString &query);

    void setupTableWidget(const QString &queryString, QTableWidget* tableWidget);

    void ActualiserPatient();

    void ActualiserRDV();

private slots:
    void on_ajouterPatient_clicked();

    void on_ajouterRDV_clicked();

    void on_btnGererPayement_clicked();

    void on_btnGererRDV_clicked();

    void on_btnAdmin_clicked();

    void on_btnGererPatient_clicked();

    void on_L_rechercherPatient_textChanged(const QString &arg1);

    void on_L_rechercherRdv_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    DatabaseManager *dbManager;

    QCompleter *completer;
};

#endif // MAINWINDOW_H
