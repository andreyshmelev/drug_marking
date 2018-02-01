#ifndef REGISTERPRODUCTWIDGET313_H
#define REGISTERPRODUCTWIDGET313_H

#include <QtCore>
#include <QWidget>
#include <medicament.h>
#include <manufacturer.h>
#include "sql.h"

namespace Ui {
class RegisterProductWidget313;
}

class RegisterProductWidget313 : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterProductWidget313(QWidget *parent = 0);
    ~RegisterProductWidget313();


    bool getRegistration() const;
    void setRegistration(bool value);

private:

    SQL  * sqlDB;
    void updateWidgetGui(QString gtinstring, QString batchstring, QString SNstring, QString tnvedstring, QString expstring);
    bool registration ;
    medicament * ScannedMedicament;
    QList<medicament *> MedicamentsList;
    bool CheckMedicamentinDB(medicament * m);

    const QString Start313ProcessQRString = "085987";
    const QString Stop313ProcessQRString  = "883940";

    QList<manufacturer *> manufacturesList;

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
    void RegistrationCompleted ( QList<medicament *> MedList, QDateTime operation_date);


private:
    Ui::RegisterProductWidget313 *ui;
    QDateTime getoperationDate();
};

#endif // REGISTERPRODUCTWIDGET313_H
