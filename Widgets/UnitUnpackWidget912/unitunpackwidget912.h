#ifndef UNITUNPACKWIDGET912_H
#define UNITUNPACKWIDGET912_H
#include <pack.h>
#include <manufacturer.h>
#include <QWidget>
#include <QDateTime>

namespace Ui {
class UnitUnpackWidget912;
}

class UnitUnpackWidget912 : public QWidget
{
    Q_OBJECT

public:
    explicit UnitUnpackWidget912(QWidget *parent = 0);
    ~UnitUnpackWidget912();


public slots:

    void GetPack (pack * p);
    void ToggleRegistration();
    void StartRegistrationProcess();
    void StopRegistrationProcess();
    void updateGUI();

    void GetCompaniesDBList(QList<manufacturer *> man);
    manufacturer *getcompanysender();

signals:

    void RegistrationToggled();

    void RegistrationCompleted (pack * p, manufacturer *companysender, QDateTime operation_date);

private slots:

    void on_RegistrationStartButton_clicked();

private:

    QList<manufacturer *> manufacturesList;

    bool registration ;

private:

    Ui::UnitUnpackWidget912 *ui;
};

#endif // UNITUNPACKWIDGET912_H
