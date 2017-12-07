#include "unitpackwidget911.h"
#include "ui_unitpackwidget911.h"
#include "../../mainwindow.h"
#include <QDebug>

UnitPackWidget911::UnitPackWidget911(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UnitPackWidget911)
{
    ui->setupUi(this);

    connect(this, SIGNAL(RegistrationToggled()), this, SLOT(updateGUI())) ;
    registration = false;
    updateGUI();}

UnitPackWidget911::~UnitPackWidget911()
{
    delete ui;
}



bool UnitPackWidget911::getRegistration() const
{
    return registration;
}

void UnitPackWidget911::setRegistration(bool value)
{
    registration = value;
}

void UnitPackWidget911::updateWidgetGui(QString gtinstring, QString batchstring, QString SNstring, QString tnvedstring, QString expstring)
{
    ui->GTINTextAgregation->setText(gtinstring);
    ui->serialNumberAgregationValue->setText(SNstring);
    ui->batchnumberTextAgregation->setText(batchstring);
    ui->expirationdateAgregation->setText(expstring);
    ui->TNVEDValueAgregation->setText(tnvedstring);
}

bool UnitPackWidget911::CheckMedicamentinDB(medicament *m)
{
    return false;
}

void UnitPackWidget911::ToggleRegistration()
{
    if (registration != false)
        StopRegistrationProcess();
    else
        StartRegistrationProcess();
}

void UnitPackWidget911::StartRegistrationProcess()
{
    registration = true;
    emit setScannerLanguage(true);
    emit RegistrationStarted();
    emit RegistrationToggled();
}


manufacturer * UnitPackWidget911::getcompanysender()
{
    manufacturer *companysender = manufacturesList.at(ui->senderID->currentIndex());

    return companysender;
}


QDateTime UnitPackWidget911::getoperationDate()
{
    QDateTime operation_date = ui->operationDate->dateTime();

    return operation_date;
}


void UnitPackWidget911::StopRegistrationProcess()
{
    registration = false;

    manufacturer *companysender = getcompanysender();



    QDateTime operation_date = getoperationDate();

//    emit RegistrationCompleted(MedicamentsList,companysender,companyowner,ordertype,operation_date);
    emit RegistrationToggled();
    emit setScannerLanguage(false);

    MedicamentsList.clear();

    qDebug() << "RegistrationCompleted";
}

void UnitPackWidget911::GetMedicament(medicament *med)
{
    ScannedMedicament = med;
    updateWidgetGui(med->GTIN, med->SerialNumber, med->TNVED, med->ExperyDate, med->BatchNumber);

    qDebug() << "registration was"<< registration ;

    if (registration == true)
    {
        qDebug() <<"311 GetMedicament";
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

void UnitPackWidget911::updateGUI()
{
    int levelIndex = 1;
    int versionIndex = 0;
    bool bExtent = true;
    int maskIndex = -1;
    double scale_size = 2.2;

    QImage i;
    QPixmap scale_image ;

    if (registration != false) // if true
    {
        i = MainWindow::QRCodeToQImageConverter(Stop311ProcessQRString,2.2,  versionIndex, levelIndex, bExtent, maskIndex);
        ui->qrstartstop->setPixmap( QPixmap::fromImage( i ) );
        ui->RegistrationStartButton->setText("Закончить регистрацию");
        ui->GTINTextAgregation->setEnabled(true);
        ui->batchnumberTextAgregation->setEnabled(true);
        ui->expirationdateAgregation->setEnabled(true);
        ui->TNVEDValueAgregation->setEnabled(true);
        ui->serialNumberAgregationValue->setEnabled(true);
    }
    else
    {
        i = MainWindow::QRCodeToQImageConverter(Start311ProcessQRString,2.2,  versionIndex, levelIndex, bExtent, maskIndex);
        ui->qrstartstop->setPixmap( QPixmap::fromImage( i ) );
        ui->RegistrationStartButton->setText("Начать регистрацию");
        ui->GTINTextAgregation->setEnabled(false);
        ui->batchnumberTextAgregation->setEnabled(false);
        ui->expirationdateAgregation->setEnabled(false);
        ui->TNVEDValueAgregation->setEnabled(false);
        ui->serialNumberAgregationValue->setEnabled(false);
        ui->MedicamentsTable->clearContents();
        ui->MedicamentsTable->setRowCount(0);
    }

    double scale = 29 * scale_size;
    scale_image =  ui->qrstartstop->pixmap()->scaled( scale, scale );
    ui->qrstartstop->setPixmap( scale_image );
}

void UnitPackWidget911::AddMedicamentToTable(medicament *m)
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

void UnitPackWidget911::AddMedicamentToDB(medicament *m)
{

}

void UnitPackWidget911::on_RegistrationStartButton_clicked()
{
    ToggleRegistration();
}


void UnitPackWidget911::GetCompaniesDBList(QList<manufacturer*> man)
{
    QStringList a ;

    foreach (manufacturer * d , man) {
        manufacturesList.append(d);

        //        qDebug() << d->get_organisation_name() << "UnitPackWidget911";
        a.append(d->get_organisation_name());
    }

    ui->senderID->addItems(a);
}
