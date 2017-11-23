#include "unitextractwidget.h"
#include "ui_unitextractwidget.h"
#include "mainwindow.h"
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

UnitExtractWidget::GetParsedString(QString gtinstring, QString SNstring, QString tnvedstring, QString expstring, QString batchstring, QString sGTINString)
{
    updateWidgetGui(gtinstring, batchstring, SNstring, tnvedstring, expstring);

    qDebug() << gtinstring<< SNstring<< tnvedstring<< expstring<< batchstring<< sGTINString;
}

void UnitExtractWidget::ToggleRegistration()
{
    if (registration != false)
    {
//        emit StartRegisterControlSamples();
        registration = false;
    }
    else
    {
//        emit StopRegisterControlSamples();
        registration = true;
    }
    emit RegistrationToggled();
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
    auto scale_size = 2.2;

    QImage i;
    QPixmap scale_image ;




    if (registration != false) // if true
    {
        i = MainWindow::QRCodeToQImageConverter("finish",2.2,  versionIndex, levelIndex, bExtent, maskIndex);
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
        i = MainWindow::QRCodeToQImageConverter("start",2.2,  versionIndex, levelIndex, bExtent, maskIndex);
        ui->qrstartstop->setPixmap( QPixmap::fromImage( i ) );

        ui->RegistrationStartButton->setText("Начать регистрацию");
        ui->GTINTextAgregation->setEnabled(false);
        ui->batchnumberTextAgregation->setEnabled(false);
        ui->expirationdateAgregation->setEnabled(false);
        ui->TNVEDValueAgregation->setEnabled(false);
        ui->serialNumberAgregationValue->setEnabled(false);

        ui->MedicamentsTable->clearContents();

    }

        auto scale = 29 * scale_size;
    scale_image =  ui->qrstartstop->pixmap()->scaled( scale, scale );
    ui->qrstartstop->setPixmap( scale_image );

//    if (inputDataStringFromScaner.isEmpty())
//        ui->ScannedCode->clear();

//    if ( (inputDataStringFromScaner!= startcodestring) && (inputDataStringFromScaner!= stopcodestring) )
//    {
//        ui->ScannedCode->setText(inputDataStringFromScaner);
//    }


//    ui->GTINTextAgregation->setText(gtinstring);
//    ui->serialNumberAgregationValue->setText(SNstring);
//    ui->batchnumberTextAgregation->setText(batchstring);
//    ui->expirationdateAgregation->setText(expstring);
//    ui->TNVEDValueAgregation->setText(tnvedstring);
}
