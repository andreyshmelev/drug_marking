#ifndef BASEBUISNESSCLASS_H
#define BASEBUISNESSCLASS_H

#include <QObject>

class BaseBuisnessClass : public QObject
{
    Q_OBJECT
public:
    explicit BaseBuisnessClass(QObject *parent = nullptr);


//    const QString Start312ProcessQRString = "909628";
//    const QString Stop312ProcessQRString  = "272080";

//private:

//    Ui::UnitExtractWidget *ui;
//    void updateWidgetGui(QString gtinstring, QString batchstring, QString SNstring, QString tnvedstring, QString expstring);
//    bool registration ;
//    medicament * ScannedMedicament;
//    QList<medicament *> MedicamentsList;
//    bool CheckMedicamentinDB(medicament * m);

//public slots:

//    void ToggleRegistration();
//    void StartRegistrationProcess();
//    void StopRegistrationProcess();

//private slots:

//    void GetMedicament (medicament * m);
//    void on_RegistrationStartButton_clicked();
//    void updateGUI();

//    void AddMedicamentToTable(medicament * m);
//    void AddMedicamentToDB(medicament * m);

signals:

    void RegistrationStarted();
    void StopRegisterControlSamples();
    void RegistrationToggled();
//    void RegistrationCompleted(QList<medicament *> MedList , uint8_t controlsamplestype);
};

#endif // BASEBUISNESSCLASS_H
