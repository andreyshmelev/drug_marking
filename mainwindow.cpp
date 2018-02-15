#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include "QDateTime"
#include "QSpinBox"
#include "QTextCodec"
#include "QByteArray"
#include "QtXml"
#include "QFile"
#include "QDesktopServices"
#include "manufacturer.h"
#include "basetypes.h"
#include "serializationline.h"
#include "sql.h"
#include "QtPrintSupport/qprinter.h"
#include "QtPrintSupport/QPrinter"
#include "QtPrintSupport/QPrintDialog"
#include <QTime>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>

#define testitemscount 0

int aaa = 222;
int bbb = 789;

int summint;

QElapsedTimer MainWindow::SQLInsertSpeedTest()
{
    QString req;
    QElapsedTimer timer;
    timer.start();

    qDebug() << "The slow operation took" << timer.elapsed()/1000<< "seconds";
    qDebug() << "The slow operation took" << timer.elapsed() << "milliseconds";
    return timer;
}

QElapsedTimer MainWindow::SQLSelectSpeedTest()
{
    QString req;
    QElapsedTimer timer;
    timer.start();
    //    QStringList sellist = sqlDB->sel("company", "company", "","company");
    QStringList sellist = sqlDB->sel("date", "scannerlog", "Message = 'EHa7vpPtxgzWhNCC108D6RUnZIo2AJCnKmNRhtuYMtaTAm8Shw'","date");

    qDebug() << "The SQLSelectSpeedTest took" << timer.elapsed()/1000<< "seconds";
    qDebug() << "The SQLSelectSpeedTest took" << timer.elapsed() << "milliseconds";
    return timer;
}

void MainWindow::GetCompaniesDBList()
{
    companies = sqlDB->sel("company_name", "company", "","company_name");

    foreach (QString s, companies) {

        // читаем производителя из БД
        QString fromcompany = "company";
        QString    wherecompany = "company_name = '" + s + "' ";
        // подтягиваем параметры компании
        QString companyname = sqlDB->sel("company_name", fromcompany, wherecompany,"company_name")[0];
        QString company_subject_id = sqlDB->sel("subject_id", fromcompany, wherecompany,"subject_id")[0];
        QString company_owner_id = sqlDB->sel("owner_id", fromcompany, wherecompany,"owner_id")[0];
        QString company_email = sqlDB->sel("email", fromcompany, wherecompany,"email")[0];
        QString company_ul = sqlDB->sel("ul", fromcompany, wherecompany,"ul")[0];
        QString company_fl = sqlDB->sel("fl", fromcompany, wherecompany,"fl")[0];
        QString company_inn = sqlDB->sel("inn", fromcompany, wherecompany,"inn")[0];
        QString company_kpp = sqlDB->sel("kpp", fromcompany, wherecompany,"kpp")[0];
        QString company_gs1id = sqlDB->sel("gs1id", fromcompany, wherecompany,"gs1id")[0];

        manufacturer * c = new manufacturer (company_subject_id,companyname,company_email, company_ul, company_fl, company_inn,company_kpp,company_owner_id , company_gs1id);
        CompaniesListFromDB.append(c);
    }
    emit SendCompaniesDBList(CompaniesListFromDB);
}

void MainWindow::GetStatisticsFromDB()
{
    QString curtext ;

    statisticsbisnessprocesses = sqlDB->seldistinct("BProcess", "mark.statistics", "","BProcess");

    curtext = ui->StatistBPcomboBox->currentText();

    ui->StatistBPcomboBox->clear();
    ui->StatistBPcomboBox->addItem("%");
    ui->StatistBPcomboBox->addItems(statisticsbisnessprocesses);
    ui->StatistBPcomboBox->setCurrentText(curtext);

    statisticsmedicaments = sqlDB->seldistinct("LPName", "mark.statistics", "","LPName");

    curtext = ui->StatistMedicamentComboBox->currentText();
    ui->StatistMedicamentComboBox->clear();
    ui->StatistMedicamentComboBox->addItem("%");
    ui->StatistMedicamentComboBox->addItems(statisticsmedicaments);
    ui->StatistMedicamentComboBox->setCurrentText(curtext);

    statisticsbatches = sqlDB->seldistinct("batch", "mark.statistics", "","batch");

    curtext = ui->StatistBatchComboBox->currentText();
    ui->StatistBatchComboBox->clear();
    ui->StatistBatchComboBox->addItem("%");
    ui->StatistBatchComboBox->addItems(statisticsbatches);
    ui->StatistBatchComboBox->setCurrentText(curtext);

    QStringList statisticsgtin = sqlDB->seldistinct("GTIN", "mark.statistics", "","GTIN");

    curtext = ui->StatistGTINCombobox->currentText();
    ui->StatistGTINCombobox->clear();
    ui->StatistGTINCombobox->addItem("%");
    ui->StatistGTINCombobox->addItems(statisticsgtin);
    ui->StatistGTINCombobox->setCurrentText(curtext);
}

bool MainWindow::IsDateProper(QString stringtotest)
{
    QDate TestDate = QDate::fromString(stringtotest,"yyMMdd");
    bool result;
    qDebug() << TestDate << "TestDate ";
    if (TestDate.toString("yyMMdd") != stringtotest)
        result = false;
    else
        result = true;
    return result;
}

void MainWindow::SQLInit()
{
    sqlDB = new SQL("ненужная строка");
    drugs = sqlDB->sel("drugs_name", "drugs", "","drugs_name");
    //    sqlDB->makesqlreq(QString("insert into process911 values (%1,%2,%3,%4,%5)").arg("insertfromQt",QDateTime::currentDateTime().toTimeSpec(Qt::LocalTime).toString("dd-MM-yyyy"),QDateTime::currentDateTime().toTimeSpec(Qt::LocalTime).toString("hh-mm-ss"),"XML","SSCC"));
    sqlDB->makesqlreq(QString("insert into process911 values (\"qt53\",\"qt53\",\"qt33\",\"44\",\"55\");") );
    // подтягиваем параметры компании
    GetCompaniesDBList();
    GetStatisticsFromDB();
}

void MainWindow::SetLibrariesPath()
{
    QCoreApplication::addLibraryPath(QDir::currentPath());
}

void MainWindow::SetStyleSheets()
{

    QList<QLabel *> list = this->findChildren<QLabel *>();

    int i = 0 ;
    foreach(QLabel *l, list)
    {
        l->setStyleSheet("QLabel {background-color: QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #547FA8, stop: 0.1 #215689, stop: 0.49 #215689, stop: 0.5 #215689, stop: 1 #547FA8);border-style: outset;border-width: 1px;border-radius: 4px;                   border-color: beige;                   padding: 6px;               }");
    }



    QList<QCheckBox *> checkboxeslist = this->findChildren<QCheckBox *>();
    foreach(QCheckBox *l, checkboxeslist)
    {
        qDebug() <<l->objectName() ;

        l->setStyleSheet("QCheckBox::indicator {width: 20px;height: 20px;}");
    }



    QList<QPushButton *> but = this->findChildren<QPushButton *>();

    QString styleMainButtons = "QPushButton\
    {\
            background-color: QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #F19D6E, stop: 0.1 #E25303, stop: 0.49 #E25303, stop: 0.5 #E25303, stop: 1 #F19D6E);\
            \
            \
            border-style: outset;\
            border-width: 2px;\
            border-radius: 10px;\
            border-color: beige;\
            font: bold 18px; color: white;\
            padding: 6px;\
}\
            QPushButton:disabled \
    {\
            background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(226,83,3, 70), stop:1 rgba(226,83,3, 70));\
            border-style: outset;\
            border-width: 2px;\
            border-radius: 10px;\
            border-color: beige;\
            font: bold 18px; color: white;\
            padding: 6px;\
}\
            \
            QPushButton:pressed \
    {\
            background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(0xe2,0x3c,0x03, 255), stop:1 rgba(226,83,3, 255));\
            border-style: outset;\
            border-width: 2px;\
            border-radius: 10px;\
            border-color: beige;\
            font: bold 18px; color: white;\
            padding: 6px;\
}";


            ;
    QString styleRegularButtons = "QPushButton\
    {\
            background-color: QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #F19D6E, stop: 0.1 #E25303, stop: 0.49 #E25303, stop: 0.5 #E25303, stop: 1 #F19D6E);\
            \
            \
            border-style: outset;\
            border-width: 2px;\
            border-radius: 10px;\
            border-color: beige;\
            font: bold 14px; color: white;\
            padding: 6px;\
}\
            QPushButton:disabled \
    {\
            background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(226,83,3, 70), stop:1 rgba(226,83,3, 70));\
            border-style: outset;\
            border-width: 2px;\
            border-radius: 10px;\
            border-color: beige;\
            font: bold 14px; color: white;\
            padding: 6px;\
}\
            \
            QPushButton:pressed \
    {\
            background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(0xe2,0x3c,0x03, 255), stop:1 rgba(226,83,3, 255));\
            border-style: outset;\
            border-width: 2px;\
            border-radius: 10px;\
            border-color: beige;\
            font: bold 14px; color: white;\
            padding: 6px;\
}";

            i = 0 ;
    foreach(QPushButton *b, but)
    {
        b->setStyleSheet(styleRegularButtons);
    }

    ui->printControlButton->setStyleSheet(styleMainButtons);
    ui->programOptionsButton->setStyleSheet(styleMainButtons);
    ui->agregationButton->setStyleSheet(styleMainButtons);
    ui->statisticksButton->setStyleSheet(styleMainButtons);

}

