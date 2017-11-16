#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include "QDateTime"
#include "QSpinBox"
#include "QTextCodec"
#include "QByteArray"

QByteArray text;
int t;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    QPixmap pixmap(QDir::currentPath() + "/logo.JPG");
    ui->organizationLabel->setPixmap(pixmap);
    ui->organizationLabel->show();

    //    QPixmap pixmapqr(QDir::currentPath() + "/startapp.jpg");

    pixmapqr = new QPixmap(QDir::currentPath() + "/startapp.jpg");
    ui->qrstartstop->setPixmap(*pixmapqr);
    ui->qrstartstop->show();
    ui->qrstartstop->setScaledContents(1);


    this->installEventFilter(this);

    journalTimer = new QTimer();
    journalTimer->setInterval(550);

    connect(journalTimer, SIGNAL(timeout()), this, SLOT(addMessageToJournal()));
    connect(journalTimer, SIGNAL(timeout()), this, SLOT(updateDMPicture()));
    connect(journalTimer, SIGNAL(timeout()), this, SLOT(updateDMcode()));

    datetimeTimer = new QTimer();
    datetimeTimer->setInterval(1000);
    connect(datetimeTimer, SIGNAL(timeout()), this, SLOT(updateTimeDate()));


    DMCodeUpdateTimeoutTimer = new QTimer();
    DMCodeUpdateTimeoutTimer->setInterval(100);
    connect(DMCodeUpdateTimeoutTimer, SIGNAL(timeout()), this, SLOT(updateReadedDMCode()));

    DMCodeUpdateTimeoutTimer->start();
    signalMapper = new QSignalMapper (this) ;

    connect(ui->printControlButton, SIGNAL(pressed()), signalMapper, SLOT(map())) ;
    connect(ui->programOptionsButton, SIGNAL(pressed()), signalMapper, SLOT(map())) ;
    connect(ui->agregationButton, SIGNAL(pressed()), signalMapper, SLOT(map())) ;
    connect(ui->statisticksButton, SIGNAL(pressed()), signalMapper, SLOT(map())) ;
    connect(ui->XMLButton, SIGNAL(pressed()), signalMapper, SLOT(map())) ;

    connect(ui->agregationStartButton, SIGNAL(pressed()), this, SLOT(toggleAgregation())) ;
    connect(this, SIGNAL(agregationstatusToggled()), this, SLOT(updateAgregationGUI())) ;

    // ПРИСВАИВАЕМ КАЖДОМУ СИГНАЛУ КНОПКИ ИНДЕКС
    signalMapper -> setMapping (ui->printControlButton, 0) ;
    signalMapper -> setMapping (ui->programOptionsButton, 1) ;
    signalMapper -> setMapping (ui->agregationButton, 2) ;
    signalMapper -> setMapping (ui->statisticksButton, 3) ;
    signalMapper -> setMapping (ui->XMLButton, 4) ;

    connect (signalMapper, SIGNAL(mapped(int)), this, SLOT(setStackedPage(int)));

    journalTimer->start();
    datetimeTimer->start();


    scene = new QGraphicsScene();
    view = new QGraphicsView(scene);
    messages = new QStringList();
    messages->append("Брак");
    messages->append("Ок");

    imageObject = new QImage();
    imageObject->load(QDir::currentPath() + "/DM1.JPG");
    image = QPixmap::fromImage(*imageObject);

    scene = new QGraphicsScene(this);
    scene->addPixmap(image);
    scene->setSceneRect(image.rect());
    ui->packagePicture->setScene(scene);
    ui->packagePicture->show();

    setAgregation(false);
    updateAgregationGUI();
    setStackedPage(2);
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::GenerateNumber(int High, int Low)
{
    int val = (qrand() % ((High + 1) - Low) + Low);
    return val;
}

QString MainWindow::getGuiGTIN()
{
    return ui->GTINText->toPlainText();
}

QString MainWindow::getSN()
{
    return SN;
}

QString MainWindow::generateSN()
{
    QString newSN = "12345qzRF" + QString::number(GenerateNumber(999,100));
    return newSN;
}

QString MainWindow::getGuiBatchNumber()
{
    return ui->batchvalue->text();
}

QString MainWindow::getGuiExpery()
{
    return ui->expirationdate->text();
}

QString MainWindow::getGuiTNVED()
{
    return ui->TNVEDValue->toPlainText();
}

void MainWindow::SetSN(QString newSN)
{
    SN = newSN;
}

bool MainWindow::getAgregation()
{
    return agregation;
}


void MainWindow::addMessageToJournal()
{

    QListWidgetItem * item = new QListWidgetItem ();

    int High = 100, Low = 0;

    int val = GenerateNumber(High, Low);

    if( val > 30)
    {
        item->setTextColor(Qt::green);
        item->setText(messages->at(1));
    }
    else
    {
        item->setTextColor(Qt::red);
        item->setText(messages->at(0));
    }

    ui->journalList->addItem(item);
    ui->journalList->scrollToBottom();
}

