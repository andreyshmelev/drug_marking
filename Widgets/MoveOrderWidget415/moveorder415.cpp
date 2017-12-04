#include "moveorder415.h"
#include "ui_moveorder415.h"
#include <QDebug>

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
    ui->GTINTextAgregation->setText(gtinstring);
    ui->serialNumberAgregationValue->setText(SNstring);
    ui->batchnumberTextAgregation->setText(batchstring);
    ui->expirationdateAgregation->setText(expstring);
    ui->TNVEDValueAgregation->setText(tnvedstring);
}

bool MoveOrder415::CheckMedicamentinDB(medicament *m)
{
    return false;
}

void MoveOrder415::ToggleRegistration()
{
    if (registration != false)
        StopRegistrationProcess();
    else
        StartRegistrationProcess();
}

void MoveOrder415::StartRegistrationProcess()
{

}

void MoveOrder415::StopRegistrationProcess()
{

}

void MoveOrder415::GetMedicament(medicament *med)
{
    ScannedMedicament = med;
    updateWidgetGui(med->GTIN, med->SerialNumber, med->TNVED, med->ExperyDate, med->BatchNumber);

    qDebug() << "registration was"<< registration ;

    if (registration == true)
    {
        qDebug() <<"UnitExtractWidget GetMedicament";
        // проверяем если пачка с таким же номером партии и серийником была просканирована недавно
        foreach ( medicament * listmed , MedicamentsList)
        {
            if ( (med->SerialNumber == listmed->SerialNumber)&&(med->BatchNumber == listmed->BatchNumber) )
            {
                qDebug() << "такой медикамент уже есть";
                return;
            }

            if ( (med->GTIN != listmed->GTIN ) )
            {
                qDebug() << "неверный GTIN, препарат должен иметь GTIN = " + MedicamentsList.at(0)->GTIN;
            }

            if ( (med->ExperyDate != listmed->ExperyDate ) )
            {
                qDebug() << "неверная дата годности, верная -  " + MedicamentsList.at(0)->ExperyDate;

            }

            if ( (med->BatchNumber != listmed->BatchNumber ) )
            {
                qDebug() << "неверная партия, верная -  " + MedicamentsList.at(0)->BatchNumber;
            }

            if ( (med->TNVED != listmed->TNVED ) )
            {
                qDebug() << "неверная TNVED, верная -  " + MedicamentsList.at(0)->TNVED;
            }

            if (( (med->GTIN != listmed->GTIN ) ) ||( (med->ExperyDate != listmed->ExperyDate ) )|| ( (med->BatchNumber != listmed->BatchNumber ) ) || ( (med->TNVED != listmed->TNVED ) ))
            {
                return;
            }
        }
        if (CheckMedicamentinDB(med))
        {
            qDebug() << "такой медикамент уже есть в базе данных";
            return;
        }

        MedicamentsList.append(med);
        AddMedicamentToTable(med);
        AddMedicamentToDB(med);
    }
}

void MoveOrder415::updateGUI()
{

}

void MoveOrder415::AddMedicamentToTable(medicament *m)
{
    ui->MedicamentsTable->insertRow(0);
    ui->MedicamentsTable->setItem(0, 0, new QTableWidgetItem(m->medicament_name));
    ui->MedicamentsTable->setItem(0, 1, new QTableWidgetItem(m->GTIN));
    ui->MedicamentsTable->setItem(0, 2, new QTableWidgetItem(m->BatchNumber));
    ui->MedicamentsTable->setItem(0, 3, new QTableWidgetItem(m->SerialNumber));
    ui->MedicamentsTable->setItem(0, 4, new QTableWidgetItem(m->TNVED));
    ui->MedicamentsTable->setItem(0, 5, new QTableWidgetItem(m->ExperyDate));
    ui->MedicamentsTable->scrollToTop();
}

void MoveOrder415::AddMedicamentToDB(medicament *m)
{

}

void MoveOrder415::on_RegistrationStartButton_clicked()
{
    ToggleRegistration();
}


void MoveOrder415::GetCompaniesDBList(QList<manufacturer*> man)
{
    QStringList a ;

    foreach (manufacturer * d , man) {
        manufacturesList.append(d);

//        qDebug() << d->get_organisation_name() << "MoveOrder415";
        a.append(d->get_organisation_name());
    }

    ui->senderCombobox->addItems(a);
    ui->recieverCombobox->addItems(a);


}
