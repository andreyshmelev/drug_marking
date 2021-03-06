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
    updateGUI();
}

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
    emit RegistrationCompleted(MedicamentsList,companysender,operation_date);
    emit RegistrationToggled();
    emit setScannerLanguage(false);
    MedicamentsList.clear();
}

void UnitPackWidget911::GetMedicament(medicament *med)
{
    ScannedMedicament = med;
    updateWidgetGui(med->GTIN, med->BatchNumber, med->SerialNumber, med->TNVED, med->ExperyDate);

    if (registration == true)
    {
        // ?????????????????? ???????? ?????????? ?? ?????????? ???? ?????????????? ???????????? ?? ???????????????????? ???????? ???????????????????????????? ??????????????
        foreach ( medicament * listmed , MedicamentsList)
        {
            if ( (med->SerialNumber == listmed->SerialNumber)&&(med->BatchNumber == listmed->BatchNumber) )
            {
                qDebug() << "?????????? ???????????????????? ?????? ????????";
                ui->errorLabel->setText("???????????????????? ?????? ??????????????????????????");
                return;
            }

            if ( (med->GTIN != listmed->GTIN ) )
            {
                qDebug() << "???????????????? GTIN, ???????????????? ???????????? ?????????? GTIN = " + MedicamentsList.at(0)->GTIN;
                ui->errorLabel->setText("???????????????? GTIN");
            }

            if ( (med->ExperyDate != listmed->ExperyDate ) )
            {
                qDebug() << "???????????????? ???????? ????????????????, ???????????? -  " + MedicamentsList.at(0)->ExperyDate;
                ui->errorLabel->setText("???????????????? ????????");

            }

            if ( (med->BatchNumber != listmed->BatchNumber ) )
            {
                qDebug() << "???????????????? ????????????, ???????????? -  " + MedicamentsList.at(0)->BatchNumber;
                ui->errorLabel->setText("???????????????? ????????????");
            }

            if ( (med->TNVED != listmed->TNVED ) )
            {
                qDebug() << "???????????????? TNVED, ???????????? -  " + MedicamentsList.at(0)->TNVED;
                ui->errorLabel->setText("???????????????? ??????????");
            }

            if (( (med->GTIN != listmed->GTIN ) ) ||( (med->ExperyDate != listmed->ExperyDate ) )|| ( (med->BatchNumber != listmed->BatchNumber ) ) || ( (med->TNVED != listmed->TNVED ) ))
            {
                return;
            }
        }
        if (CheckMedicamentinDB(med))
        {
            qDebug() << "?????????? ???????????????????? ?????? ???????? ?? ???????? ????????????";
            ui->errorLabel->setText("???????????????????? ???????? ?? ????");
            return;
        }

        if (MedicamentsList.length()>=25000)
        {
            StopRegistrationProcess();
        }

        MedicamentsList.append(med);
        AddMedicamentToTable(med);

        emit AddMedicamentToDBTable(med, "process911");
        ui->errorLabel->clear();
        ui->countMedicamentValue->setText(QString::number(MedicamentsList.length()));
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

    if (registration == true) // if true
    {
        i = MainWindow::QRCodeToQImageConverter(Stop311ProcessQRString,2.2,  versionIndex, levelIndex, bExtent, maskIndex);
        ui->qrstartstop->setPixmap( QPixmap::fromImage( i ) );
        ui->RegistrationStartButton->setText("?????????????????? ??????????????????????");
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
        ui->RegistrationStartButton->setText("???????????? ??????????????????????");
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

//void UnitPackWidget911::AddMedicamentToDB(medicament *m)
//{
//    sqlDB = new SQL("???????????????? ????????????");
//    sqlDB->makesqlreq(QString("insert into process911 values (%1,%2,%3,%4,%5)").arg(m->GTIN,QDateTime::currentDateTime().toTimeSpec(Qt::LocalTime).toString("dd-MM-yyyy"),QDateTime::currentDateTime().toTimeSpec(Qt::LocalTime).toString("hh-mm-ss"),"XML","SSCC"));
//    //    drugs = sqlDB->sel("drugs_name", "process311", "","drugs_name");
//}

void UnitPackWidget911::on_RegistrationStartButton_clicked()
{
    ToggleRegistration();
}

void UnitPackWidget911::GetCompaniesDBList(QList<manufacturer*> man)
{
    QStringList a ;

    foreach (manufacturer * d , man) {
        manufacturesList.append(d);
        a.append(d->get_organisation_name());
    }
    ui->senderID->addItems(a);
}
