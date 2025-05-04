#-------------------------------------------------
#
# Project created by QtCreator 2024-07-08T20:10:15
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MedocPatient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    connexion/databasemanager.cpp \
    authentification.cpp \
    ajouterrdv.cpp \
    RDV/rdv.cpp

HEADERS  += mainwindow.h \
    connexion/databasemanager.h \
    authentification.h \
    ajouterrdv.h \
    RDV/rdv.h

FORMS    += mainwindow.ui \
    authentification.ui \
    ajouterrdv.ui

RESOURCES += \
    icon.qrc
