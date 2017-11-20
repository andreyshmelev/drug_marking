#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include "QDateTime"
#include "QSpinBox"
#include "QTextCodec"
#include "QByteArray"
#include "QtXml"
#include "QFile"

int t;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPixmap pixmap(QDir::currentPath() + "/logo.JPG");
    ui->organizationLabel->setPixmap(pixmap);
    ui->organizationLabel->show();

    pixmapqr = new QPixmap(QDir::currentPath() + "/startapp.jpg");
    ui->qrstartstop->setPixmap(*pixmapqr);
    ui->qrstartstop->show();
    ui->qrstartstop->setScaledContents(1);

    this->installEventFilter(this);

    journalTimer = new QTimer();
    journalTimer->setInterval(250);

    connect(journalTimer, SIGNAL(timeout()), this, SLOT(addMessageToJournal()));
    connect(journalTimer, SIGNAL(timeout()), this, SLOT(updateDMPicture()));
    connect(journalTimer, SIGNAL(timeout()), this, SLOT(updateDMcode()));

    datetimeTimer = new QTimer();
    datetimeTimer->setInterval(1000);
    connect(datetimeTimer, SIGNAL(timeout()), this, SLOT(updateTimeDate()));

    DMCodeUpdateTimeoutTimer = new QTimer();
    DMCodeUpdateTimeoutTimer->setInterval(100); // таймер который обновлятся при каждом нажатии клавиши, при его переполнении мы идем парсить строку (все символы со сканера приняты)
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
    connect(this, SIGNAL(ParcingEnded()), this, SLOT(updateAgregationGUI())) ;

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

    CreateXML313Doc();
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

QString MainWindow::GetRegularString(QString stringforparse, QString SNRegularexpression)
{
    QString matched ;
    QRegularExpression re(SNRegularexpression );
    QRegularExpressionMatch match = re.match(stringforparse);
    if (match.hasMatch()) {
        matched = match.captured(0);
        qDebug() << matched << "matched";
    }

    return matched;
}

void MainWindow::CreateXML313Doc()
{
    QDomDocument document;
    QDomElement root = document.createElement("documents");
    document.appendChild(root);



    //for (int var = 0; var < 10; ++var)
    {
        QDomElement registerproductemissionelement  = document.createElement("register_product_emission");
        registerproductemissionelement.setAttribute("action_id", 313);
        root.appendChild(registerproductemissionelement);
    }

    // add  register_product_emission element

    QDomElement registerproductemissionelement = document.createElement("register_product_emission action_id");

    registerproductemissionelement.setAttribute("ID", 313);

    //    documents.appendChild(registerproductemissionelement);

    QFile file(QDir::currentPath()   + "/313-register_product_emission.xml");

    if ( !file.open(QIODevice::WriteOnly| QIODevice::Text))
    {
        qDebug() << "Failed to open";
        //return -1;
    }
    else
    {
        QTextStream stream(&file);
        stream<< document.toString();
        file.close();
        qDebug() << "Finished";
        qDebug() << QDir::currentPath()   + "/313-register_product_emission.xml";

    }
}

