#ifndef REGISTERENDPACKINGWIDGET311_H
#define REGISTERENDPACKINGWIDGET311_H

#include <QtCore>
#include <QWidget>
#include <medicament.h>
#include <manufacturer.h>
#include "sql.h"

namespace Ui {
class RegisterEndPackingWidget311;
}

class RegisterEndPackingWidget311 : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterEndPackingWidget311(QWidget *parent = 0);
    ~RegisterEndPackingWidget311();

    bool getRegistration() const;
    void setRegistration(bool value);

private:

    SQL  * sqlDB;
    void updateWidgetGui(QString gtinstring, QString batchstring, QString SNstring, QString tnvedstring, QString expstring);
    bool registration ;
    medicament * ScannedMedicament;
    QList<medicament *> MedicamentsList;
    bool CheckMedicamentinDB(medicament * m);

    const QString Start311ProcessQRString = "501301";
    const QString Stop311ProcessQRString  = "641195";

    QList<manufacturer *> manufacturesList;

    manufacturer *getcompanysender();
    manufacturer *getcompanyowner();
    QDateTime getoperationDate();
    int getOrderType();


public slots:

    void ToggleRegistration();
    void StartRegistrationProcess();
    void StopRegistrationProcess();
    void GetCompaniesDBList(QList<manufacturer *>);
    void GetMedicament (medicament * m);

private slots:

    void updateGUI();
    void AddMedicamentToTable(medicament * m);
    void on_RegistrationStartButton_clicked();

signals:


    void AddMedicamentToDBTable(medicament * m, QString tablename); // tablename - например process311
    void setScannerLanguage(bool);
    void RegistrationStarted();
    void StopRegisterControlSamples();
    void RegistrationToggled();
    void RegistrationCompleted (QList<medicament *> MedList, manufacturer * sender, manufacturer * owner,  int ordertype, QDateTime operation_date);

private:
    Ui::RegisterEndPackingWidget311 *ui;
};
#endif // REGISTERENDPACKINGWIDGET311_H
