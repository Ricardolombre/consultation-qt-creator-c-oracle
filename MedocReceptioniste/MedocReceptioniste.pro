#-------------------------------------------------
#
# Project created by QtCreator 2024-06-27T15:33:34
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MedocReceptioniste
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    connexion/databasemanager.cpp \
    patient/patient.cpp \
    ajouterpatient.cpp \
    ajouterrdv.cpp \
    RDV/rdv.cpp

HEADERS  += mainwindow.h \
    connexion/databasemanager.h \
    patient/patient.h \
    ajouterpatient.h \
    ajouterrdv.h \
    RDV/rdv.h

FORMS    += mainwindow.ui \
    ajouterpatient.ui \
    ajouterrdv.ui

RESOURCES += \
    icon.qrc
