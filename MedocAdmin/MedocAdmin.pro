#-------------------------------------------------
#
# Project created by QtCreator 2024-06-26T12:44:20
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MedocAdmin
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    connexion/databasemanager.cpp \
    patient/patient.cpp \
    ajouterpatient.cpp \
    ajoutermedecin.cpp \
    medecin/medecin.cpp \
    specialite/specialite.cpp \
    medicament/medicament.cpp \
    posologie/posologie.cpp \
    bilan/bilan.cpp

HEADERS  += mainwindow.h \
    connexion/databasemanager.h \
    patient/patient.h \
    ajouterpatient.h \
    ajoutermedecin.h \
    medecin/medecin.h \
    specialite/specialite.h \
    medicament/medicament.h \
    posologie/posologie.h \
    bilan/bilan.h

FORMS    += mainwindow.ui \
    ajouterpatient.ui \
    ajoutermedecin.ui

RESOURCES += \
    icon.qrc
