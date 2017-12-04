#include "moveorder415.h"
#include "ui_moveorder415.h"

MoveOrder415::MoveOrder415(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MoveOrder415)
{
    ui->setupUi(this);
}

MoveOrder415::~MoveOrder415()
{
    delete ui;
}

bool MoveOrder415::getRegistration() const
{
    return registration;
}

void MoveOrder415::setRegistration(bool value)
{
    registration = value;
}

void MoveOrder415::updateWidgetGui(QString gtinstring, QString batchstring, QString SNstring, QString tnvedstring, QString expstring)
{

}

bool MoveOrder415::CheckMedicamentinDB(medicament *m)
{

}

void MoveOrder415::ToggleRegistration()
{

}

void MoveOrder415::StartRegistrationProcess()
{

}

void MoveOrder415::StopRegistrationProcess()
{

}

void MoveOrder415::GetMedicament(medicament *m)
{

}

void MoveOrder415::updateGUI()
{

}

void MoveOrder415::AddMedicamentToTable(medicament *m)
{

}

void MoveOrder415::AddMedicamentToDB(medicament *m)
{

}
