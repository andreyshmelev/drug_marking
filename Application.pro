#-------------------------------------------------
#
# Project created by QtCreator 2017-11-13T11:36:52
#
#-------------------------------------------------

QT       += core gui
QT       += xml
QT += sql
QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Application
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    QRCodeGenerator.cpp \
    manufacturer.cpp \
    medicament.cpp \
    Widgets/UnitExtractWidget/unitextractwidget.cpp \
    Widgets/UnitAppendWidget/unitappendwidget.cpp \
    sql.cpp \
    basebuisnessclass.cpp

HEADERS += \
        mainwindow.h \
    QRCodeGenerator.h \
    manufacturer.h \
    medicament.h \
    basetypes.h \
    Widgets/UnitExtractWidget/unitextractwidget.h \
    Widgets/UnitAppendWidget/unitappendwidget.h \
    sql.h \
    basebuisnessclass.h

FORMS += \
        mainwindow.ui \
    Widgets/UnitExtractWidget/unitextractwidget.ui \
    Widgets/UnitAppendWidget/unitappendwidget.ui
