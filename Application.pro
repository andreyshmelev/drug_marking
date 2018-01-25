#-------------------------------------------------
#
# Project created by QtCreator 2017-11-13T11:36:52
#
#-------------------------------------------------

QT += core gui
QT += xml
QT += sql
QT += network
QT += printsupport

# CONFIG += c++11

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
    sql.cpp \
    basebuisnessclass.cpp \
    Widgets/MoveOrderWidget415/moveorder415.cpp \
    Widgets/RelabelingWidget811/relabelingwidget811.cpp \
    Widgets/UnitPackWidget911/unitpackwidget911.cpp \
    Widgets/RegisterEndPackingWidget311/registerendpackingwidget311.cpp \
    code128.cpp \
    code128item.cpp \
    serializationline.cpp

HEADERS += \
        mainwindow.h \
    QRCodeGenerator.h \
    manufacturer.h \
    medicament.h \
    basetypes.h \
    Widgets/UnitExtractWidget/unitextractwidget.h \
    sql.h \
    basebuisnessclass.h \
    Widgets/MoveOrderWidget415/moveorder415.h \
    Widgets/RelabelingWidget811/relabelingwidget811.h \
    Widgets/UnitPackWidget911/unitpackwidget911.h \
    Widgets/RegisterEndPackingWidget311/registerendpackingwidget311.h \
    code128.h \
    code128item.h \
    serializationline.h

FORMS += \
        mainwindow.ui \
    Widgets/UnitExtractWidget/unitextractwidget.ui \
    Widgets/MoveOrderWidget415/moveorder415.ui \
    Widgets/RelabelingWidget811/relabelingwidget811.ui \
    Widgets/UnitPackWidget911/unitpackwidget911.ui \
    Widgets/RegisterEndPackingWidget311/registerendpackingwidget311.ui
