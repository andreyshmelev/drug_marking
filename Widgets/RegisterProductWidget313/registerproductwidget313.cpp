#include "registerproductwidget313.h"
#include "ui_registerproductwidget313.h"

#include "../../mainwindow.h"
#include <QDebug>

RegisterProductWidget313::RegisterProductWidget313(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegisterProductWidget313)
{
    ui->setupUi(this);

    connect(this, SIGNAL(RegistrationToggled()), this, SLOT(updateGUI())) ;
    registration = false;
    updateGUI();

}

RegisterProductWidget313::~RegisterProductWidget313()
{
    delete ui;
}

bool RegisterProductWidget313::getRegistration() const
{
    return registration;
}

void RegisterProductWidget313::setRegistration(bool value)
{
    registration = value;
}

void RegisterProductWidget313::updateWidgetGui(QString gtinstring, QString batchstring, QString SNstring, QString tnvedstring, QString expstring)
{
    ui->GTINTextAgregation->setText(gtinstring);
    ui->serialNumberAgregationValue->setText(SNstring);
    ui->batchnumberTextAgregation->setText(batchstring);
    ui->expirationdateAgregation->setText(expstring);
    ui->TNVEDValueAgregation->setText(tnvedstring);
}

bool RegisterProductWidget313::CheckMedicamentinDB(medicament *m)
{
    return false;
}

void RegisterProductWidget313::ToggleRegistration()
{
    if (registration != false)
        StopRegistrationProcess();
    else
        StartRegistrationProcess();
}

void RegisterProductWidget313::StartRegistrationProcess()
{
    registration = true;
    emit setScannerLanguage(true);
    emit RegistrationStarted();
    emit RegistrationToggled();
}

int RegisterProductWidget313::getOrderType()
{
    int ordertype ;
    return ordertype;
}

QDateTime RegisterProductWidget313::getoperationDate()
{
    QDateTime ISOdate;
    ISOdate = QDateTime::currentDateTime().toOffsetFromUtc(QDateTime::currentDateTime().offsetFromUtc());
    return ISOdate;
}

void RegisterProductWidget313::StopRegistrationProcess()
{
    registration = false;

    QDateTime operation_date = getoperationDate();

    emit RegistrationCompleted(MedicamentsList, operation_date);
    emit RegistrationToggled();
    emit setScannerLanguage(false);

    MedicamentsList.clear();

}

void RegisterProductWidget313::GetMedicament(medicament *med)
{
    ScannedMedicament = med;
    updateWidgetGui(med->GTIN, med->BatchNumber, med->SerialNumber, med->TNVED, med->ExperyDate);

    if (registration == true)
    {
        // проверяем если пачка с таким же номером партии и серийником была просканирована недавно
        foreach ( medicament * listmed , MedicamentsList)
        {
            if ( (med->SerialNumber == listmed->SerialNumber)&&(med->BatchNumber == listmed->BatchNumber) )
            {
                ui->errorLabel->setText("Медикамент уже просканирован");
                return;
            }

            if ( (med->GTIN != listmed->GTIN ) )
            {
                ui->errorLabel->setText("неверный GTIN");
            }

            if ( (med->ExperyDate != listmed->ExperyDate ) )
            {
                ui->errorLabel->setText("неверная Дата");
            }

            if ( (med->BatchNumber != listmed->BatchNumber ) )
            {
                ui->errorLabel->setText("неверная партия");
            }

            if ( (med->TNVED != listmed->TNVED ) )
            {
                ui->errorLabel->setText("неверная ТНВЭД");
            }

            if (( (med->GTIN != listmed->GTIN ) ) ||( (med->ExperyDate != listmed->ExperyDate ) )|| ( (med->BatchNumber != listmed->BatchNumber ) ) || ( (med->TNVED != listmed->TNVED ) ))
            {
                return;
            }
        }
        if (CheckMedicamentinDB(med))
        {
            ui->errorLabel->setText("Медикамент есть в БД");
            return;
        }

        if (MedicamentsList.length()>=25000)
        {
            StopRegistrationProcess();
        }

        MedicamentsList.append(med);
        AddMedicamentToTable(med);
        emit AddMedicamentToDBTable(med, "process313");
        ui->errorLabel->clear();
        ui->countMedicamentValue->setText(QString::number(MedicamentsList.length()));
    }
}

void RegisterProductWidget313::updateGUI()
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
        i = MainWindow::QRCodeToQImageConverter(Stop313ProcessQRString,2.2,  versionIndex, levelIndex, bExtent, maskIndex);
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
        i = MainWindow::QRCodeToQImageConverter(Start313ProcessQRString,2.2,  versionIndex, levelIndex, bExtent, maskIndex);
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

void RegisterProductWidget313::AddMedicamentToTable(medicament *m)
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

void RegisterProductWidget313::on_RegistrationStartButton_clicked()
{
    ToggleRegistration();
}


void RegisterProductWidget313::GetCompaniesDBList(QList<manufacturer*> man)
{
    QStringList a ;

    foreach (manufacturer * d , man) {
        manufacturesList.append(d);
        a.append(d->get_organisation_name());
    }
}
