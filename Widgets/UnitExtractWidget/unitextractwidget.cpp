#include "unitextractwidget.h"
#include "ui_unitextractwidget.h"
#include "mainwindow.h"
#include <QDebug>

UnitExtractWidget::UnitExtractWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UnitExtractWidget)
{
    ui->setupUi(this);

    int levelIndex = 1;
    int versionIndex = 0;
    bool bExtent = true;
    int maskIndex = -1;
    auto scale_size = 2.2;

    QImage i = MainWindow::QRCodeToQImageConverter("blabla",2.2,  versionIndex, levelIndex, bExtent, maskIndex);

    auto scale = 29 * scale_size;

    ui->qrstartstop->setPixmap( QPixmap::fromImage( i ) );


    QPixmap scale_image =  ui->qrstartstop->pixmap()->scaled( scale, scale );
    ui->qrstartstop->setPixmap( scale_image );
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

UnitExtractWidget::GetParcedString(QString gtinstring, QString SNstring, QString tnvedstring, QString expstring, QString batchstring, QString sGTINString)
{
    updateWidgetGui(gtinstring, batchstring, SNstring, tnvedstring, expstring);

    qDebug() << gtinstring<< SNstring<< tnvedstring<< expstring<< batchstring<< sGTINString;
}

void UnitExtractWidget::on_RegistrationStartButton_clicked()
{

}
