#include "moveorder415.h"
#include "ui_moveorder415.h"
#include "../../mainwindow.h"
#include <QDebug>

MoveOrder415::MoveOrder415(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MoveOrder415)
{
    ui->setupUi(this);

    connect(this, SIGNAL(RegistrationToggled()), this, SLOT(updateGUI())) ;
    registration = false;
    updateGUI();

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
    registration = true;
    emit RegistrationStarted();
    emit RegistrationToggled();
}

manufacturer * MoveOrder415::getcompanyreciver()
{
    manufacturer *companyreciver = manufacturesList.at(ui->recieverCombobox->currentIndex());

    return companyreciver;
}

manufacturer * MoveOrder415::getcompanysender()
{
    manufacturer *companysender = manufacturesList.at(ui->senderCombobox->currentIndex());

    return companysender;
}

QDate MoveOrder415::getoperationDate()
{
    QDate operation_date = QDate::currentDate();

    return operation_date;
}

QDate MoveOrder415::getDocDate()
{
    QDate Docdate = QDate::currentDate();

    return Docdate;
}

QString MoveOrder415::getDocNum()
{
    QString DocNum = ui->documentnumber->toPlainText();

    return DocNum;
}

int MoveOrder415::getTurnoverType()
{
    int turnovertype;

    switch (ui->turnovertypecombo->currentIndex()) {
    case 0:
    {
        turnovertype = 1;
        break;
    }
    case 1:
    {
        turnovertype = 2;
        break;
    }

    default:
    {
        turnovertype = -1;
        break;
    }
    }

    return turnovertype;
}

int MoveOrder415::getSourceType()
{
    int sourcetype ;


    switch (ui->sourcecombo->currentIndex()) {
    case 0:
    {
        sourcetype = 1;
        break;
    }
    case 1:
    {
        sourcetype = 2;
        break;
    }

    default:
    {
        sourcetype = -1;
        break;
    }
    }

    return sourcetype;
}

int MoveOrder415::getContractType()
{
    int contracttype ;


    switch (ui->contracttype->currentIndex()) {
    case 0:
    {
        contracttype = 1;
        break;
    }
    case 1:
    {
        contracttype = 2;
        break;
    }
    case 2:
    {
        contracttype = 3;
        break;
    }
    default:
    {
        contracttype = -1;
        break;
    }
    }

    return contracttype;
}

void MoveOrder415::StopRegistrationProcess()
{
    registration = false;
    qDebug() << "RegistrationCompleted";

    manufacturer *companyreciver = getcompanyreciver();
    manufacturer *companysender = getcompanysender();
    QDate operation_date = getoperationDate();
    QString DocNum = getDocNum();
    QDate doc_date = getDocDate();

    int turnovertype = getTurnoverType();
    int sourcetype = getSourceType();
    int contracttype = getSourceType();

    emit RegistrationCompleted (MedicamentsList, companyreciver, companysender,  operation_date, DocNum, doc_date, turnovertype, sourcetype, contracttype);

    //    emit RegistrationCompleted(MedicamentsList,controlsamplestype);
    MedicamentsList.clear();
    emit RegistrationToggled();
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
    int levelIndex = 1;
    int versionIndex = 0;
    bool bExtent = true;
    int maskIndex = -1;
    double scale_size = 2.2;

    QImage i;
    QPixmap scale_image ;

    if (registration != false) // if true
    {
        i = MainWindow::QRCodeToQImageConverter(Stop415ProcessQRString,2.2,  versionIndex, levelIndex, bExtent, maskIndex);
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
        i = MainWindow::QRCodeToQImageConverter(Start415ProcessQRString,2.2,  versionIndex, levelIndex, bExtent, maskIndex);
        ui->qrstartstop->setPixmap( QPixmap::fromImage( i ) );
        ui->RegistrationStartButton->setText("Начать регистрацию");
        ui->GTINTextAgregation->setEnabled(false);
        ui->batchnumberTextAgregation->setEnabled(false);
        ui->expirationdateAgregation->setEnabled(false);
        ui->TNVEDValueAgregation->setEnabled(false);
        ui->serialNumberAgregationValue->setEnabled(false);
        ui->MedicamentsTable->clearContents();
    }

    double scale = 29 * scale_size;
    scale_image =  ui->qrstartstop->pixmap()->scaled( scale, scale );
    ui->qrstartstop->setPixmap( scale_image );
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