void MainWindow::updateTimeDate()
{
    ui->DateTimeLabelValue->setText(QDateTime::currentDateTime().toTimeSpec(Qt::LocalTime).toString("hh-mm-ss dd-MM-yyyy"));
}

void MainWindow::updateReadedDMCode()
{

    ParseDMCode(inputDataStringFromScaner);
    //qDebug() << "stop timeout timer";
    DMCodeUpdateTimeoutTimer->stop();
    inputDataStringFromScaner.clear();
}

void MainWindow::ParseDMCode(QString stringforparse)
{

    QString gtinstring;
    QString snstring;
    QString batchstring;
    QString expstring;
    QString tnvedstring;

    uint8_t Batchlenght = 7; // длина серии может меняться в зависимости...

    if (stringforparse == startcodestring)
    {
        setAgregation(1);
        inputDataStringFromScaner.clear();
        return;
    }

    if (stringforparse == stopcodestring)
    {
        setAgregation(0);
        inputDataStringFromScaner.clear();
        return;
    }



    int gtinstartindex = stringforparse.indexOf(GTINid);
    int snstartindex = stringforparse.indexOf(SNid);
    int batchstartindex = stringforparse.indexOf(Batchid);
    int experiestartindex = stringforparse.indexOf(Experyid);
    int tnvedstartindex = stringforparse.indexOf(TNVEDid);


    // если в прочитаной строке отсутствует хоть один айди из списка то нахрен!
    //    if (   (gtinstartindex == -1) ||
    //           (snstartindex == -1) ||
    //           (batchstartindex == -1) ||
    //           (experiestartindex == -1)
    //           )

    if  (gtinstartindex == -1)
    {
        ui->GTINTextAgregation->setText("Отсутствует");
    }
    else
    {
        ui->GTINTextAgregation->clear();
    }

    if  (snstartindex == -1)
    {
        ui->serialNumberAgregationValue->setText("Отсутствует");
    }
    else
    {
        ui->serialNumberAgregationValue->clear();
    }

    if  (batchstartindex == -1)
    {
        ui->batchnumberTextAgregation->setText("Отсутствует");
    }
    else
    {
        ui->batchnumberTextAgregation->clear();
    }

    if  (experiestartindex == -1)
    {
        ui->expirationdateAgregation->setText("Отсутствует");
    }
    else
    {
        ui->expirationdateAgregation->clear();
    }

    if  (tnvedstartindex == -1)
    {
        ui->TNVEDValueAgregation->setText("Отсутствует");
    }
    else
    {
        ui->TNVEDValueAgregation->clear();
    }

    // если в прочитаной строке отсутствует хоть один айди из списка то нахрен!
        if (   (gtinstartindex == -1) ||
               (snstartindex == -1) ||
               (batchstartindex == -1) ||
               (experiestartindex == -1)
               )
        {
            return;
        }

    qDebug() << gtinstartindex << "gtinstartindex ";
    qDebug() << snstartindex << "snstartindex ";
    qDebug() << batchstartindex << "batchstartindex ";
    qDebug() << experiestartindex <<"experiestartindex" ;
    qDebug() << tnvedstartindex << "tnvedstartindex ";

    // сюда мы перешли если наш код может быть распарсен

    gtinstring = stringforparse.mid(gtinstartindex+GTINid.length(),Gtinlenght);
    snstring = stringforparse.mid(snstartindex+SNid.length(),SNlenght);
    batchstring = stringforparse.mid(batchstartindex+Batchid.length(),Batchlenght);
    expstring = stringforparse.mid(experiestartindex+Experyid.length(),ExpLenght);
    tnvedstring = stringforparse.mid(tnvedstartindex+TNVEDid.length(),TNVEDLenght);

    ui->GTINTextAgregation->setText(gtinstring);
    ui->serialNumberAgregationValue->setText(snstring);
    ui->batchnumberTextAgregation->setText(batchstring);
    ui->expirationdateAgregation->setText(expstring);
    ui->TNVEDValueAgregation->setText(tnvedstring);

    ui->ScannedCode->setText(inputDataStringFromScaner);

    qDebug() << gtinstring << "gtinstring ";
    qDebug() << snstring << "snstring"  ;
    qDebug() << batchstring << "batchstring"  ;
    qDebug() << expstring << "expstring"  ;
    qDebug() << tnvedstring << "tnvedstring"  ;
}

void MainWindow::updateDMPicture()
{
    int val = GenerateNumber(3, 1);
    imageObject->load(QDir::currentPath() + "/DM" + QString::number(val) + ".JPG");
    image = QPixmap::fromImage(*imageObject);
    ui->packagePicture->setScene(scene);
    scene->addPixmap(image);
    scene->setSceneRect(image.rect());
    ui->packagePicture->show();
}

void MainWindow::updateDMcode()
{
    ui->DMcodeValue->setText(GenerateDMcode());
}

void MainWindow::setAgregation(bool set)
{
    agregation = set;
    emit agregationstatusToggled();
}

