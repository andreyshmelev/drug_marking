#include "registerendpackingwidget311.h"
#include "ui_registerendpackingwidget311.h"
#include "../../mainwindow.h"
#include <QDebug>

RegisterEndPackingWidget311::RegisterEndPackingWidget311(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegisterEndPackingWidget311)
{
    ui->setupUi(this);

    connect(this, SIGNAL(RegistrationToggled()), this, SLOT(updateGUI())) ;
    registration = false;
    updateGUI();
}

RegisterEndPackingWidget311::~RegisterEndPackingWidget311()
{
    delete ui;
}


bool RegisterEndPackingWidget311::getRegistration() const
{
    return registration;
}

void RegisterEndPackingWidget311::setRegistration(bool value)
{
    registration = value;
}

void RegisterEndPackingWidget311::updateWidgetGui(QString gtinstring, QString batchstring, QString SNstring, QString tnvedstring, QString expstring)
{
    ui->GTINTextAgregation->setText(gtinstring);
    ui->serialNumberAgregationValue->setText(SNstring);
    ui->batchnumberTextAgregation->setText(batchstring);
    ui->expirationdateAgregation->setText(expstring);
    ui->TNVEDValueAgregation->setText(tnvedstring);
}

bool RegisterEndPackingWidget311::CheckMedicamentinDB(medicament *m)
{
    return false;
}

void RegisterEndPackingWidget311::ToggleRegistration()
{
    if (registration != false)
        StopRegistrationProcess();
    else
        StartRegistrationProcess();
}

void RegisterEndPackingWidget311::StartRegistrationProcess()
{
    registration = true;
    emit setScannerLanguage(true);
    emit RegistrationStarted();
    emit RegistrationToggled();
}


manufacturer * RegisterEndPackingWidget311::getcompanysender()
{
    manufacturer *companysender = manufacturesList.at(ui->senderID->currentIndex());

    return companysender;
}

manufacturer * RegisterEndPackingWidget311::getcompanyowner()
{
    manufacturer *companyowne = manufacturesList.at(ui->ownerID->currentIndex());

    return companyowne;
}



QDateTime RegisterEndPackingWidget311::getoperationDate()
{
    QDateTime operation_date = ui->operationDate->dateTime();

    return operation_date;
}


int RegisterEndPackingWidget311::getOrderType()
{
    int ordertype ;


    switch (ui->OrderTypeCombo->currentIndex()) {
    case 0:
    {
        ordertype = 1;
        break;
    }
    case 1:
    {
        ordertype = 2;
        break;
    }

    default:
    {
        ordertype = -1;
        break;
    }
    }

    return ordertype;
}


void RegisterEndPackingWidget311::StopRegistrationProcess()
{
    registration = false;

    manufacturer *companysender = getcompanysender();
    manufacturer *companyowner = getcompanyowner();

    QDateTime operation_date = getoperationDate();
    int ordertype = getOrderType();

    emit RegistrationCompleted(MedicamentsList,companysender,companyowner,ordertype,operation_date);
    emit RegistrationToggled();
    emit setScannerLanguage(false);

    MedicamentsList.clear();
}

void RegisterEndPackingWidget311::GetMedicament(medicament *med)
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
                ui->errorLabel->setText("???????????????????? ?????? ??????????????????????????");
                return;
            }

            if ( (med->GTIN != listmed->GTIN ) )
            {
                ui->errorLabel->setText("???????????????? GTIN");
            }

            if ( (med->ExperyDate != listmed->ExperyDate ) )
            {
                ui->errorLabel->setText("???????????????? ????????");
            }

            if ( (med->BatchNumber != listmed->BatchNumber ) )
            {
                ui->errorLabel->setText("???????????????? ????????????");
            }

            if ( (med->TNVED != listmed->TNVED ) )
            {
                ui->errorLabel->setText("???????????????? ??????????");
            }

            if (( (med->GTIN != listmed->GTIN ) ) ||( (med->ExperyDate != listmed->ExperyDate ) )|| ( (med->BatchNumber != listmed->BatchNumber ) ) || ( (med->TNVED != listmed->TNVED ) ))
            {
                return;
            }
        }
        if (CheckMedicamentinDB(med))
        {
            ui->errorLabel->setText("???????????????????? ???????? ?? ????");
            return;
        }

        if (MedicamentsList.length()>=25000)
        {
            StopRegistrationProcess();
        }

        MedicamentsList.append(med);
        AddMedicamentToTable(med);
        emit AddMedicamentToDBTable(med, "process311");
        ui->errorLabel->clear();
        ui->countMedicamentValue->setText(QString::number(MedicamentsList.length()));
    }
}

void RegisterEndPackingWidget311::updateGUI()
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

void RegisterEndPackingWidget311::AddMedicamentToTable(medicament *m)
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


void RegisterEndPackingWidget311::on_RegistrationStartButton_clicked()
{
    ToggleRegistration();
}


void RegisterEndPackingWidget311::GetCompaniesDBList(QList<manufacturer*> man)
{
    QStringList a ;

    foreach (manufacturer * d , man) {
        manufacturesList.append(d);
        a.append(d->get_organisation_name());
    }

    ui->senderID->addItems(a);
    ui->ownerID->addItems(a);
}