void MainWindow::ParseDMCode(QString stringforparse)
{

    if (stringforparse == startcodestring)
    {
        setAgregation(1);
        inputDataStringFromScaner.clear();
        return;
    }

    if (stringforparse == stopcodestring)
    {
        CreateXML313Doc();
        setAgregation(0);
        inputDataStringFromScaner.clear();
        return;
    }

    ui->ScannedCode->setText(inputDataStringFromScaner);

    // сюда перешли если нас устроил код

    // добавляем в конец строки символ завершения <GS>, точнее часть его,
    // чтобы обрабатывать поля, которые стоят в конце строки и которые не заканчиваются этим символом.
    // Но если эти поля в центре то они должны заканчиваться этим символом ( кроме даты выпуска )

    stringforparse.append(GSSymbol);

    // начинаем разбирать GTIN
    // он всегда должен быть в начала. остальные группы данных могут быть в любом месте, и заканчиваться символом 29, за исключением если они находятся в конце строки.

    if (stringforparse.indexOf(GTINid) == 0)    // если начинается как нужно - с 01 то продолжаем, иначе - ошибка
    {
        gtinstring = stringforparse;
        gtinstring = gtinstring.remove(0,2);
        gtinstring = gtinstring.mid(0,Gtinlenght);
        //удаляем джитин из общей строки
        stringforparse.remove(0,Gtinlenght + GTINid.length());
    }

    else
    {
        gtinstring = NotFoundString;
    }

    // кончаем разбирать GTIN

    // начинаем разбирать серийник


    SNstring = GetRegularString(stringforparse, SNRegularexpression);
    SNstring.remove(0,2);
    SNstring.replace(GSSymbol,Emptystring);

    if(SNstring == NULL)
    {
        SNstring = NotFoundString;
    }

    // заканчиваем разбирать серийник

    // начинаем разбирать ТНВЭД

    tnvedstring = GetRegularString(stringforparse, TNVEDRegularexpression);
    tnvedstring.remove(0,3);
    tnvedstring.replace(GSSymbol,Emptystring);
    if(tnvedstring == NULL)
    {
        tnvedstring = NotFoundString;
    }

    // кончаем разбирать ТНВЭД


    // начинаем разбирать Срок Годности

    expstring = GetRegularString(stringforparse, ExpRegularexpression);
    expstring.remove(0,2);
    expstring.replace(GSSymbol,Emptystring);
    if(expstring == NULL)
    {
        expstring = NotFoundString;
    }

    // кончаем разбирать Срок Годности


    // начинаем разбирать Партию

    batchstring = GetRegularString(stringforparse, BatchRegularexpression);
    batchstring.remove(0,2);
    batchstring.replace(GSSymbol,Emptystring);

    if(batchstring == NULL)
    {
        batchstring = NotFoundString;
    }

    // кончаем разбирать Партию


    emit ParcingEnded(); // испускаем сигнал что закончили парсинг строки
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


    ui->GTINTextAgregation->setText(gtinstring);
    ui->serialNumberAgregationValue->setText(SNstring);
    ui->batchnumberTextAgregation->setText(batchstring);
    ui->expirationdateAgregation->setText(expstring);
    ui->TNVEDValueAgregation->setText(tnvedstring);

}

QString MainWindow::GenerateDMcode()
{
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

    if (getAgregation() == true)
    {
        // Просто меняем раскладку если у нас агрегация.
        // так как Ручной сканер работает как клавиатура, то он эмулирует нажатие клавиш, что при русской раскладке дает неверные символы

        if (str == "Й") { str = "Q" ; } else
            if (str == "Ц") { str = "W" ; } else
                if (str == "У") { str = "E" ; } else
                    if (str == "К") { str = "R" ; } else
                        if (str == "Е") { str = "T" ; } else
                            if (str == "Н") { str = "Y" ; } else
                                if (str == "Г") { str = "U" ; } else
                                    if (str == "Ш") { str = "I" ; } else
                                        if (str == "Щ") { str = "O" ; } else
                                            if (str == "З") { str = "P" ; } else
                                                if (str == "Х") { str = "[" ; } else
                                                    if (str == "Ъ") { str = "]" ; } else

                                                        if (str == "Ф") { str = "A" ; } else
                                                            if (str == "Ы") { str = "S" ; } else
                                                                if (str == "В") { str = "D" ; } else
                                                                    if (str == "А") { str = "F" ; } else
                                                                        if (str == "П") { str = "G" ; } else
                                                                            if (str == "Р") { str = "H" ; } else
                                                                                if (str == "О") { str = "J" ; } else
                                                                                    if (str == "Л") { str = "K" ; } else
                                                                                        if (str == "Д") { str = "L" ; } else
                                                                                            if (str == "Ж") { str = ";" ; } else
                                                                                                if (str == "Э") { str = "'" ; } else

                                                                                                    if (str == "Я") { str = "Z" ; } else
                                                                                                        if (str == "Ч") { str = "X" ; } else
                                                                                                            if (str == "С") { str = "C" ; } else
                                                                                                                if (str == "М") { str = "V" ; } else
                                                                                                                    if (str == "И") { str = "B" ; } else
                                                                                                                        if (str == "Т") { str = "N" ; } else
                                                                                                                            if (str == "Ь") { str = "M" ; } else
                                                                                                                                if (str == "Б") { str = "," ; } else
                                                                                                                                    if (str == "Ю") { str = "." ; } else
                                                                                                                                        if (str == ".") { str = "/" ; }

    }

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
        //qDebug() << text;
        if ( (key->key()==Qt::Key_Enter) || (key->key()==Qt::Key_Return)|| (key->key()==Qt::Key_Shift) ) {
            //Enter or return was pressed
        } else {
            keyString = QString( QChar(key1) );
            addSymbolToInputString(keyString);

            //            qDebug() << keyString << key1;


            //            if(key1 == NULL)
            //                qDebug() << " = 0";

            return QObject::eventFilter(obj, event);
        }
        return true;
    } else {
        return QObject::eventFilter(obj, event);
    }
    return false;
}
