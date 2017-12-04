#ifndef MOVEORDER415_H
#define MOVEORDER415_H

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

public slots:

    void ToggleRegistration();
    void StartRegistrationProcess();
    void StopRegistrationProcess();

private slots:

    void GetMedicament (medicament * m);
    void updateGUI();

    void AddMedicamentToTable(medicament * m);
    void AddMedicamentToDB(medicament * m);

    void on_RegistrationStartButton_clicked();

signals:

    void RegistrationStarted();
    void StopRegisterControlSamples();
    void RegistrationToggled();
    void RegistrationCompleted(QList<medicament *> MedList , uint8_t controlsamplestype);
};

#endif // MOVEORDER415_H
