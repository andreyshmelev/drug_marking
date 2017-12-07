#ifndef UNITPACKWIDGET911_H
#define UNITPACKWIDGET911_H

#include <QtCore>
#include <QWidget>
#include <medicament.h>
#include <manufacturer.h>


namespace Ui {
class UnitPackWidget911;
}

class UnitPackWidget911 : public QWidget
{
    Q_OBJECT

public:
    explicit UnitPackWidget911(QWidget *parent = 0);
    ~UnitPackWidget911();

    bool getRegistration() const;
    void setRegistration(bool value);

private:

    void updateWidgetGui(QString gtinstring, QString batchstring, QString SNstring, QString tnvedstring, QString expstring);
    bool registration ;
    medicament * ScannedMedicament;
    QList<medicament *> MedicamentsList;
    bool CheckMedicamentinDB(medicament * m);

    const QString Start311ProcessQRString = "501301";
    const QString Stop311ProcessQRString  = "641195";

    QList<manufacturer *> manufacturesList;



    manufacturer *getcompanysender();
    QDateTime getoperationDate();


public slots:

    void ToggleRegistration();
    void StartRegistrationProcess();
    void StopRegistrationProcess();
    void GetCompaniesDBList(QList<manufacturer *>);
    void GetMedicament (medicament * m);

private slots:

    void updateGUI();

    void AddMedicamentToTable(medicament * m);
    void AddMedicamentToDB(medicament * m);

    void on_RegistrationStartButton_clicked();

signals:

    void setScannerLanguage(bool);
    void RegistrationStarted();
    void StopRegisterControlSamples();
    void RegistrationToggled();
    void RegistrationCompleted (QList<medicament *> MedList, manufacturer * sender, manufacturer * owner,  int ordertype, QDateTime operation_date);


private:
    Ui::UnitPackWidget911 *ui;
};

#endif // UNITPACKWIDGET911_H
