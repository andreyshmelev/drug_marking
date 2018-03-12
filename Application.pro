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
    serializationline.cpp \
    Widgets/RegisterProductWidget313/registerproductwidget313.cpp \
    apimark.cpp \
    Widgets/UnitUnpackWidget912/unitunpackwidget912.cpp \
    Widgets/UnitExtractWidget913/unitextractwidget913.cpp \
    Widgets/UnitAppendWidget914/unitappendwidget914.cpp \
    Widgets/MultiPackWidget915/multipackwidget915.cpp \
    Widgets/RefusalRecieverWidget252/refusalrecieverwidget252.cpp \
    Widgets/RecallWidget250/recallwidget250.cpp \
    Widgets/RefusalSenderWidget251/refusalsenderwidget251.cpp \
    Widgets/MovePlaceWidget431/moveplacewidget431.cpp \
    Widgets/MoveDestructionWidget541/movedestructionwidget541.cpp \
    Widgets/DestructionWidget542/destructionwidget542.cpp \
    Widgets/WithdrawalWidget552/withdrawalwidget552.cpp \
    pack.cpp \
    Widgets/XMLViewerWidget/xmlviewerwidget.cpp \
    ccomboboxpopupitemdelegate.cpp

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
    serializationline.h \
    Widgets/RegisterProductWidget313/registerproductwidget313.h \
    apimark.h \
    Widgets/UnitUnpackWidget912/unitunpackwidget912.h \
    Widgets/UnitExtractWidget913/unitextractwidget913.h \
    Widgets/UnitAppendWidget914/unitappendwidget914.h \
    Widgets/MultiPackWidget915/multipackwidget915.h \
    Widgets/RefusalRecieverWidget252/refusalrecieverwidget252.h \
    Widgets/RecallWidget250/recallwidget250.h \
    Widgets/RefusalSenderWidget251/refusalsenderwidget251.h \
    Widgets/MovePlaceWidget431/moveplacewidget431.h \
    Widgets/MoveDestructionWidget541/movedestructionwidget541.h \
    Widgets/DestructionWidget542/destructionwidget542.h \
    Widgets/WithdrawalWidget552/withdrawalwidget552.h \
    pack.h \
    xmlviewerwidget.h \
    Widgets/XMLViewerWidget/xmlviewerwidget.h \
    ccomboboxpopupitemdelegate.h

FORMS += \
        mainwindow.ui \
    Widgets/UnitExtractWidget/unitextractwidget.ui \
    Widgets/MoveOrderWidget415/moveorder415.ui \
    Widgets/RelabelingWidget811/relabelingwidget811.ui \
    Widgets/UnitPackWidget911/unitpackwidget911.ui \
    Widgets/RegisterEndPackingWidget311/registerendpackingwidget311.ui \
    Widgets/RegisterProductWidget313/registerproductwidget313.ui \
    Widgets/UnitUnpackWidget912/unitunpackwidget912.ui \
    Widgets/UnitExtractWidget913/unitextractwidget913.ui \
    Widgets/UnitAppendWidget914/unitappendwidget914.ui \
    Widgets/MultiPackWidget915/multipackwidget915.ui \
    Widgets/RefusalRecieverWidget252/refusalrecieverwidget252.ui \
    Widgets/RecallWidget250/recallwidget250.ui \
    Widgets/RefusalSenderWidget251/refusalsenderwidget251.ui \
    Widgets/MovePlaceWidget431/moveplacewidget431.ui \
    Widgets/MoveDestructionWidget541/movedestructionwidget541.ui \
    Widgets/DestructionWidget542/destructionwidget542.ui \
    Widgets/WithdrawalWidget552/withdrawalwidget552.ui \
    Widgets/XMLViewerWidget/xmlviewerwidget.ui
