#include "unitextractwidget.h"
#include "ui_unitextractwidget.h"
#include "../../mainwindow.h"
#include <QDebug>

UnitExtractWidget::UnitExtractWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UnitExtractWidget)
{
    ui->setupUi(this);
    connect(this, SIGNAL(RegistrationToggled()), this, SLOT(updateGUI())) ;
    registration = false;
    updateGUI();
}

UnitExtractWidget::~UnitExtractWidget()
{
    delete ui;
}

void UnitExtractWidget::updateWidgetGui(QString gtinstring, QString batchstring, QString SNstring, QString tnvedstring, QString expstring)
{
    ui->GTINTextAgregation->setText(gtinstring);
    ui->serialNumberAgregationValue->setText(SNstring);
    ui->batchnumberTextAgregation->setText(batchstring);
    ui->expirationdateAgregation->setText(expstring);
    ui->TNVEDValueAgregation->setText(tnvedstring);
}

bool UnitExtractWidget::CheckMedicamentinDB(medicament *m)
{
    return false;
}

void UnitExtractWidget::GetMedicament(medicament * med )
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

void UnitExtractWidget::StartRegistrationProcess()
{
    registration = true;
    emit RegistrationStarted();
    emit RegistrationToggled();
}

void UnitExtractWidget::StopRegistrationProcess()
{
    registration = false;
    qDebug() << "RegistrationCompleted";

    int controlsamplestype;

    switch (ui->qualitycomboBox->currentIndex()) {
    case 0:
        controlsamplestype = ToQualityControl;
        break;
    case 1:
        controlsamplestype = ToVerifyCompliance;
        break;
    default:
        break;
    }
    emit RegistrationCompleted(MedicamentsList,controlsamplestype);

    MedicamentsList.clear();
    emit RegistrationToggled();
}

void UnitExtractWidget::ToggleRegistration()
{
    if (registration != false)
        StopRegistrationProcess();
    else
        StartRegistrationProcess();
}

void UnitExtractWidget::on_RegistrationStartButton_clicked()
{
    ToggleRegistration();
}

void UnitExtractWidget::updateGUI()
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
        i = MainWindow::QRCodeToQImageConverter(Stop312ProcessQRString,2.2,  versionIndex, levelIndex, bExtent, maskIndex);
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
        i = MainWindow::QRCodeToQImageConverter(Start312ProcessQRString,2.2,  versionIndex, levelIndex, bExtent, maskIndex);
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

void UnitExtractWidget::AddMedicamentToTable(medicament * m)
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

void UnitExtractWidget::AddMedicamentToDB(medicament *m)
{
}

QImage UnitExtractWidget::QRCodeToQImageConverter( QString textcode, int scale ,  int versionIndex, int levelIndex, bool bExtent, int maskIndex)
{
    CQR_Encode qrEncode;

    qrEncode.EncodeData( levelIndex, versionIndex, bExtent, maskIndex, textcode.toUtf8().data() );
    int qrImageSize = qrEncode.m_nSymbleSize;
    // Создаем двумерный образ кода

    int encodeImageSize = qrImageSize + ( QR_MARGIN * 2 );
    QImage encodeImage2( encodeImageSize, encodeImageSize, QImage::Format_Mono );
    encodeImage2.fill( 1 );

    // Создать двумерный образ кода
    for ( int i = 0; i < qrImageSize; i++ )
        for ( int j = 0; j < qrImageSize; j++ )
            if ( qrEncode.m_byModuleData[i][j] )
                encodeImage2.setPixel( i + QR_MARGIN, j + QR_MARGIN, 0 );
    return encodeImage2;
}