void MainWindow::toggleAgregation()
{
    if(getAgregation() == false)
    {
        setAgregation(true);
    }
    else
    {
        setAgregation(false);
    }
}

void MainWindow::updateAgregationGUI()
{
    if (getAgregation() == true)
    {
        ui->agregationStartButton->setText("Закончить агрегацию");
        ui->GTINTextAgregation->setEnabled(true);
        ui->batchnumberTextAgregation->setEnabled(true);
        ui->expirationdateAgregation->setEnabled(true);
        ui->TNVEDValueAgregation->setEnabled(true);
        ui->ScannedCode->setEnabled(true);
        ui->serialNumberAgregationValue->setEnabled(true);

        pixmapqr->load(QDir::currentPath() + "/stopapp.jpg");
        ui->qrstartstop->setPixmap(*pixmapqr);
        ui->qrstartstop->show();
        ui->qrstartstop->setScaledContents(1);

    }
    else
    {
        ui->agregationStartButton->setText("Начать агрегацию");
        ui->GTINTextAgregation->setEnabled(false);
        ui->batchnumberTextAgregation->setEnabled(false);
        ui->expirationdateAgregation->setEnabled(false);
        ui->TNVEDValueAgregation->setEnabled(false);
        ui->ScannedCode->setEnabled(false);
        ui->serialNumberAgregationValue->setEnabled(false);

        pixmapqr->load(QDir::currentPath() + "/startapp.jpg");
        ui->qrstartstop->setPixmap(*pixmapqr);
        ui->qrstartstop->show();
        ui->qrstartstop->setScaledContents(1);
    }

    if (inputDataStringFromScaner.isEmpty())
        ui->ScannedCode->clear();

    if ( (inputDataStringFromScaner!= startcodestring) && (inputDataStringFromScaner!= stopcodestring) )
    {
        ui->ScannedCode->setText(inputDataStringFromScaner);
    }
}

QString MainWindow::GenerateDMcode()
{
    QString GTINid = "01";
    QString SNid = "21";
    QString Batchid = "10";
    QString Experyid = "17";
    QString TNVEDid = "240";
    QString DMCode = GTINid + getGuiGTIN() + SNid  +generateSN() + Batchid + getGuiBatchNumber() +  Experyid + getGuiExpery() + TNVEDid  + getGuiTNVED();
    updateQRImage();
    return DMCode;
}

void MainWindow::setStackedPage(int newindex)
{
    ui->stackedWidget->setCurrentIndex(newindex);
}


void MainWindow::updateQRImage()
{
    int levelIndex = 1;
    int versionIndex = 0;
    bool bExtent = true;
    int maskIndex = -1;
    QString encodeString = ui->DMcodeValue->text();

    successfulEncoding = qrEncode.EncodeData( levelIndex, versionIndex, bExtent, maskIndex, encodeString.toUtf8().data() );
    if ( !successfulEncoding )
    {
        ui->image_label->clear();
        ui->image_label->setText( tr("QR Code...") );
        return;
    }

    int qrImageSize = qrEncode.m_nSymbleSize;

    // Создаем двумерный образ кода
    encodeImageSize = qrImageSize + ( QR_MARGIN * 2 );
    QImage encodeImage( encodeImageSize, encodeImageSize, QImage::Format_Mono );
    encodeImage.fill( 1 );

    // Создать двумерный образ кода
    for ( int i = 0; i < qrImageSize; i++ )
        for ( int j = 0; j < qrImageSize; j++ )
            if ( qrEncode.m_byModuleData[i][j] )
                encodeImage.setPixel( i + QR_MARGIN, j + QR_MARGIN, 0 );

    ui->image_label->setPixmap( QPixmap::fromImage( encodeImage ) );

    setScale(3);
    //    ui->pButtonSave->setEnabled( successfulEncoding );
}

void MainWindow::addSymbolToInputString(QString str)
{
    DMCodeUpdateTimeoutTimer->start();

    //  qDebug() << "start timeout timer";
    QString wastext = inputDataStringFromScaner;
    wastext.append(str);
    inputDataStringFromScaner = wastext;
}

void MainWindow::setScale(int scale)
{
    if ( successfulEncoding )
    {
        int scale_size = encodeImageSize * scale;

        const QPixmap & scale_image = ui->image_label->pixmap()->scaled( scale_size, scale_size );
        ui->image_label->setPixmap( scale_image );
    }
}

bool MainWindow::eventFilter(QObject* obj, QEvent* event)
{

    if (event->type()==QEvent::KeyRelease) {

        QKeyEvent* key = static_cast<QKeyEvent*>(event);

        int key1 = key->key();

        if ( (key->key()==Qt::Key_Enter) || (key->key()==Qt::Key_Return)|| (key->key()==Qt::Key_Shift) ) {
            //Enter or return was pressed
        } else {
            keyString = QString( QChar(key1) );
            addSymbolToInputString(keyString);
            return QObject::eventFilter(obj, event);
        }
        return true;
    } else {
        return QObject::eventFilter(obj, event);
    }
    return false;
}
