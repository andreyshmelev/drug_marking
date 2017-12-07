#ifndef MOVEORDER415_H
#define MOVEORDER415_H

#include <QtCore>
#include <QWidget>
#include <medicament.h>
#include <manufacturer.h>

namespace Ui {
class MoveOrder415;
}

class MoveOrder415 : public QWidget
{
    Q_OBJECT

public:
    explicit MoveOrder415(QWidget *parent = 0);
    ~MoveOrder415();

    bool getRegistration() const;
    void setRegistration(bool value);

private:
    Ui::MoveOrder415 *ui;


private:

    void updateWidgetGui(QString gtinstring, QString batchstring, QString SNstring, QString tnvedstring, QString expstring);
    bool registration ;
    medicament * ScannedMedicament;
    QList<medicament *> MedicamentsList;
    bool CheckMedicamentinDB(medicament * m);

    const QString Start415ProcessQRString = "963274";
    const QString Stop415ProcessQRString  = "900865";

    QList<manufacturer *> manufacturesList;

    manufacturer * getcompanyreciver();

    manufacturer *getcompanysender();
    QDateTime getoperationDate();

    QString getDocNum();

    QDate getDocDate();
    int getTurnoverType();

    int getSourceType();

    int getContractType();

    QString getPrice();

    QString getVat();

public slots:

    void ToggleRegistration();
    void StartRegistrationProcess();
    void StopRegistrationProcess();
    void GetCompaniesDBList(QList<manufacturer *>);

private slots:

    void GetMedicament (medicament * m);
    void updateGUI();

    void AddMedicamentToTable(medicament * m);
    void AddMedicamentToDB(medicament * m);

    void on_RegistrationStartButton_clicked();

signals:

    void setScannerLanguage(bool);
    void RegistrationStarted();
    void StopRegisterControlSamples();
    void RegistrationToggled();
    void RegistrationCompleted (QList<medicament *> MedList, manufacturer * companyreciver, manufacturer * companytranciver, QDateTime operation_date, QString DocNum, QDate doc_date, int turnovertype, int source, int contracttype, QString Price, QString Vat);
};

#endif // MOVEORDER415_H
