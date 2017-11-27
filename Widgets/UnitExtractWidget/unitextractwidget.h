#ifndef UNITEXTRACTWIDGET_H
#define UNITEXTRACTWIDGET_H

#include <QWidget>
#include <medicament.h>

namespace Ui {
class UnitExtractWidget;
}

class UnitExtractWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UnitExtractWidget(QWidget *parent = 0);
    ~UnitExtractWidget();

    const QString Start312ProcessQRString = "909628";
    const QString Stop312ProcessQRString  = "272080";

private:

    Ui::UnitExtractWidget *ui;
    void updateWidgetGui(QString gtinstring, QString batchstring, QString SNstring, QString tnvedstring, QString expstring);
    bool registration ;
     medicament * ScannedMedicament;

public slots:

    void GetParsedString (QString  gtinstring, QString SNstring, QString tnvedstring, QString expstring, QString batchstring, QString sGTINString);
    void GetMedicament (medicament * m);
    void ToggleRegistration();
    void StartRegistrationProcess();
    void StopRegistrationProcess();

private slots:

    void on_RegistrationStartButton_clicked();
    void updateGUI();

    void updateTable();

signals:

    void StartRegisterControlSamples();
    void StopRegisterControlSamples();
    void RegistrationToggled();
};

#endif // UNITEXTRACTWIDGET_H
