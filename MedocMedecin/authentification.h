#ifndef AUTHENTIFICATION_H
#define AUTHENTIFICATION_H

#include <QDialog>

namespace Ui {
class Authentification;
}

class Authentification : public QDialog
{
    Q_OBJECT

public:
    explicit Authentification(QWidget *parent = 0);
    ~Authentification();

private slots:
    void on_btnconnexion_clicked();

private:
    Ui::Authentification *ui;
};

#endif // AUTHENTIFICATION_H
