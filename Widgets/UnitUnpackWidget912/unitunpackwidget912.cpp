#include "unitunpackwidget912.h"
#include "ui_unitunpackwidget912.h"
#include <QDebug>

UnitUnpackWidget912::UnitUnpackWidget912(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UnitUnpackWidget912)
{
    ui->setupUi(this);

    connect(this, SIGNAL(RegistrationToggled()), this, SLOT(updateGUI())) ;
    registration = false;
    updateGUI();
}

UnitUnpackWidget912::~UnitUnpackWidget912()
{
    delete ui;
}

void UnitUnpackWidget912::GetPack(pack *p)
{
    qDebug() << "i got pack " << p->getSSCC() ;
    if (registration == true)
    {
        ui->SSCCValue->setText(p->getSSCC());
    }
}

void UnitUnpackWidget912::ToggleRegistration()
{
    if (registration != false)
        StopRegistrationProcess();
    else
        StartRegistrationProcess();
}

void UnitUnpackWidget912::StartRegistrationProcess()
{
    registration = true;
    emit RegistrationToggled();
}

void UnitUnpackWidget912::StopRegistrationProcess()
{
    registration = false;
    emit RegistrationToggled();
}

void UnitUnpackWidget912::updateGUI()
{
    if (registration == true) // if true
    {
        ui->RegistrationStartButton->setText("Закончить регистрацию");
    }
    else
    {
        ui->RegistrationStartButton->setText("Начать регистрацию");
    }
}

manufacturer * UnitUnpackWidget912::getcompanysender()
{
    manufacturer *companysender = manufacturesList.at(ui->senderID->currentIndex());
    return companysender;
}

void UnitUnpackWidget912::GetCompaniesDBList(QList<manufacturer*> man)
{
    QStringList a ;

    manufacturesList.clear();
    foreach (manufacturer * d , man) {
        manufacturesList.append(d);
        a.append(d->get_organisation_name());
    }

    ui->senderID->addItems(a);
}

void UnitUnpackWidget912::on_RegistrationStartButton_clicked()
{
    ToggleRegistration();
}
