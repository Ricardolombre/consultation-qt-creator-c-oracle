#-------------------------------------------------
#
# Project created by QtCreator 2024-07-10T21:20:32
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MedocMedecin
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    connexion/databasemanager.cpp \
    consultation/consultation.cpp \
    ajouterordonnance.cpp \
    Medicament_recommande/medicament_recommande.cpp \
    ajouterbilan.cpp \
    Bilan_recommande/bilan_recommande.cpp \
    ajouterrdv.cpp \
    RDV/rdv.cpp \
    authentification.cpp

HEADERS  += mainwindow.h \
    connexion/databasemanager.h \
    consultation/consultation.h \
    ajouterordonnance.h \
    Medicament_recommande/medicament_recommande.h \
    ajouterbilan.h \
    Bilan_recommande/bilan_recommande.h \
    ajouterrdv.h \
    RDV/rdv.h \
    authentification.h

FORMS    += mainwindow.ui \
    ajouterordonnance.ui \
    ajouterbilan.ui \
    ajouterrdv.ui \
    authentification.ui

RESOURCES += \
    icon.qrc
