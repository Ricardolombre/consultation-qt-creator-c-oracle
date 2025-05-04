#include "mainwindow.h"
#include <QApplication>
#include "authentification.h"
#include <QSplashScreen>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap(":/ICON/icon.png"));
    splash->show();

    Authentification w;

    QTimer::singleShot(2000, splash, SLOT(close()));
    QTimer::singleShot(2000, &w, SLOT(show()));
//    w.show();

    return a.exec();
}