// MainWindow  constructor
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    agregation = false;

    updateQRLabels();

    QPixmap pixmap(QDir::currentPath() + "/logo.JPG");
    ui->organizationLabel->setPixmap(pixmap);
    ui->organizationLabel->show();

    pixmapqr = new QPixmap(QDir::currentPath() + "/startapp.jpg");

    this->installEventFilter(this);

    RandomStringSenderToVideoJetTimer = new QTimer();
    RandomStringSenderToVideoJetTimer->setInterval(350); // каждые сорок пять секунд посылаем новую произвольную строку
    //    RandomStringSenderToVideoJetTimer->setInterval(1000); // каждые сорок пять секунд посылаем новую произвольную строку
    connect(RandomStringSenderToVideoJetTimer, &QTimer::timeout, this, &MainWindow::SendRandomToVideoJet);
    RandomStringSenderToVideoJetTimer->start();

    datetimeTimer = new QTimer();
    datetimeTimer->setInterval(1000);
    connect(datetimeTimer, SIGNAL(timeout()), this, SLOT(updateTimeDate()));

    DMCodeUpdateTimeoutTimer = new QTimer();
    DMCodeUpdateTimeoutTimer->setInterval(100); // таймер который обновлятся при каждом нажатии клавиши, при его переполнении мы идем парсить строку (все символы со сканера приняты)
    connect(DMCodeUpdateTimeoutTimer, SIGNAL(timeout()), this, SLOT(updateReadedDMCode()));

    DMCodeUpdateTimeoutTimer->start();
    signalMapper = new QSignalMapper (this) ;

    connect(ui->printControlButton, SIGNAL(pressed()), this, SLOT(PrintControlPageOpen())) ;
    connect(ui->programOptionsButton, SIGNAL(pressed()), this, SLOT(productOptionsPageOpen())) ;
    connect(ui->agregationButton, SIGNAL(pressed()), this, SLOT(agregationOptionsPageOpen())) ;
    connect(ui->statisticksButton, SIGNAL(pressed()), this, SLOT(statisticsPageOpen())) ;

    connect(this, SIGNAL(printControlQRCodeScanned()), this, SLOT(PrintControlPageOpen())) ;
    connect(this, SIGNAL(programOptionsQRCodeScanned()), this, SLOT(productOptionsPageOpen())) ;
    connect(this, SIGNAL(agregationQRCodeScanned()), this, SLOT(agregationOptionsPageOpen())) ;
    connect(this, SIGNAL(statisticsQRCodeScanned()), this, SLOT(statisticsPageOpen())) ;

    // сигналы и слоты для 311 бизнес процесса
    connect(ui->RegisterEndPackingPage311Widget, &RegisterEndPackingWidget311::setScannerLanguage, this, &MainWindow::setLanguageswitcher) ;
    connect(ui->RegisterEndPackingPage311Widget, &RegisterEndPackingWidget311::AddMedicamentToDBTable,this , &MainWindow::AddMedicamentToDBTable );
    connect(this, &MainWindow::SendMedicamentSignal, ui->RegisterEndPackingPage311Widget, &RegisterEndPackingWidget311::GetMedicament) ;
    connect(this, &MainWindow::SendCompaniesDBList, ui->RegisterEndPackingPage311Widget, &RegisterEndPackingWidget311::GetCompaniesDBList) ;
    connect(this, SIGNAL(SendCompaniesDBList(QList<manufacturer*>)), this, SLOT(GetCompaniesDBList(QList<manufacturer*>))) ;
    connect(ui->RegisterEndPackingPage311Widget, SIGNAL(RegistrationCompleted(QList<medicament*>,manufacturer*,manufacturer*,int,QDateTime)), this, SLOT(CreateXML311Doc(QList<medicament*>,manufacturer*,manufacturer*,int,QDateTime)));

    // сигналы и слоты для 312 бизнес процесса
    connect(this, &MainWindow::Start312Process, ui->ExtractWidget, &UnitExtractWidget::StartRegistrationProcess ) ;
    connect(this, &MainWindow::Stop312Process, ui->ExtractWidget, &UnitExtractWidget::StopRegistrationProcess ) ;
    connect(this, SIGNAL(SendMedicamentSignal(medicament*)), ui->ExtractWidget, SLOT(GetMedicament(medicament*))) ;
    connect(ui->ExtractWidget, &UnitExtractWidget::RegistrationCompleted, this, &MainWindow::CreateXML312Doc) ;
    connect(ui->ExtractWidget, &UnitExtractWidget::RegistrationCompleted,this , &MainWindow::StopAgregation ) ;
    connect(ui->ExtractWidget, SIGNAL(RegistrationStarted()),this , SLOT(StartAgregation()) ) ;
    connect(ui->ExtractWidget, &UnitExtractWidget::AddMedicamentToDBTable,this , &MainWindow::AddMedicamentToDBTable );

    // сигналы и слоты для 313 бизнес процесса, пока он в форме mainwindow, позже нужно будет создать отдельный виджет
    connect(ui->registerproductwidget, &RegisterProductWidget313::setScannerLanguage, this, &MainWindow::setLanguageswitcher) ;
    connect(ui->registerproductwidget, &RegisterProductWidget313::AddMedicamentToDBTable,this , &MainWindow::AddMedicamentToDBTable );
    connect(this, &MainWindow::SendMedicamentSignal, ui->registerproductwidget, &RegisterProductWidget313::GetMedicament) ;
    connect(this, &MainWindow::SendCompaniesDBList, ui->registerproductwidget, &RegisterProductWidget313::GetCompaniesDBList) ;
    connect(this, SIGNAL(registerproductwidget(QList<manufacturer*>)), this, SLOT(GetCompaniesDBList(QList<manufacturer*>))) ;
    connect(ui->registerproductwidget, SIGNAL(RegistrationCompleted(QList<medicament*>,QDateTime)), this, SLOT(CreateXML313Doc(QList<medicament*>,QDateTime)));

    //    // сигналы и слоты для 415 бизнес процесса

    //    connect(ui->MoveOrderWidget, &MoveOrder415::RegistrationCompleted, this, &MainWindow::CreateXML415Doc) ;
    //    connect(ui->MoveOrderWidget, &MoveOrder415::setScannerLanguage, this, &MainWindow::setLanguageswitcher) ;
    //    connect(this, &MainWindow::SendMedicamentSignal, ui->MoveOrderWidget, &MoveOrder415::GetMedicament) ;
    //    connect(this, &MainWindow::SendCompaniesDBList, ui->MoveOrderWidget, &MoveOrder415::GetCompaniesDBList) ;
    //    connect(ui->MoveOrderWidget, &MoveOrder415::AddMedicamentToDBTable,this , &MainWindow::AddMedicamentToDBTable );

    // сигналы и слоты для 911 бизнес процесса
    connect(ui->UnitPackPageWidget, &UnitPackWidget911::setScannerLanguage, this, &MainWindow::setLanguageswitcher);
    connect(this, &MainWindow::SendMedicamentSignal, ui->UnitPackPageWidget, &UnitPackWidget911::GetMedicament);
    connect(this, &MainWindow::SendCompaniesDBList, ui->UnitPackPageWidget, &UnitPackWidget911::GetCompaniesDBList);
    connect(ui->UnitPackPageWidget, &UnitPackWidget911::RegistrationCompleted, this, &MainWindow::CreateXML911Doc);

    // сигналы и слоты для 811 бизнес процесса

    //    connect(ui->RelabelingWidget, &RelabelingWidget811::setScannerLanguage, this, &MainWindow::setLanguageswitcher);
    //    connect(this, &MainWindow::SendMedicamentSignal, ui->RelabelingWidget, &RelabelingWidget811::GetMedicament);
    //    connect(this, &MainWindow::SendCompaniesDBList, ui->RelabelingWidget, &RelabelingWidget811::GetCompaniesDBList);
    //    connect(ui->RelabelingWidget, &RelabelingWidget811::RegistrationCompleted, this, &MainWindow::CreateXML911Doc);

    // сигнал-слоты для сериализации

    connect(ui->StartSerializationButton, &QAbstractButton::pressed, this, &MainWindow::StartSerialization) ;
    connect(ui->PauseSerializationButton, &QAbstractButton::pressed, this, &MainWindow::PauseSerialization);
    connect(ui->ContinueSerializationButton, &QAbstractButton::pressed, this, &MainWindow::ContinueSerialization);
    connect(ui->StopSerializationButton, &QAbstractButton::pressed, this, &MainWindow::StopSerialization);

    // ПРИСВАИВАЕМ КАЖДОМУ СИГНАЛУ КНОПКИ ИНДЕКС
    signalMapper -> setMapping (ui->printControlButton, 0) ;
    signalMapper -> setMapping (ui->programOptionsButton, 1) ;
    signalMapper -> setMapping (ui->agregationButton, 2) ;
    signalMapper -> setMapping (ui->statisticksButton, 3) ;

    connect (signalMapper, SIGNAL(mapped(int)), this, SLOT(setStackedPage(int)));

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

    updateAgregationGUI();
    setStackedPage(2);
    SQLInit();
    SerializationCompanySender = CompaniesListFromDB.at(1);

    ui->CompaniesCombobox->clear();
    ui->DrugsComboBox->clear();
    ui->DrugsComboBox->addItems(drugs);
    ui->CompaniesCombobox->addItems(companies);

    //adding TCP Client
    connectTcp(TCPaddress, TCPPort);
    StopAgregation();

    SetLibrariesPath();
    SetStyleSheets();

    // для нормального рандомайза нужно добавить этот блок qsrand
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
    //    StopSerialization();

    //    SerializationLine1 = new SerializationLine();

    ui->batchnumberText->setPlainText(generateSN(6));
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyfinished(QNetworkReply*)));

    QUrl url( "dev-api.markirovka.nalog.ru" );
    QNetworkRequest request( url );
    QString auth = QString( "POST /api/v1/auth/ HTTP/1.1" ) ;
    request.setRawHeader( "Authorization", "Basic " + auth.toLatin1().toBase64() );
    request.setHeader( QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded" );

    QString bound="margin"; //name of the boundary
    //according to rfc 1867 we need to put this string here:

    QByteArray data = ("{\"client_id\": \"ef77a1f8-e374-451d-9da9-7c3519d0d143\",\"client_secret\": \"c4bf1684-eb4e-4119-bed7-b28fc3beb68b\",\"user_id\": \"401425968749462818672954964831426197036787062\",\"auth_type\": \"PASSWORD\"}");

    QNetworkRequest request3; //our server with php-script
//    QUrl serviceURL("http://dev-api.markirovka.nalog.ru/api/v1/auth"); // на это сервер че-то отвечает
    QUrl serviceURL("http://dev-api.markirovka.nalog.ru/api/v1/auth"); // на это сервер че-то отвечает
    request3.setUrl(serviceURL);
    request3.setRawHeader("Content-Type","application/json");
    request3.setRawHeader("Cache-Control","no-cache");
    QNetworkReply *reply = manager->post(request3,data);

    //    request3.setRawHeader("Authorization","token 68f30b78-16b5-4422-8ae5-fd0cab4ceb07");
    //    request3.setRawHeader("Postman-Token","82c8a467-aa6d-3d5f-27e5-092a8c8a8140");
    //    request3.setRawHeader("client_secret","c4bf1684-eb4e-4119-bed7-b28fc3beb68b");
    //    request3.setRawHeader("client_id","ef77a1f8-e374-451d-9da9-7c3519d0d143");
    //    request3.setRawHeader("user_id","test_non_resident");
    //    request3.setRawHeader("auth_type","PASSWORD");
    //    QByteArray data = ("{\"filter\": { \"doc_status\": \"PROCESSED_DOCUMENT\", \"start_date\": \"2018-01-01 01:01:01\", \"end_date\":\"2018-02-09 02:02:02\" }, \"start_from\": 0, \"count\": 100}");
    //connect(manager, SIGNAL(finished(QNetworkReply*)), SLOT(sendingFinished(QNetworkReply*)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ResponseFromLineRecieved(QString address,quint16 port,QString message)
{
    QString date =  QDateTime::currentDateTime().toString("hh:mm::ss:zzz");
    ui->SerialTimeRecieve->setText( date  + " : " + message );
}

int MainWindow::GenerateNumber(int High, int Low)
{
    int val = (qrand() % ((High + 1) - Low) + Low);
    return val;
}

QString MainWindow::getGuiGTIN()
{
    return ui->GTINVal->toPlainText();
}

QString MainWindow::getSN()
{
    return SN;
}

QString MainWindow::generateSN(int lenght)
{
    const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");  // abcdefghijklmnopqrstuvwxyz
    const int randomStringLength = lenght; // SNlenght  assuming you want random strings of 12 characters

    QString randomString;
    for(int i=0; i<randomStringLength; ++i)
    {
        int index = qrand() % possibleCharacters.length();
        QChar nextChar = possibleCharacters.at(index);
        randomString.append(nextChar);
    }

    QString newSN = randomString;
    return newSN;
}


QString MainWindow::generateCode128(int lenght)
{
    //    const QString possibleCharacters("0123456789");
    const QString possibleCharacters("01234567890123456789012345678901234567890123456789012345678901");
    const int randomStringLength = lenght; // SNlenght  assuming you want random strings of 12 characters

    QString randomString;
    for(int i=0; i<randomStringLength; ++i)
    {
        int index = qrand() % possibleCharacters.length();
        QChar nextChar = possibleCharacters.at(index);
        randomString.append(nextChar);
    }

    QString newSN = randomString;
    return newSN;
}

QString MainWindow::getGuiBatchNumber()
{
    return ui->batchvalue->text();
}

QString MainWindow::getGuiBatchValue()
{
    return ui->batchnumberText->toPlainText();
}

QString MainWindow::getGuiExpery()
{
    return ui->expirationdate->text();
}

QDateTime MainWindow::getGuiExperyDate()
{
    return ui->expirationdate->dateTime();
}

bool MainWindow::getAutoprogramagregation() const
{
    return autoprogramagregation;
}

void MainWindow::setAutoprogramagregation(bool value)
{
    autoprogramagregation = value;

    if (autoprogramagregation || autoagregation)
        ui->AgregationGroupBox->setEnabled(false);
    else
        ui->AgregationGroupBox->setEnabled(true);
}

bool MainWindow::getAutoagregation() const
{
    return autoagregation;
}

void MainWindow::setAutoagregation(bool value)
{
    autoagregation = value;



    if (autoprogramagregation || autoagregation)
        ui->AgregationGroupBox->setEnabled(false);
    else
        ui->AgregationGroupBox->setEnabled(true);
}

bool MainWindow::getAutoupakovka() const
{
    return autoupakovka;
}

void MainWindow::setAutoupakovka(bool value)
{
    autoupakovka = value;

    ui->AutoUpakovkaGroupBox->setEnabled(!autoupakovka);
}

QString MainWindow::getGuiTNVED()
{
    return ui->TNVEDVal->toPlainText();
}

QString MainWindow::getGuiDrugsName()
{
    return ui->DrugsComboBox->currentText();
}

QString MainWindow::getGuiDose()
{
    return ui->DoseVal->toPlainText();
}

void MainWindow::SetSN(QString newSN)
{
    SN = newSN;
}

bool MainWindow::getAgregation()
{
    return agregation;
}

void MainWindow::addMessageToJournal(QString message, QColor textcolor,QColor backcolor)
{
    QString ctime  = QDateTime::currentDateTime().toTimeSpec(Qt::LocalTime).toString("hh:mm:ss dd.MM.yy ");
    QListWidgetItem * item = new QListWidgetItem ();
    item->setTextColor( textcolor);
    item->setBackgroundColor(backcolor);
    item->setText(ctime + message);
    ui->journalList->addItem(item);
    ui->journalList->scrollToBottom();
}

void MainWindow::updateTimeDate()
{
    ui->DateTimeLabelValue->setText(QDateTime::currentDateTime().toTimeSpec(Qt::LocalTime).toString("hh-mm-ss dd-MM-yyyy"));
}

void MainWindow::AddHandScannerLOG()
{
    QElapsedTimer timer;
    timer.start();
    QString req = QString("INSERT INTO \"scannerlog\" (\"date\",\"Message\") VALUES ('%1','%2')").arg(QDateTime::currentDateTime().toTimeSpec(Qt::LocalTime).toString("hh-mm-ss dd-MM-yyyy"),inputDataStringFromScaner); //.remove(30,10)
    qDebug() << req << "req";
    sqlDB->makesqlreq(req );
    qDebug() << "AddHand scannerlog took" << timer.elapsed() << "milliseconds";
}

bool MainWindow::getRunningBuisenessProcess() const
{
    return runningBuisenessProcess;
}

void MainWindow::setRunningBuisenessProcess(bool value)
{
    runningBuisenessProcess = value;
}

bool MainWindow::getLanguageswitcher() const
{
    return languageswitcher;
}

void MainWindow::setLanguageswitcher(bool value)
{
    languageswitcher = value;
    qDebug() << "languageswitcher =" << value;
}

void MainWindow::Getmessage(QString m)
{
}

void MainWindow::PrintSSCCCode(QString newcode)
{
    m_Barcode = new Code128Item();
    m_Barcode->setWidth( 190 / 1.5 );
    m_Barcode->setHeight( 110 / 1.3 );
    m_Barcode->setText(newcode);
    m_Scene.clear();
    m_Scene.addItem( m_Barcode );
    m_Scene.update();
    m_Barcode->update();

    QPrinter printer;
    QSize size(30,40);

    printer.setPageSizeMM(size);
    QPainter painter(&printer);
    painter.setRenderHint(QPainter::Antialiasing);
    m_Scene.render(&painter);
}

bool MainWindow::PrintBIGEtiketka(eticetka * et)
{
    QPrinter printer;
    QSize size(100,150);

    printer.setPageSizeMM(size);
    QPainter painter(&printer);
    painter.setRenderHint(QPainter::Antialiasing);
    et->all_etiketka.render(&painter);

    return true;
}

void MainWindow::updateReadedDMCode()
{
    if (inputDataStringFromScaner!="")
    {

        qDebug() << inputDataStringFromScaner;

        QString sss = "002#9";

        inputDataStringFromScaner.replace(sss," ");
        if (getLanguageswitcher() == true)
        {
            // Просто меняем раскладку если у нас агрегация.
            // так как Ручной сканер работает как клавиатура, то он эмулирует нажатие клавиш, что при русской раскладке дает неверные символы

            inputDataStringFromScaner.replace("Й","Q");
            inputDataStringFromScaner.replace("Ц","W");
            inputDataStringFromScaner.replace("У","E");
            inputDataStringFromScaner.replace("К","R");
            inputDataStringFromScaner.replace("Е","T");
            inputDataStringFromScaner.replace("Н","Y");
            inputDataStringFromScaner.replace("Г","U");
            inputDataStringFromScaner.replace("Ш","I");
            inputDataStringFromScaner.replace("Щ","O");
            inputDataStringFromScaner.replace("З","P");
            inputDataStringFromScaner.replace("Х","[");
            inputDataStringFromScaner.replace("Ъ","]");

            inputDataStringFromScaner.replace("Ф","A");
            inputDataStringFromScaner.replace("Ы","S");
            inputDataStringFromScaner.replace("В","D");
            inputDataStringFromScaner.replace("А","F");
            inputDataStringFromScaner.replace("П","G");
            inputDataStringFromScaner.replace("Р","H");
            inputDataStringFromScaner.replace("О","J");
            inputDataStringFromScaner.replace("Л","K");
            inputDataStringFromScaner.replace("Д","L");
            inputDataStringFromScaner.replace("Ж",";");
            inputDataStringFromScaner.replace("Э","'");

            inputDataStringFromScaner.replace("Я","Z");
            inputDataStringFromScaner.replace("Ч","X");
            inputDataStringFromScaner.replace("С","C");
            inputDataStringFromScaner.replace("М","V");
            inputDataStringFromScaner.replace("И","B");
            inputDataStringFromScaner.replace("Т","N");
            inputDataStringFromScaner.replace("Ь","M");
            inputDataStringFromScaner.replace("Б",",");
            inputDataStringFromScaner.replace("Ю",".");
            inputDataStringFromScaner.replace(".","/");

            // если зажался шифт в Англ. раскладке
            inputDataStringFromScaner.replace("!","1");
            inputDataStringFromScaner.replace("@","2");
            inputDataStringFromScaner.replace("#","3");
            inputDataStringFromScaner.replace("$","4");
            inputDataStringFromScaner.replace("%","5");
            inputDataStringFromScaner.replace("^","6");
            inputDataStringFromScaner.replace("&","7");
            inputDataStringFromScaner.replace("*","8");
            inputDataStringFromScaner.replace("(","9");
            inputDataStringFromScaner.replace(")","0");

            // если зажался шифт в Русской раскладке
            inputDataStringFromScaner.replace("!","1");
            inputDataStringFromScaner.replace("\"","2");
            inputDataStringFromScaner.replace("№","3");
            inputDataStringFromScaner.replace(";","4");
            inputDataStringFromScaner.replace("%","5");
            inputDataStringFromScaner.replace(":","6");
            inputDataStringFromScaner.replace("?","7");
            inputDataStringFromScaner.replace("*","8");
            inputDataStringFromScaner.replace("(","9");
            inputDataStringFromScaner.replace(")","0");
        }

        ParseHandScannerData(inputDataStringFromScaner);
        DMCodeUpdateTimeoutTimer->stop();
        inputDataStringFromScaner.clear();
    }
}

QString MainWindow::GetRegularString(QString stringforparse, QString SNRegularexpression)
{
    QString matched ;
    QRegularExpression re(SNRegularexpression );
    QRegularExpressionMatch match = re.match(stringforparse);
    if (match.hasMatch()) {
        matched = match.captured(0);
    }
    return matched;
}

QString MainWindow::GetISODate()
{
    QString ISOdate;
    // формат <operation_date>2017-10-07T15:00:00+05:00</operation_date>
    ISOdate = QDateTime::currentDateTime().toOffsetFromUtc(QDateTime::currentDateTime().offsetFromUtc()).toString(Qt::ISODate);
    return ISOdate;
}

QDateTime MainWindow::GetISODateTime()
{
    QDateTime ISOdate;
    ISOdate = QDateTime::currentDateTime().toOffsetFromUtc(QDateTime::currentDateTime().offsetFromUtc());
    return ISOdate;
}


QString MainWindow::GetDOCDate()
{
    QString DOCdate;
    // формат <doc_date>07.10.2017</doc_date>
    DOCdate = QDateTime::currentDateTime().toTimeSpec(Qt::LocalTime).toString("dd.MM.yyyy");
    return DOCdate;
}

void MainWindow::CreateXML313Doc( QList<medicament *> MedList, QDateTime operation_date)
{
    qDebug() << "CreateXML313Doc" ;
    setRunningBuisenessProcess(false);
    setLanguageswitcher(false);

    manufacturer *organization = getSerializationCompanySender();

    //  если пустой список то и генерировать то нечего
    if (MedList.length() == 0)
        return;

    QDomDocument document;
    QDomElement root = document.createElement("documents");
    document.appendChild(root);

    QDomElement reg_prod_emis_elem  = document.createElement("register_product_emission");
    reg_prod_emis_elem.setAttribute("action_id", "313");
    root.appendChild(reg_prod_emis_elem);

    // добавляем subject_id

    QDomElement subjectIDelement  = document.createElement("subject_id");
    reg_prod_emis_elem.appendChild(subjectIDelement);

    QDomText subjectIDtext  = document.createTextNode("subject_id");
    subjectIDtext.setNodeValue(organization->get_subject_id());
    subjectIDelement.appendChild(subjectIDtext);

    // добавили subject_id

    // добавляем operation_date

    QDomElement operationdateelement  = document.createElement("operation_date");
    reg_prod_emis_elem.appendChild(operationdateelement);

    QDomText operationdatetext  = document.createTextNode("operation_date"); // operation_date");

    operationdatetext.setNodeValue(operation_date.toOffsetFromUtc(QDateTime::currentDateTime().offsetFromUtc()).toString(Qt::ISODate) );

    operationdateelement.appendChild(operationdatetext);

    // добавили operation_date


    // добавляем confirm_doc

    QDomElement confirm_doc_lement  = document.createElement("confirm_doc");
    reg_prod_emis_elem.appendChild(confirm_doc_lement);

    QDomText confirm_doc_text  = document.createTextNode("confirm_doc");
    confirm_doc_text.setNodeValue("1");
    confirm_doc_lement.appendChild(confirm_doc_text);

    // добавили confirm_doc


    // добавляем doc_num

    QDomElement doc_num_element  = document.createElement("doc_num");
    reg_prod_emis_elem.appendChild(doc_num_element);

    QDomText doc_num_text  = document.createTextNode("doc_num");
    doc_num_text.setNodeValue("ds1");
    doc_num_element.appendChild(doc_num_text);

    // добавили doc_num

    // добавляем doc_num

    QDomElement doc_date_element  = document.createElement("doc_date");
    reg_prod_emis_elem.appendChild(doc_date_element);

    QDomText doc_date_text  = document.createTextNode("doc_date");
    doc_date_text.setNodeValue(GetDOCDate());
    doc_date_element.appendChild(doc_date_text);

    // добавили doc_num
    // добавляем signs (для первичной агрегации это GTINs

    QDomElement signs_element  = document.createElement("signs");
    reg_prod_emis_elem.appendChild(signs_element);

    // следуя документу, sgtin  - Индивидуальный серийный номер вторичной упаковки, то есть серийный номер (который генерируется)
    // добавляем sgtin

    QDomElement sgtin_element ;
    QDomText sgtin_text ;
    // добавили doc_num

    for (int var = 0; var < MedList.length(); ++var) {
        sgtin_element = document.createElement("sgtin");
        signs_element.appendChild(sgtin_element);
        sgtin_text  = document.createTextNode("sgtintext"); // operation_date");
        sgtin_text.setNodeValue(MedList.at(var)->sGTIN);
        sgtin_element.appendChild(sgtin_text);
    }

    // добавили signs

    QString filename = MedList.at(0)->BatchNumber+"_P313_" + QDateTime::currentDateTime().toTimeSpec(Qt::LocalTime).toString("hh-mm-ss dd-MM-yy") + ".xml";
    QString filepath = QDir::currentPath()   + "/" + filename ;
    QFile file(filepath);

    if ( !file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        addMessageToJournal("Не удалось создать файл:" + filepath, Qt::black, Qt::white);
        return;
    }

    QTextStream stream(&file);
    stream<< document.toString();
    file.close();

    addMessageToJournal("Создан файл:" + filepath, Qt::black, Qt::white);
    AddStatisticsToDB("313",MedList.at(0),QDateTime::currentDateTime(), MedList.length(),filename);
}

void MainWindow::CreateXML415Doc(QList<medicament *> MedList, manufacturer *companyreciver, manufacturer *companysender, QDateTime operation_date, QString DocNum, QDate DocDate, int turnovertype, int source, int contracttype , QString Price, QString Vat)
{
    setRunningBuisenessProcess(false);
    setLanguageswitcher(false);

    if (MedList.length() <=0)
        return ;

    QDomDocument document;
    QDomElement root = document.createElement("documents");
    document.appendChild(root);

    QDomElement move_order_elem  = document.createElement("move_order");
    move_order_elem.setAttribute("action_id", "415");
    root.appendChild(move_order_elem);

    // добавляем subject_id
    addXMLTextNode(move_order_elem,  companysender->get_subject_id() , "subject_id", document);
    // добавили subject_id

    // добавляем receiver_id
    addXMLTextNode(move_order_elem,  companyreciver->get_subject_id() , "receiver_id", document);
    // добавили receiver_id

    // добавляем operation_date
    addXMLTextNode(move_order_elem,  operation_date.toString((Qt::ISODate)) , "operation_date", document);
    // добавили operation_date

    // добавляем doc_num
    addXMLTextNode(move_order_elem, DocNum , "doc_num", document);
    // добавили doc_num

    // добавляем doc_date
    addXMLTextNode(move_order_elem, DocDate.toString("dd.MM.yyyy") , "doc_date", document);
    // добавили doc_date

    // добавляем turnover_type
    addXMLTextNode(move_order_elem, QString::number(turnovertype) , "turnover_type", document);
    // добавили turnover_type

    if (source!=-1) // если у нас регламентируется источник
    {
        // добавляем source
        addXMLTextNode(move_order_elem, QString::number(source) , "source", document);
        // добавили source
    }

    // добавляем contract_type
    addXMLTextNode(move_order_elem, QString::number(contracttype), "contract_type", document);
    // добавили contract_type

    QDomElement order_details_element  = document.createElement("order_details");
    move_order_elem.appendChild(order_details_element);


    //    // следуя документу, sgtin  - Индивидуальный серийный номер вторичной упаковки, то есть серийный номер (который генерируется)
    //    // добавляем sgtin

    for (int var = 0; var < MedList.length(); ++var) {
        QDomElement union_element  = document.createElement("union");
        order_details_element.appendChild(union_element);

        // добавляем sgtin в union
        addXMLTextNode(union_element,  MedList.at(var)->sGTIN , "sgtin", document);
        // добавили sgtin в union

        // добавляем cost в union
        addXMLTextNode(union_element,  Price, "cost", document);
        // добавили cost в union

        // добавляем vat_value в union
        addXMLTextNode(union_element,  Vat, "vat_value", document);
        // добавили vat_value в union
    }

    // добавили signs

    QString filename = MedList.at(0)->BatchNumber+"_P415_" + QDateTime::currentDateTime().toTimeSpec(Qt::LocalTime).toString("hh-mm-ss dd-MM-yy") + ".xml";
    QString filepath = QDir::currentPath()   + "/" + filename ;
    QFile file(filepath);

    if ( !file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        addMessageToJournal("Не удалось создать файл:" + filepath, Qt::black, Qt::white);
        return;
    }

    QTextStream stream(&file);
    stream<< document.toString();
    file.close();

    addMessageToJournal("Создан файл: " + filepath, Qt::black, Qt::white);
    AddStatisticsToDB("415",MedList.at(0),QDateTime::currentDateTime(), MedList.length(),filename);
}

void MainWindow::CreateXML811Doc(QList<medicament *> MedListOld, QList<medicament *> MedListNew, manufacturer *company_subject, QDateTime operation_date)
{
    setRunningBuisenessProcess(false);
    setLanguageswitcher(false);

    if (MedListOld.length() <=0)
        return ;

    if (MedListNew.length() <=0)
        return ;

    if (MedListNew.length() != MedListOld.length())
        return ;

    QDomDocument document;
    QDomElement root = document.createElement("documents");
    document.appendChild(root);

    QDomElement relabeling_elem  = document.createElement("relabeling");
    relabeling_elem.setAttribute("action_id", "811");
    root.appendChild(relabeling_elem);

    //добавляем subject_id
    addXMLTextNode(relabeling_elem,  company_subject->get_subject_id() , "subject_id", document);
    //добавили subject_id

    // добавляем operation_date
    // addXMLTextNode(unit_pack_elem,  operation_date.toString(Qt::ISODate) , "operation_date", document);
    addXMLTextNode(relabeling_elem,  GetISODate(), "operation_date", document);
    // добавили operation_date


    QDomElement relabeling_detail_element  = document.createElement("relabeling_detail");
    relabeling_elem.appendChild(relabeling_detail_element);

    // следуя документу, sgtin  - Индивидуальный серийный номер вторичной упаковки, то есть серийный номер (который генерируется)
    // добавляем sgtin

    for (int var = 0; var < MedListOld.length(); ++var) {

        QDomElement detail_element  = document.createElement("detail");
        relabeling_elem.appendChild(detail_element);

        // добавляем new_sgtin в detail
        addXMLTextNode(detail_element,  MedListNew.at(var)->sGTIN , "new_sgtin", document);
        // добавили new_sgtin в detail

        // добавляем old_sgtin в detail
        addXMLTextNode(detail_element,  MedListOld.at(var)->sGTIN , "old_sgtin", document);
        // добавили old_sgtin в detail
    }

    //добавили signs

    QString filename = MedListNew.at(0)->BatchNumber+"_P811_" + QDateTime::currentDateTime().toTimeSpec(Qt::LocalTime).toString("hh-mm-ss dd-MM-yy") + ".xml";
    QString filepath = QDir::currentPath() + "/" + filename ;
    QFile file(filepath);

    if ( !file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        addMessageToJournal("Не удалось создать файл:" + filepath, Qt::black, Qt::white);
        return;
    }

    QTextStream stream(&file);
    stream<< document.toString();
    file.close();

    addMessageToJournal("Создан файл: " + filepath, Qt::black, Qt::white);
    AddStatisticsToDB("811",MedListNew.at(0),QDateTime::currentDateTime(), MedListNew.length(),filename);
}

void MainWindow::CreateXML911Doc(QList<medicament *> MedList, manufacturer *companysender, QDateTime operation_date)
{
    setRunningBuisenessProcess(false);
    setLanguageswitcher(false);

    if (MedList.length() <=0)
    {
        return ;
    }

    QDomDocument document;
    QDomElement root = document.createElement("documents");
    document.appendChild(root);

    QDomElement unit_pack_elem  = document.createElement("unit_pack");
    unit_pack_elem.setAttribute("action_id", "911");
    root.appendChild(unit_pack_elem);

    //добавляем subject_id
    addXMLTextNode(unit_pack_elem,  companysender->get_subject_id() , "subject_id", document);
    //добавили subject_id

    //QString SSCCCode128 ="(00) " + companysender->getGS1id() + " " +  generateCode128(8);
    QString SSCCCode128 = "00" +  companysender->getGS1id() +  generateCode128(8);
    QString CorrectedDate = "20" + MedList.at(0)->ExperyDate;
    QDate ExperyDate = QDate::fromString(CorrectedDate,"yyyyMMdd");


    QString where1 = QString ( "drugs_name = '%1' " ).arg(MedList.at(0)->medicament_name);
    QString dose = sqlDB->sel("Dose", "drugs", where1,"Dose").at(0);
    QString conditions = sqlDB->sel("conditions", "drugs", where1,"conditions").at(0);
    QString quantity = sqlDB->sel("quantity", "drugs", where1,"quantity").at(0);
    QString address = "623704, Свердловская область, г. Березовский, ул. Кольцевая, 13а";

    EticetkaBFZ = new eticetka(companysender->get_organisation_name(),dose,address,MedList.at(0)->medicament_name,MedList.length(),MedList.at(0)->GTIN,MedList.at(0)->BatchNumber,operation_date.toTimeSpec(Qt::LocalTime).toString("dd.MM.yyyy"),MedList.at(0)->ExperyDate,conditions,"0000",SSCCCode128 );

    if ( PrintBIGEtiketka(EticetkaBFZ)!= true)
    {
        return;
    }

    // добавляем subject_id
    addXMLTextNode(unit_pack_elem,  SSCCCode128 , "sscc", document);
    // добавили subject_id

    // добавляем operation_date
    // addXMLTextNode(unit_pack_elem,  operation_date.toString(Qt::ISODate) , "operation_date", document);
    addXMLTextNode(unit_pack_elem,  GetISODate(), "operation_date", document);
    // добавили operation_date

    QDomElement signs_element  = document.createElement("signs");
    unit_pack_elem.appendChild(signs_element);

    // следуя документу, sgtin  - Индивидуальный серийный номер вторичной упаковки, то есть серийный номер (который генерируется)
    // добавляем sgtin

    for (int var = 0; var < MedList.length(); ++var) {

        addXMLTextNode(signs_element, MedList.at(var)->sGTIN, "sgtin", document);

    }
    // добавили signs

    QString filename = MedList.at(0)->BatchNumber+"_P911_" + QDateTime::currentDateTime().toTimeSpec(Qt::LocalTime).toString("hh-mm-ss dd-MM-yy") + ".xml";
    QString filepath = QDir::currentPath()   + "/" + filename ;
    QFile file(filepath);

    if ( !file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        addMessageToJournal("Не удалось создать файл:" + filepath, Qt::black, Qt::white);
        return;
    }

    QTextStream stream(&file);
    stream<< document.toString();
    file.close();

    addMessageToJournal("Создан файл: " + filepath, Qt::black, Qt::white);
    AddStatisticsToDB("911",MedList.at(0),QDateTime::currentDateTime(), MedList.length(),filename);
}

void MainWindow::CreateXML312Doc( QList<medicament *> MedList, quint8 controlsamplestype)
{

    setRunningBuisenessProcess(false);
    setLanguageswitcher(false);

    manufacturer *organization = getSerializationCompanySender();

    //  если пустой список то и генерировать то нечего
    if (MedList.length() == 0)
        return;

    QDomDocument document;
    QDomElement root = document.createElement("documents");
    document.appendChild(root);

    QDomElement reg_ctrl_smples_elem  = document.createElement("register_control_samples");
    reg_ctrl_smples_elem.setAttribute("action_id", "313");
    root.appendChild(reg_ctrl_smples_elem);

    // добавляем subject_id

    QDomElement subjectIDelement  = document.createElement("subject_id");
    reg_ctrl_smples_elem.appendChild(subjectIDelement);

    QDomText subjectIDtext  = document.createTextNode("subject_id");
    subjectIDtext.setNodeValue(organization->get_subject_id());
    subjectIDelement.appendChild(subjectIDtext);

    // добавили subject_id

    // добавляем operation_date

    QDomElement operationdateelement  = document.createElement("operation_date");
    reg_ctrl_smples_elem.appendChild(operationdateelement);

    QDomText operationdatetext  = document.createTextNode("operation_date"); // operation_date");

    operationdatetext.setNodeValue( GetISODate());
    operationdateelement.appendChild(operationdatetext);

    // добавили operation_date

    // добавляем control_samples_type

    QDomElement control_samples_type_element  = document.createElement("control_samples_type");
    reg_ctrl_smples_elem.appendChild(control_samples_type_element);

    QDomText control_samples_type_text  = document.createTextNode("control_samples_type");

    control_samples_type_text.setNodeValue( QString::number(controlsamplestype) );
    control_samples_type_element.appendChild(control_samples_type_text);

    // добавили control_samples_type


    // добавляем signs (для первичной агрегации это GTINs

    QDomElement signs_element  = document.createElement("signs");
    reg_ctrl_smples_elem.appendChild(signs_element);


    // следуя документу, sgtin  - Индивидуальный серийный номер вторичной упаковки, то есть серийный номер (который генерируется)
    // добавляем sgtin

    QDomElement sgtin_element ;
    QDomText sgtin_text ;

    // добавили doc_num

    for (int var = 0; var < MedList.length(); ++var) {

        sgtin_element = document.createElement("sgtin");
        signs_element.appendChild(sgtin_element);

        sgtin_text  = document.createTextNode("sgtintext");
        sgtin_text.setNodeValue(MedList.at(var)->sGTIN);
        sgtin_element.appendChild(sgtin_text);
    }

    // добавили signs

    QString filename = MedList.at(0)->BatchNumber+"_P312_" + QDateTime::currentDateTime().toTimeSpec(Qt::LocalTime).toString("hh-mm-ss dd-MM-yy") + ".xml";
    QString filepath = QDir::currentPath()   + "/" + filename ;
    QFile file(filepath);

    if ( !file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        addMessageToJournal("Не удалось создать файл:" + filepath, Qt::black, Qt::white);
        return;
    }

    QTextStream stream(&file);
    stream<< document.toString();
    file.close();

    addMessageToJournal("Создан файл: " + filepath, Qt::black, Qt::white);
    AddStatisticsToDB("312",MedList.at(0),QDateTime::currentDateTime(), MedList.length(),filename);
}

void MainWindow::addXMLTextNode(QDomElement reg_end_pack_elem, QString nodevalue, QString nodename, QDomDocument document)
{
    QDomElement q_dom_elem  = document.createElement(nodename);
    reg_end_pack_elem.appendChild(q_dom_elem);
    QDomText gtin_text  = document.createTextNode(nodename);
    gtin_text.setNodeValue(nodevalue );
    q_dom_elem.appendChild(gtin_text);
}

void MainWindow::CreateXML311Doc(QList<medicament *> MedList, manufacturer * sender, manufacturer * owner,  int ordertype, QDateTime operation_date)
{
    setRunningBuisenessProcess(false);
    setLanguageswitcher(false);

    if (MedList.length() <=0)
        return ;

    QDomDocument document;
    QDomElement root = document.createElement("documents");
    document.appendChild(root);

    QDomElement reg_end_pack_elem  = document.createElement("register_end_packing");
    reg_end_pack_elem.setAttribute("action_id", "311");
    root.appendChild(reg_end_pack_elem);

    // добавляем subject_id
    addXMLTextNode(reg_end_pack_elem,  sender->get_subject_id() , "subject_id", document);
    // добавили subject_id


    // добавляем operation_date
    addXMLTextNode(reg_end_pack_elem,  operation_date.toString(Qt::ISODate) , "operation_date", document);
    // добавили operation_date

    // добавляем order_type
    addXMLTextNode(reg_end_pack_elem,  QString::number(ordertype) , "order_type", document);
    // добавили order_type

    // если у нас контрактное производство то мы вводим идентификатор собственника
    if (ordertype == 2)
    {
        // добавляем owner_id
        //        addXMLTextNode(reg_end_pack_elem, owner->get_owner_id(), "owner_id", document);
        addXMLTextNode(reg_end_pack_elem, owner->get_subject_id(),"owner_id", document);
        // добавили owner_id
    }


    // добавляем series_number - номер производственной серии (не серийник потребит.упак. а именно партия)
    addXMLTextNode(reg_end_pack_elem, MedList.at(0)->BatchNumber, "series_number", document);
    // добавили series_number


    // добавляем expiration_date - срок годности препарата
    addXMLTextNode(reg_end_pack_elem, MedList.at(0)->ExperyDate, "expiration_date", document);
    // добавили expiration_date

    // добавляем gtin - срок годности препарата
    addXMLTextNode(reg_end_pack_elem, MedList.at(0)->GTIN, "gtin", document);
    // добавили  gtin

    // добавляем tnved_code
    addXMLTextNode(reg_end_pack_elem, MedList.at(0)->TNVED, "tnved_code", document);
    // добавили  tnved_code

    // добавляем signs (для первичной агрегации это sGTINs) а для вторичной это SSCC
    QDomElement signs_element  = document.createElement("signs");
    reg_end_pack_elem.appendChild(signs_element);
    // следуя документу, sgtin  - Индивидуальный серийный номер вторичной упаковки, то есть серийный номер (который генерируется)
    // добавляем sgtin

    for (int var = 0; var < MedList.length(); ++var) {
        addXMLTextNode(signs_element, MedList.at(var)->sGTIN, "sgtin", document);
    }

    // добавили signs

    QString filename = MedList.at(0)->BatchNumber+"_P311_" + QDateTime::currentDateTime().toTimeSpec(Qt::LocalTime).toString("hh-mm-ss dd-MM-yy") + ".xml";
    QString filepath = QDir::currentPath()   + "/" + filename ;


    QFile file(filepath);

    if ( !file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        addMessageToJournal("Не удалось создать файл:" + filepath, Qt::black, Qt::white);
        return;
    }

    QTextStream stream(&file);
    stream<< document.toString();
    file.close();

    addMessageToJournal("Создан файл: " + filepath, Qt::black, Qt::white);
    AddStatisticsToDB("311",MedList.at(0),QDateTime::currentDateTime(), MedList.length(),filename);
}

void MainWindow::StartAgregation()
{
    setLanguageswitcher(true);
    setRunningBuisenessProcess(true);
}

void MainWindow::StopAgregation()
{
    setLanguageswitcher(false);
    setRunningBuisenessProcess(false);
}

void MainWindow::ParseHandScannerData(QString stringforparse)
{
    // сначала проверяем ня соответствие QR кодам
    qDebug() << stringforparse;


    if (stringforparse == register_product_emission_QR_string)
    {
        RegisterProductEmissionPageOpen();
        return;
    }

    if (stringforparse == register_control_samples_QR_string)
    {
        RegisterControlSamplesPageOpen();
        return;
    }

    if (stringforparse == register_end_packing_QR_string)
    {
        RegisterEndPackingPageOpen();
        return;
    }


    if (stringforparse == unit_pack_QR_string)
    {
        UnitPackPageOpen();
        return;
    }

    if (stringforparse == move_order_QR_string)
    {
        MoveOrderPageOpen();
        return;
    }


    if (stringforparse == releabeling_QR_string)
    {
        ReleabilingPageOpen();
        return;
    }







    if (stringforparse == printControlQRCode)
    {
        emit printControlQRCodeScanned();
        return;
    }

    if (stringforparse == programOptionsQRCode)
    {
        emit programOptionsQRCodeScanned();
        return;
    }

    if (stringforparse == statisticsQRCode)
    {
        emit statisticsQRCodeScanned();
        return;
    }

    if (stringforparse == agregationQRCode)
    {
        emit agregationQRCodeScanned();
        return;
    }

    if (getRunningBuisenessProcess() == false)
    {
        if (stringforparse == Start311ProcessQRString)
        {
            StartAgregation();
            emit Start311Process();
            return;
        }

        if (stringforparse == Start312ProcessQRString)
        {
            StartAgregation();
            emit Start312Process();
            return;
        }

        if (stringforparse == Start313ProcessQRString)
        {
            StartAgregation();
            emit Start313Process();
            Start313Process(true);
            return;
        }
    }

    if (stringforparse == Stop311ProcessQRString)
    {
        StopAgregation();
        emit Stop311Process();
        return;
    }

    if (stringforparse == Stop312ProcessQRString)
    {
        StopAgregation();
        emit Stop312Process();
        return;
    }

    if (stringforparse == Stop313ProcessQRString)
    {
        StopAgregation();
        Start313Process(0);
        emit Stop313Process();
        return;
    }


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
        //удаляем GTIN из общей строки
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
    tnvedstring.remove(0,4);
    tnvedstring.replace(GSSymbol,Emptystring);

    if(tnvedstring == NULL)
    {
        tnvedstring = NotFoundString;
    }

    // кончаем разбирать ТНВЭД


    // начинаем разбирать Срок Годности

    expstring = GetRegularString(stringforparse, ExpRegularexpression);
    expstring.remove(0,3);
    expstring.replace(GSSymbol,Emptystring);
    if(expstring == NULL)
    {
        expstring = NotFoundString;
    }

    // если прочитали неверную дату годности
    //    if( IsDateProper(expstring) == false)
    //    {
    //        expstring = NotFoundString;
    //    }


    // кончаем разбирать Срок Годности

    // начинаем разбирать Партию

    batchstring = GetRegularString(stringforparse, BatchRegularexpression);
    batchstring.remove(0,3);
    batchstring.replace(GSSymbol,Emptystring);

    if (batchstring == NULL)
    {
        batchstring = NotFoundString;
    }

    sGTINString = gtinstring + SNstring;

    // кончаем разбирать Партию

    QString medicamentName;
    QString whereGtin = QString("gtin = '%1'  ").arg(gtinstring);
    medicamentName = sqlDB->sel("drugs_name", "drugs", whereGtin,"drugs_name").at(0);
    if (medicamentName == "")
    {
        medicamentName = "No drug in DB found";
    }
    else
    {
        ScannedMedicament = new medicament(medicamentName,gtinstring,SNstring,batchstring,expstring,sGTINString,tnvedstring);
        emit SendMedicamentSignal(ScannedMedicament);
    }

    emit ParcingEnded(); // испускаем сигнал что закончили парсинг строки
}

void MainWindow::updateDMPicture()
{
    int val = GenerateNumber(3, 1);
    imageObject->load(QDir::currentPath() + "/DM" + QString::number(val) + ".JPG");
    image = QPixmap::fromImage(*imageObject);
    //    ui->packagePicture->setScene(scene);
    scene->addPixmap(image);
    scene->setSceneRect(image.rect());
    //    ui->packagePicture->show();

}

void MainWindow::EmulateAutomaticMedicamentScan()
{
    QString SNNN = generateSN(11);
    ScannedMedicament = new medicament(getGuiDrugsName(),getGuiGTIN(),SNNN,getGuiBatchValue(),getGuiExpery(),getGuiGTIN()+SNNN,getGuiTNVED());
    emit SendMedicamentSignal(ScannedMedicament);
    //ScannedMedicament->deleteLater();
}

void MainWindow::Start313Process(bool set)
{
    if (set == true) // если запускаем агрегацию
    {
        setRunningBuisenessProcess(true);
        setLanguageswitcher(true);
        inputDataStringFromScaner.clear();
    }
    else    // если закончили агрегацию
    {
        setRunningBuisenessProcess(false);
        setLanguageswitcher(false);
        CreateXML313Doc(MedicamentsList,GetISODateTime());
        inputDataStringFromScaner.clear();
    }

    MedicamentsList.clear();
    agregation = set;
    emit agregationstatusToggled();
}

bool MainWindow::getBSerializationPaused() const
{
    return bSerializationPaused;
}

void MainWindow::setBSerializationPaused(bool value)
{
    bSerializationPaused = value;
}

bool MainWindow::getBSerializationStopped() const
{
    return bSerializationStopped;
}

void MainWindow::setBSerializationStopped(bool value)
{
    bSerializationStopped = value;
}

bool MainWindow::getBSerializationStarted() const
{
    return bSerializationStarted;
}

void MainWindow::setBSerializationStarted(bool value)
{
    bSerializationStarted = value;
}

manufacturer *MainWindow::getSerializationCompanyOwner() const
{
    return SerializationCompanyOwner;
}

void MainWindow::setSerializationCompanyOwner(manufacturer *value)
{
    SerializationCompanyOwner = value;
}

int MainWindow::getSerializationOrderType() const
{
    int SerializationOrderType = ui->OrderTypeCombo->currentIndex();
    return SerializationOrderType;
}

void MainWindow::setSerializationOrderType(int value)
{
    SerializationOrderType = value;
}

QString MainWindow::getSerializationBatchValue() const
{
    return SerializationBatchValue;
}

void MainWindow::setSerializationBatchValue(const QString &value)
{
    SerializationBatchValue = value;
}

QString MainWindow::getSerializationQuantity() const
{
    return SerializationQuantity;
}

void MainWindow::setSerializationQuantity(const QString &value)
{
    SerializationQuantity = value;
}

QString MainWindow::getSerializationTNVED() const
{
    return SerializationTNVED;
}

void MainWindow::setSerializationTNVED(const QString &value)
{
    SerializationTNVED = value;
}

QString MainWindow::getSerializationDose() const
{
    return SerializationDose;
}

void MainWindow::setSerializationDose(const QString &value)
{
    SerializationDose = value;
}

QString MainWindow::getSerializationDrugName() const
{
    return SerializationDrugName;
}

void MainWindow::setSerializationDrugName(const QString &value)
{
    SerializationDrugName = value;
}

QString MainWindow::getSerializationExpery() const
{
    return SerializationExpery;
}

void MainWindow::setSerializationExpery(const QString &value)
{
    SerializationExpery = value;
}

QString MainWindow::getSerializationBatchName() const
{
    return SerializationBatchName;
}

void MainWindow::setSerializationBatchName(const QString &value)
{
    SerializationBatchName = value;
}

QString MainWindow::getSerializationSN() const
{
    return SerializationSN;
}

void MainWindow::setSerializationSN(const QString &value)
{
    SerializationSN = value;
}

QString MainWindow::getSerializationGTIN() const
{
    return SerializationGTIN;
}

void MainWindow::setSerializationGTIN(const QString &value)
{
    SerializationGTIN = value;
}

manufacturer *MainWindow::getSerializationCompanySender() const
{
    return SerializationCompanySender;
}

void MainWindow::SetSerializationCompanySender(manufacturer *value)
{
    SerializationCompanySender = value;
}

void MainWindow::Toggle313Process()
{
    if(getAgregation() == false)
    {

        Start313Process(true);
    }
    else
    {
        Start313Process(false);
    }
}

void MainWindow::updateAgregationGUI()
{

}

bool MainWindow::CheckMedicamentinDB(medicament *m)
{
    QString where = QString ( "drugs_name = '%1' " ).arg(m->SerialNumber);

    if (sqlDB->sel("tnved", "drugs", where,"tnved").at(0) != "")
    {

    }
    return false;
}

QString MainWindow::GenerateDMcode()
{
    QString DMCode = GTINid + getGuiGTIN() + SNid  +generateSN(SNlenght) + Batchid + getGuiBatchNumber() +  Experyid + getGuiExpery() + TNVEDid  + getGuiTNVED();
    updateQRImage();
    return DMCode;
}

void MainWindow::RegisterProductEmissionPageOpen()
{
    setStackedPage(8);
}

void MainWindow::RegisterControlSamplesPageOpen()
{
    setStackedPage(6);
}

void MainWindow::RegisterEndPackingPageOpen()
{
    setStackedPage(7);
}

void MainWindow::PrintControlPageOpen()
{
    setStackedPage(0);
}

void MainWindow::productOptionsPageOpen()
{
    setStackedPage(1);
}

void MainWindow::agregationOptionsPageOpen()
{
    setStackedPage(2);
}

void MainWindow::statisticsPageOpen()
{
    setStackedPage(3);
}

void MainWindow::setStackedPage(int newindex)
{
    ui->stackedWidget->setCurrentIndex(newindex);
    qDebug() << newindex;
}

void MainWindow::SendCommandToVideoJet(QString a)
{
    return;
    serverWrite(a);
    a ="\r"; // посылаем знак завершения посылки.
    //    serverWrite(a);
}

void MainWindow::updateQRImage()
{
    int levelIndex = 1;
    int versionIndex = 0;
    bool bExtent = true;
    int maskIndex = -1;
    QString encodeString = generateSN(11);;

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
}

QImage MainWindow::QRCodeToQLabelConverter(QLabel* labelq, QString textcode, int scale ,  int versionIndex, int levelIndex, bool bExtent, int maskIndex)
{
    successfulEncoding = qrEncode.EncodeData( levelIndex, versionIndex, bExtent, maskIndex, textcode.toUtf8().data() );
    if ( !successfulEncoding )
    {
        ui->register_control_samples_Label->clear();
        ui->register_control_samples_Label->setText( tr("QR Code...") );
    }

    int qrImageSize = qrEncode.m_nSymbleSize;

    // Создаем двумерный образ кода

    encodeImageSize = qrImageSize + ( QR_MARGIN * 2 );
    QImage encodeImage2( encodeImageSize, encodeImageSize, QImage::Format_Mono );
    encodeImage2.fill( 1 );

    // Создать двумерный образ кода
    for ( int i = 0; i < qrImageSize; i++ )
        for ( int j = 0; j < qrImageSize; j++ )
            if ( qrEncode.m_byModuleData[i][j] )
                encodeImage2.setPixel( i + QR_MARGIN, j + QR_MARGIN, 0 );

    labelq->setPixmap( QPixmap::fromImage( encodeImage2 ) );


    if ( successfulEncoding )
    {
        int scale_size = encodeImageSize * scale;

        QPixmap scale_image = labelq->pixmap()->scaled( scale_size, scale_size );
        labelq->setPixmap( scale_image );
    }

    return encodeImage2;
}

QImage MainWindow::QRCodeToQImageConverter( QString textcode, int scale ,  int versionIndex, int levelIndex, bool bExtent, int maskIndex)
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

void MainWindow::updateQRLabels()
{
    int levelIndex = 1;
    int versionIndex = 0;
    bool bExtent = true;
    int maskIndex = -1;

    QRCodeToQLabelConverter(ui->register_product_emission_QRLabel, register_product_emission_QR_string ,2,  versionIndex, levelIndex, bExtent, maskIndex);
    QRCodeToQLabelConverter(ui->register_control_samples_Label, register_control_samples_QR_string,2, versionIndex, levelIndex, bExtent, maskIndex);
    QRCodeToQLabelConverter(ui->register_end_packing_Label, register_end_packing_QR_string,2, versionIndex, levelIndex, bExtent, maskIndex);
    QRCodeToQLabelConverter(ui->unit_pack_QRLabel, unit_pack_QR_string,2, versionIndex, levelIndex, bExtent, maskIndex);
    QRCodeToQLabelConverter(ui->move_order_QRLabel, move_order_QR_string,2, versionIndex, levelIndex, bExtent, maskIndex);
    QRCodeToQLabelConverter(ui->releabeling_QRLabel, releabeling_QR_string,2, versionIndex, levelIndex, bExtent, maskIndex);

    QRCodeToQLabelConverter(ui->printControlLabel, printControlQRCode,2, versionIndex, levelIndex, bExtent, maskIndex);
    QRCodeToQLabelConverter(ui->programOptionsLabel, programOptionsQRCode,2, versionIndex, levelIndex, bExtent, maskIndex);
    QRCodeToQLabelConverter(ui->agregationLabel, agregationQRCode,2, versionIndex, levelIndex, bExtent, maskIndex);
    QRCodeToQLabelConverter(ui->statisticsLabel, statisticsQRCode,2, versionIndex, levelIndex, bExtent, maskIndex);

}

void MainWindow::addSymbolToInputString(QString str)
{
    DMCodeUpdateTimeoutTimer->start();


    qDebug() << str;
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
        if ( (key1==Qt::Key_Enter) || (key1==Qt::Key_Return)|| (key1==Qt::Key_Shift) || key1 == 0 ) {
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


void MainWindow::on_register_product_emission_Button_clicked()
{
    RegisterProductEmissionPageOpen();
}

void MainWindow::on_register_control_samples_Button_clicked()
{
    RegisterControlSamplesPageOpen();
}


void MainWindow::on_register_end_packing_Button_clicked()
{
    RegisterEndPackingPageOpen();
}


void MainWindow::connectTcp(QString address, int port)
{
    Socket = new QTcpSocket(this);
    Socket->connectToHost(address, port);
    Socket->waitForConnected(3000);
}

void MainWindow::serverWrite(QString str)
{
    if ( Socket->waitForConnected () )
    {
        Socket->write( QstringToQbytearray(str) ); // пишем
        Socket->waitForBytesWritten(1000 ); // ожидаем запись
    }
}

QByteArray MainWindow::QstringToQbytearray(QString str)
{
    QByteArray endytearray;
    endytearray.append(str);
    return endytearray;
}

void MainWindow::on_DrugsComboBox_currentIndexChanged(int index)
{
    qDebug() << index << "index";

    QString where = QString ( "drugs_name = '%1' " ).arg(ui->DrugsComboBox->itemText(index));
    QString gtin = sqlDB->sel("gtin", "drugs", where,"gtin").at(0);
    QString tnved = sqlDB->sel("tnved", "drugs", where,"tnved").at(0);
    QString dose = sqlDB->sel("Dose", "drugs", where,"Dose").at(0);
    QString conditions = sqlDB->sel("conditions", "drugs", where,"conditions").at(0);
    QString quantity = sqlDB->sel("quantity", "drugs", where,"quantity").at(0);

    ui->GTINVal->setText(gtin);
    ui->TNVEDVal->setText(tnved);
    ui->DoseVal->setText(dose);
    ui->conditions->clear();
    ui->conditions->appendPlainText(conditions);
    ui->quantityvalue->setText(quantity);
}

void MainWindow::ShowMessageBox(QString message)
{
    QMessageBox msgBox;
    msgBox.setText(message);
    //            msgBox.setInformativeText("Партия завершена");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.setStyleSheet("QMessageBox{background-color: rgb(103,158,210);color: white;}QPushButton \
    {\
                             background-color: QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #E25303, stop: 0.1 #E25303, stop: 0.49 #E25303, stop: 0.5 #E25303, stop: 1 #E25306);\
                             border-style: outset;\
                             border-width: 2px;\
                             border-radius: 10px;\
                             border-color: beige;\
                             font: bold 14px; color: white;\
                             padding: 6px;\
                         }\
                         ");
                         int ret = msgBox.exec();
            addMessageToJournal(QString("Партия %1 завершена").arg(getSerializationBatchName()),Qt::green,Qt::transparent);
}

//слот приема данных о просканированной пачке извне
void MainWindow::GetMedicamentSerialization(medicament *med)
{
    // если автоматическая упаковка и не пауза и запущено и не остановлено.
    if(getAutoupakovka() && ( ! getBSerializationPaused() ) && ( getBSerializationStarted() )&& ( ! getBSerializationStopped()) )
    {
        // проверяем что пачки не бракованые
        if  (   med->GTIN!=getSerializationGTIN() ||
                med->BatchNumber!=getSerializationBatchName() ||
                med->TNVED!=getSerializationTNVED() ||
                med->ExperyDate!=getSerializationExpery() )
        {
            quint16 brakcount  = ui->NOKlabelValue->text().toUInt();
            ui->NOKlabelValue->setText(QString::number(++brakcount));

            return;
        }

        // если автоупаковка то сразу добавляем препарат в таблицу process311noxml
        AddMedicamentToDBTable(med,"process311noxml");
        MedicamentsSerialization.append(med);

        //Проверяем сколько реально упаковок добавлено в базу данных
        QString reqstring = QString("batch like '%1';").arg(med->BatchNumber);
        QStringList ssss = sqlDB->getsumm("COUNT(1)", "mark.process311noxml",reqstring,"COUNT(1)");
        QString summa_pachek_v_partii = ssss.at(0);

        int ostalos_pachek_upakovat = getSerializationBatchValue().toInt() - summa_pachek_v_partii.toUInt();
        int ostalos_zapolnit_v_korobe = MedicamentsSerialization.length() % getSerializationQuantity().toUInt()  ;
        int proizveli_pachek = MedicamentsSerialization.length();

        ui->OKlabelValue->setText(summa_pachek_v_partii);
        ui->remainLabelValue->setText(QString::number(ostalos_pachek_upakovat));

        if ( ( ( ostalos_zapolnit_v_korobe  == 0 ) && ( proizveli_pachek > 0 ) ) || (ostalos_pachek_upakovat <= 0 ) )
        {
            qDebug() << ostalos_zapolnit_v_korobe  << proizveli_pachek << ostalos_pachek_upakovat ;
            QDateTime date311  = GetISODateTime();
            CreateXML311Doc(MedicamentsSerialization,getSerializationCompanySender(),getSerializationCompanyOwner(),getSerializationOrderType(),date311);

            QDateTime date313  = GetISODateTime().addSecs(5);
            CreateXML313Doc(MedicamentsSerialization,date313);

            // если программная агрегация
            if(getAutoprogramagregation())
            {
                QDateTime date911 = date313.addSecs(5);
                CreateXML911Doc(MedicamentsSerialization,getSerializationCompanySender(),date911 );
            }

            MedicamentsSerialization.clear();
        }

        if ((ostalos_pachek_upakovat <= 0 ))
        {
            StopSerialization();
            QString message = QString("Партия %1 завершена").arg(getSerializationBatchName()) ;
            ShowMessageBox(message);
            return;
        }
    }
}

void MainWindow::MoveOrderPageOpen()
{
    setStackedPage(9);
}

void MainWindow::on_move_order_Button_clicked()
{
    MoveOrderPageOpen();
}

void MainWindow::ReleabilingPageOpen()
{
    setStackedPage(10);
}

void MainWindow::on_releabeling_Button_clicked()
{
    ReleabilingPageOpen();
}

void MainWindow::UnitPackPageOpen()
{
    setStackedPage(11);
}

void MainWindow::on_unit_pack_Button_clicked()
{
    UnitPackPageOpen();
}

void MainWindow::SendParamsToVideoJet()
{
    QString printerdate = getGuiExperyDate().toString("yyMMdd") ;
    QString humandate = getGuiExperyDate().toString("dd.MM.yyyy") ;
    QString randstr = generateSN(11);
}

void MainWindow::SendRandomToVideoJet()
{
    if(getBSerializationStarted())
    {
        QDate Date = QDate::fromString(getSerializationExpery(),"yyyy-MM-dd");
        QString printerdate = Date.toString("yyMMdd") ;
        QString humandate = Date.toString("dd-MM-yyyy") ;
        QString randstr = generateSN(13);
        //    QString a = QString("SLA|%1|gtinvalue=%2|batchvalue=%3|expdatevalue=%4|exphumandatevalue=%5|TNVEDvalue=%6|randomvalue=%7|").arg(VideoJetFileName, getGuiGTIN(), getGuiBatchValue(), printerdate, humandate,getGuiTNVED() , randstr);
        QString a = QString("{\"command\":\"senddata\",\"data\":  {\"GTINVAL\": \"%1\", \"SNVAL\": \"%2\", \"BATCHVAL\": \"%3\", \"DATEVAL\": \"%4\", \"TNVEDVAL\": \"%5\", \"GTINTEXT\": \"%6\", \"SNTEXT\": \"%7\", \"BATCHTEXT\": \"%8\", \"DATETEXT\": \"%9\"}}").arg(getSerializationGTIN() , randstr,getSerializationBatchName(),printerdate,getSerializationTNVED(),getSerializationGTIN() ,randstr,getSerializationBatchName(),humandate);
        SendCommandToVideoJet(a);

        //        qDebug() << a;
    }
}

void MainWindow::GUIMainWindowUpdate()
{
    ui->ExperyValueHi->setText(getSerializationExpery());
    ui->BatchValueHi->setText(getSerializationBatchName() + "/" + getSerializationBatchValue());
    ui->BatchLabelValue->setText(getSerializationBatchName());
    ui->LPDose->setText(getSerializationDose());
    ui->LPLabelName->setText(getSerializationDrugName() + "/" + getSerializationCompanySender()->get_organisation_name());
    ui->LPDose->setText(getSerializationDose());
    ui->remainLabelValue->setText(getSerializationBatchName());
    ui->BatchLabelValue->setText(getSerializationBatchValue());
    ui->QuantityValueHi->setText(getSerializationQuantity());
}

void MainWindow::on_pushButton_2_clicked()
{
    SendParamsToVideoJet();
    //    SerializationGTINString = getGuiGTIN();
    //    SerializationEXPstring = getGuiExperyDate().toString("dd.MM.yyyy");
    QString drugsname = ui->DrugsComboBox->currentText();
    QString where = QString ( "drugs_name = '%1' " ).arg(drugsname);
    QString gtin = sqlDB->sel("gtin", "drugs", where,"gtin").at(0);
    QString tnved = sqlDB->sel("tnved", "drugs", where,"tnved").at(0);
    QString dose = sqlDB->sel("Dose", "drugs", where,"Dose").at(0);
    QString experydate = ui->expirationdate->dateTime().toString("yyyy-MM-dd");
    QString batchname = ui->batchnumberText->toPlainText();
    QString batchvalue = QString::number(ui->batchvalue->value());
    QString conditions = sqlDB->sel("conditions", "drugs", where,"conditions").at(0);
    QString quantity = sqlDB->sel("quantity", "drugs", where,"quantity").at(0);

    setSerializationDrugName(drugsname);
    setSerializationDose(dose);
    setSerializationGTIN(gtin);
    setSerializationTNVED(tnved);
    setSerializationExpery(experydate);
    setSerializationBatchName(batchname);
    setSerializationBatchValue(batchvalue);
    setSerializationQuantity(quantity);
    SetSerializationCompanySender(CompaniesListFromDB.at(ui->senderID->currentIndex()));
    setSerializationCompanyOwner(CompaniesListFromDB.at(ui->ownerID->currentIndex()));

    GUIMainWindowUpdate();
}

void MainWindow::on_agregationStartButton_clicked()
{

}


eticetka::eticetka()
{
    m_Barcode = new Code128Item();
    int vertikotstup = 50;
    QString ssccString = "6936756510728528";
    // 150 x 100 ok
    m_Barcode->setWidth( 190*2.5 );
    m_Barcode->setHeight( 110*2.5 );
    m_Barcode->setText(ssccString);
    m_Scene.clear();
    m_Scene.addItem( m_Barcode );
    m_Scene.update();

    m_Barcode->setRotation(-90);
    m_Barcode->update();
    m_Barcode->setPos(550,600);
    m_Barcode->setTextVisible(false);

    all_etiketka.clear();
    all_etiketka.addItem( m_Barcode);
    OrgText = new QGraphicsTextItem("ЗАО \"Березовский фармацевтический завод\"");
    OrgText->setPos(60,900);
    OrgText->setRotation(-90);
    OrgText->setFont(QFont("Ubuntu", 25, QFont::Bold ));
    all_etiketka.addItem(OrgText);

    AddressItem = new QGraphicsTextItem("623704, Свердловская область, г. Березовский, ул. Кольцевая, 13а");
    AddressItem->setPos(100,1000);
    AddressItem->setRotation(-90);
    AddressItem->setFont(QFont("Ubuntu", 20 , QFont::Bold));
    all_etiketka.addItem(AddressItem);

    PreparatItem = new QGraphicsTextItem("Лефлуномид");
    PreparatItem->setPos(140,800);
    PreparatItem->setRotation(-90);
    PreparatItem->setFont(QFont("Ubuntu", 60 , QFont::Bold));
    all_etiketka.addItem(PreparatItem);

    Dose = new QGraphicsTextItem("таблетки 10 мг №30");
    Dose->setPos(230,700);
    Dose->setRotation(-90);
    Dose->setFont(QFont("Ubuntu", 25 , QFont::Bold));
    all_etiketka.addItem(Dose);

    mainrect = new QGraphicsRectItem(0,0,850,1270);
    mainrect->setPos(10,10);
    mainrect->setPen(QPen(Qt::black,3));
    all_etiketka.addItem(mainrect);


    logorect = new QGraphicsRectItem(0,0,20*8,20*8);
    logorect->setPos(20,1110);
    logorect->setPen(QPen(Qt::black,3));
    all_etiketka.addItem(logorect);


    kolichestvouoakovok = new QGraphicsTextItem("Количество упаковок");
    kolichestvouoakovok->setPos(300,1275);
    kolichestvouoakovok->setRotation(-90);
    kolichestvouoakovok->setFont(QFont("Ubuntu", 25 , QFont::Bold));
    all_etiketka.addItem(kolichestvouoakovok);


    GTIN = new QGraphicsTextItem("GTIN: ");
    GTIN ->setPos(400 + vertikotstup,1275);
    GTIN ->setRotation(-90);
    GTIN ->setFont(QFont("Ubuntu", 25 , QFont::Bold));
    all_etiketka.addItem(GTIN );

    Seria = new QGraphicsTextItem("Серия: ");
    Seria ->setPos(400 + vertikotstup*2,1275);
    Seria ->setRotation(-90);
    Seria ->setFont(QFont("Ubuntu", 25 , QFont::Bold));
    all_etiketka.addItem(Seria );

    dataproizvodstva = new QGraphicsTextItem("Дата производства: ");
    dataproizvodstva ->setPos(400 + vertikotstup*3,1275);
    dataproizvodstva ->setRotation(-90);
    dataproizvodstva ->setFont(QFont("Ubuntu", 25 , QFont::Bold));
    all_etiketka.addItem(dataproizvodstva );

    srokgodnosti = new QGraphicsTextItem("Срок годности: ");
    srokgodnosti ->setPos(400 + vertikotstup*4,1275);
    srokgodnosti ->setRotation(-90);
    srokgodnosti ->setFont(QFont("Ubuntu", 25 , QFont::Bold));
    all_etiketka.addItem(srokgodnosti);

    usloviahranenia = new QGraphicsTextItem("Хранить и транспортировать \nпри температуре от 15 до 30 C°");
    usloviahranenia ->setPos(400 + vertikotstup*5,1275);
    usloviahranenia ->setRotation(-90);
    usloviahranenia ->setFont(QFont("Ubuntu", 25 , QFont::Bold));
    all_etiketka.addItem(usloviahranenia);

    SSCCCode = new QGraphicsTextItem(ssccString);
    SSCCCode ->setPos(810,490);
    SSCCCode ->setRotation(-90);
    SSCCCode ->setFont(QFont("Ubuntu", 25 , QFont::Bold));
    all_etiketka.addItem(SSCCCode);
    all_etiketka.update();
}

eticetka::eticetka(QString OrgTextstring, QString Dosetext, QString Addresstext, QString PreparatText, int kolvoupakovoktext, QString gtinText, QString SeriaText, QString dataproizvodstvaText, QString SrokgodnostiText, QString usloviahraneniaText, QString regnomerText, QString SSCCCodetext)
{
    m_Barcode = new Code128Item();

    int vertikotstup = 50;

    QString ssccString = SSCCCodetext;

    // 150 x 100 ok
    m_Barcode->setWidth( 190*2.5 );
    m_Barcode->setHeight( 110*2.5 );
    m_Barcode->setText(ssccString);
    m_Scene.clear();
    m_Scene.addItem( m_Barcode );
    m_Scene.update();

    m_Barcode->setRotation(-90);
    m_Barcode->update();
    m_Barcode->setPos(550,600);
    m_Barcode->setTextVisible(false);

    all_etiketka.clear();
    all_etiketka.addItem( m_Barcode);

    OrgText = new QGraphicsTextItem(OrgTextstring);
    OrgText->setPos(60,900);
    OrgText->setRotation(-90);
    OrgText->setFont(QFont("Ubuntu", 25, QFont::Bold ));
    OrgText->setDefaultTextColor(Qt::black);
    all_etiketka.addItem(OrgText);

    AddressItem = new QGraphicsTextItem(Addresstext);
    AddressItem->setPos(100,1000);
    AddressItem->setRotation(-90);
    AddressItem->setFont(QFont("Ubuntu", 20 , QFont::Bold));
    AddressItem->setDefaultTextColor(Qt::black);
    all_etiketka.addItem(AddressItem);


    PreparatItem = new QGraphicsTextItem(PreparatText);
    PreparatItem->setPos(140,800);
    PreparatItem->setRotation(-90);
    PreparatItem->setFont(QFont("Ubuntu", 60 , QFont::Bold));
    PreparatItem->setDefaultTextColor(Qt::black);
    all_etiketka.addItem(PreparatItem);

    Dose = new QGraphicsTextItem(Dosetext);
    Dose->setPos(230,700);
    Dose->setRotation(-90);
    Dose->setFont(QFont("Ubuntu", 25 , QFont::Bold));
    Dose->setDefaultTextColor(Qt::black);
    all_etiketka.addItem(Dose);

    mainrect = new QGraphicsRectItem(0,0,850,1270);
    mainrect->setPos(10,10);
    mainrect->setPen(QPen(Qt::black,3));
    all_etiketka.addItem(mainrect);

    QPixmap pixmap("C:/Work/Application/BFZLogo.jpg");

    logo = new QGraphicsPixmapItem(pixmap);
    logo ->setPos(15,1275);
    logo->setRotation(-90);
    logo->setScale(0.25);
    all_etiketka.addItem(logo);

    kolichestvouoakovok = new QGraphicsTextItem("Количество упаковок: " + QString::number(kolvoupakovoktext));
    kolichestvouoakovok->setPos(300,1275);
    kolichestvouoakovok->setRotation(-90);
    kolichestvouoakovok->setFont(QFont("Ubuntu", 25 , QFont::Bold));
    kolichestvouoakovok->setDefaultTextColor(Qt::black);
    all_etiketka.addItem(kolichestvouoakovok);

    GTIN = new QGraphicsTextItem("GTIN: " + gtinText);
    GTIN ->setPos(400 + vertikotstup,1275);
    GTIN ->setRotation(-90);
    GTIN ->setFont(QFont("Ubuntu", 25 , QFont::Bold));
    GTIN ->setDefaultTextColor(Qt::black);
    all_etiketka.addItem(GTIN );

    Seria = new QGraphicsTextItem("Серия: " + SeriaText);
    Seria ->setPos(400 + vertikotstup*2,1275);
    Seria ->setRotation(-90);
    Seria ->setFont(QFont("Ubuntu", 25 , QFont::Bold));
    Seria ->setDefaultTextColor(Qt::black);
    all_etiketka.addItem(Seria );

    dataproizvodstva = new QGraphicsTextItem("Дата производства: " + dataproizvodstvaText);
    dataproizvodstva ->setPos(400 + vertikotstup*3,1275);
    dataproizvodstva ->setRotation(-90);
    dataproizvodstva ->setFont(QFont("Ubuntu", 25 , QFont::Bold));
    dataproizvodstva ->setDefaultTextColor(Qt::black);
    all_etiketka.addItem(dataproizvodstva );

    srokgodnosti = new QGraphicsTextItem("Срок годности: " + SrokgodnostiText);
    srokgodnosti ->setPos(400 + vertikotstup*4,1275);
    srokgodnosti ->setRotation(-90);
    srokgodnosti ->setFont(QFont("Ubuntu", 25 , QFont::Bold));
    srokgodnosti ->setDefaultTextColor(Qt::black);
    all_etiketka.addItem(srokgodnosti);


    usloviahranenia = new QGraphicsTextItem(usloviahraneniaText);
    usloviahranenia ->setPos(400 + vertikotstup*5,1275);
    usloviahranenia ->setRotation(-90);
    usloviahranenia ->setFont(QFont("Ubuntu", 25 , QFont::Bold));
    usloviahranenia ->setDefaultTextColor(Qt::black);
    all_etiketka.addItem(usloviahranenia);

    SSCCCode = new QGraphicsTextItem(ssccString);
    SSCCCode ->setPos(810,570);
    SSCCCode ->setRotation(-90);
    SSCCCode ->setFont(QFont("Ubuntu", 25 , QFont::Bold));
    SSCCCode ->setDefaultTextColor(Qt::black);
    all_etiketka.addItem(SSCCCode);

    all_etiketka.update();
}

void MainWindow::on_batchnumberText_textChanged()
{
    if (ui->batchnumberText->toPlainText().length()>maxserialnumberlenght)
    {
        QString s = ui->batchnumberText->toPlainText();
        s.truncate(maxserialnumberlenght);
        ui->batchnumberText->clear();
        ui->batchnumberText->appendPlainText(s);
    }
}

void MainWindow::on_pushButton_clicked()
{
    SQLInit();
}

void MainWindow::AddMedicamentToDBTable(medicament *m, QString tablename)
{
    QString req = QString("insert into %1 values (\"%2\",\"%3\",\"%4\",%5,\"%6\");").arg(tablename, m->GTIN + m->SerialNumber,QDateTime::currentDateTime().toTimeSpec(Qt::LocalTime).toString("dd-MM-yyyy"),QDateTime::currentDateTime().toTimeSpec(Qt::LocalTime).toString("hh-mm-ss"),"null", m->BatchNumber);
    sqlDB->makesqlreq(req);
}

void MainWindow::AddStatisticsToDB(QString bisnessprocessname, medicament *m, QDateTime datetime, int count,QString XMLFileName)
{
    QString tablename = "`mark`.`statistics`" ;
    QString req = QString("INSERT INTO %1 (`BProcess`, `GTIN`, `LPName`, `batch`, `date`, `count`, `xmlfilename`) VALUES (\"%2\",\"%3\",\"%4\",\"%5\",'%6','%7',\"%8\");").arg(tablename, bisnessprocessname,m->GTIN,m->medicament_name, m->BatchNumber,datetime.toTimeSpec(Qt::LocalTime).toString("yyyy-MM-dd hh:mm:ss"),QString::number(count), XMLFileName);
    sqlDB->makesqlreq(req);
    qDebug() << "AddStatisticsToDB " << req;
}


void MainWindow::AddStatisticsToDB(QString bisnessprocessname,QString GTIN,QString medicament_name,QString BatchNumber, QDateTime datetime, int count,QString XMLFileName)
{
    QString tablename = "`mark`.`statistics`" ;
    QString req = QString("INSERT INTO %1 (`BProcess`, `GTIN`, `LPName`, `batch`, `date`, `count`, `xmlfilename`) VALUES (\"%2\",\"%3\",\"%4\",\"%5\",'%6','%7',\"%8\");").arg(tablename, bisnessprocessname, GTIN, medicament_name, BatchNumber,datetime.toTimeSpec(Qt::LocalTime).toString("yyyy-MM-dd hh:mm:ss"),QString::number(count), XMLFileName);
    sqlDB->makesqlreq(req);
    qDebug() << "AddStatisticsToDB " << req;
}


void MainWindow::StartSerialization()
{
    setBSerializationStarted(true);
    setBSerializationStopped(false);
    setBSerializationPaused(false);

    addMessageToJournal("Старт сериализации",Qt::green,Qt::white);
    ui->MedicamentOptionsGroup->setEnabled(false);
    ui->StartSerializationButton->setEnabled(false);
    ui->PauseSerializationButton->setEnabled(true);
    ui->ContinueSerializationButton->setEnabled(false);
    ui->StopSerializationButton->setEnabled(true);
    ui->groupBox_2->setEnabled(false);

    //Проверяем сколько реально упаковок добавлено в базу данных
    QString reqstring = QString("batch like '%1';").arg(getSerializationBatchName());
    QStringList ssss = sqlDB->getsumm("COUNT(1)", "mark.process311noxml",reqstring,"COUNT(1)");

    QString summa_pachek_v_partii = ssss.at(0);
    int ostalos_pachek_upakovat = getSerializationBatchValue().toInt() - summa_pachek_v_partii.toUInt();

    if(ostalos_pachek_upakovat<=0)
    {
        QString me = QString("Партия %1 завершена").arg(getSerializationBatchName());
        addMessageToJournal(me,Qt::green,Qt::transparent);
        StopSerialization();
    }

    QString a = QString("{\"command\":\"startprint\",\"username\":\"Admin\",\"password\":\"ioj@admin\", \"startpage\":1,\"endpage\":0, \"templatename\":\"DM10works\"}");
    SendCommandToVideoJet(a);

    AddStatisticsToDB("start",getSerializationGTIN(), getSerializationDrugName(),getSerializationBatchName(),QDateTime::currentDateTime(), 0,"");
}

void MainWindow::StopSerialization()
{
    setBSerializationStarted(false);
    setBSerializationStopped(true);
    setBSerializationPaused(false);

    addMessageToJournal("Останов.сериализации",Qt::red,Qt::white);

    ui->MedicamentOptionsGroup->setEnabled(true);
    ui->StartSerializationButton->setEnabled(true);
    ui->PauseSerializationButton->setEnabled(false);
    ui->ContinueSerializationButton->setEnabled(false);
    ui->StopSerializationButton->setEnabled(false);
    ui->groupBox_2->setEnabled(true);

    QDateTime date311  = GetISODateTime();
    CreateXML311Doc(MedicamentsSerialization,getSerializationCompanySender(),getSerializationCompanyOwner(),getSerializationOrderType(),date311);

    QDateTime date313  = GetISODateTime().addSecs(1);
    CreateXML313Doc(MedicamentsSerialization,date313.addSecs(10));

    // если программная агрегация
    if(getAutoprogramagregation())
    {
        QDateTime date911 = date313.addSecs(1);
        CreateXML911Doc(MedicamentsSerialization,getSerializationCompanySender(),date911 );
    }
    MedicamentsSerialization.clear();

    QString a = QString("{\"command\":\"stopprint\",\"username\":\"Admin\",\"password\":\"ioj@admin\"}");
    SendCommandToVideoJet(a);


    AddStatisticsToDB("stop",getSerializationGTIN(), getSerializationDrugName(),getSerializationBatchName(),QDateTime::currentDateTime(), 0,"");

}

void MainWindow::PauseSerialization()
{
    setBSerializationPaused(true);

    ui->StartSerializationButton->setEnabled(false);
    ui->PauseSerializationButton->setEnabled(false);
    ui->ContinueSerializationButton->setEnabled(true);
    ui->StopSerializationButton->setEnabled(true);
    addMessageToJournal("Пауза сериализации",Qt::blue,Qt::white);
}

void MainWindow::ContinueSerialization()
{
    setBSerializationPaused(false);

    ui->StartSerializationButton->setEnabled(false);
    ui->PauseSerializationButton->setEnabled(true);
    ui->ContinueSerializationButton->setEnabled(false);
    ui->StopSerializationButton->setEnabled(true);
    addMessageToJournal("Продолж.сериализации",Qt::green,Qt::white);
}

void MainWindow::on_StatistFindButton_clicked()
{
    QString statbisnesprocess = ui->StatistBPcomboBox->currentText();
    QString statmedicament = ui->StatistMedicamentComboBox->currentText();
    QString statbatch = ui->StatistBatchComboBox->currentText();
    QString statGtin = ui->StatistGTINCombobox->currentText();
    QString datefrom = ui->StatistDateTimeFromEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString dateto = ui->StatistDateTimeToEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString reqstring = QString("batch like '%1' and BProcess like '%2' and LPName like '%3' and date BETWEEN '%4' and '%5'  and GTIN like '%6';").arg(statbatch,statbisnesprocess,statmedicament,datefrom,dateto,statGtin);
    QStringList ssss = sqlDB->getsumm("SUM(count) AS Total", "mark.statistics",reqstring,"Total");
    QString summ = ssss.at(0);


    QStringList startdates = sqlDB->sel("date", "statistics", QString("batch like '%1' and BProcess = 'start'").arg(statbatch),"date");
    QStringList stopdates = sqlDB->sel("date", "statistics",  QString("batch like '%1' and BProcess = 'stop'").arg(statbatch),"date");
    ui->FoundLabel->setText("Найдено: " + summ);


    qDebug() << "startdates" << startdates.first() ;
    qDebug() << "stopdates" << stopdates.last() ;

    //    QDate StartDate = QDate::fromString(startdates.first(),"yyyy-MM-ddThh:mm:ss");
    QDateTime StartDate = QDateTime::fromString(startdates.first(),Qt::ISODate);
    QDateTime StopDate =  QDateTime::fromString(stopdates.last(),Qt::ISODate);

    qDebug() << "StartDate" << StartDate;
    qDebug() << "StopDate" << StopDate ;

    ui->StartTimeStatisticsLabel->setText( QString("время начала ") + StartDate.toString("yyyy-MM-dd hh:mm:ss"));
    ui->StopTimeStatisticsLabel->setText(QString("время окончания ") +  StopDate.toString("yyyy-MM-dd hh:mm:ss"));

    //SELECT SUM(count) AS Total FROM mark.statistics where batch like "A12345" and GTIN like '%';
    //    qDebug() << "SUM" << ssss ;
    GetStatisticsFromDB();
}

void MainWindow::on_SerializAutoUpakovkaCheckBox_toggled(bool checked)
{
    setAutoupakovka(checked);
    ui->autoserializationoptions->setEnabled(checked);
}

void MainWindow::on_SerializAutoAgregationCheckBox_toggled(bool checked)
{
    if(checked)
    {
        setAutoprogramagregation(false);
        ui->SerializAutoAgregationProgramCheckBox->setChecked(false);
    }
    setAutoagregation(checked);
}

void MainWindow::on_SerializAutoAgregationProgramCheckBox_toggled(bool checked)
{
    if(checked)
    {
        setAutoagregation(false);
        setAutoupakovka(true);
        ui->SerializAutoUpakovkaCheckBox->setChecked(true);
        ui->SerializAutoAgregationCheckBox->setChecked(false);
    }
    setAutoprogramagregation(checked);
}

void MainWindow::GetCompaniesDBList(QList<manufacturer*> man)
{
    QStringList a ;
    foreach (manufacturer * d , man) {
        a.append(d->get_organisation_name());
    }
    ui->senderID->addItems(a);
    ui->ownerID->addItems(a);
}

QDateTime MainWindow::getoperationDate()
{
    QDateTime operation_date = ui->operationDate->dateTime();
    return operation_date;
}

void MainWindow::on_optionsButton_clicked()
{
    setStackedPage(12);
}

void MainWindow::on_SetSerializationOptionsButton_clicked()
{
    ui->SerialTime->setText( QDateTime::currentDateTime().toString("hh:mm::ss:zzz") );
    SerializationLine1 = new SerializationLine(ui->IPAddress->toPlainText(), ui->TCPPort->value(),ui->countinminuteValue->value(),ui->SPEEDValue->value() , getSerializationDrugName(),getSerializationGTIN(),getSerializationExpery(),getSerializationBatchName() );
    connect(SerializationLine1, SIGNAL(ResponseRecieved(QString,quint16,QString)), this, SLOT(ResponseFromLineRecieved(QString,quint16,QString)));
    connect(SerializationLine1, SIGNAL(DrugRecieved(QString,QString,QString,QString,QString)), this, SLOT(DrugRecievedFromEmulator(QString,QString,QString,QString,QString)));
}


void MainWindow::replyfinished(QNetworkReply *reply)
{
    int i;
    QByteArray bytes = reply->readAll(); // bytes
    qDebug("reply received");
    qDebug() << reply->errorString();
}


void MainWindow::DrugRecievedFromEmulator(QString BatchName,QString ExperyDate, QString GTIN, QString SerialNumber, QString Tnved)
{
    QString sgtin = SerialNumber + GTIN;
    medicament * t = new medicament(getSerializationDrugName(),GTIN,SerialNumber,BatchName,ExperyDate,sgtin,Tnved);
    emit SendMedicamentSignal(t);
}

void MainWindow::on_keyboardButton_clicked()
{
    bool ok = QProcess::startDetached("onboard");
}


