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
#include "sql.h"
#include "QtPrintSupport/qprinter.h"
#include "QtPrintSupport/QPrinter"
#include "QtPrintSupport/QPrintDialog"
#define testitemscount 0

int aaa = 222;
int bbb = 789;

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
    // подтягиваем параметры компании
    GetCompaniesDBList();
}

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
    ui->qrstartstop->setPixmap(*pixmapqr);
    ui->qrstartstop->show();
    ui->qrstartstop->setScaledContents(1);

    this->installEventFilter(this);

    RandomStringSenderToVideoJetTimer = new QTimer();
    RandomStringSenderToVideoJetTimer->setInterval(1000*45); // каждые сорок пять секунд посылаем новую произвольную строку
//    RandomStringSenderToVideoJetTimer->setInterval(1000); // каждые сорок пять секунд посылаем новую произвольную строку
    connect(RandomStringSenderToVideoJetTimer, &QTimer::timeout, this, &MainWindow::SendRandomToVideoJet);
    RandomStringSenderToVideoJetTimer->start();

    journalTimer = new QTimer();
    journalTimer->setInterval(1000);

    connect(journalTimer, &QTimer::timeout, this, &MainWindow::updateDMPicture);
    connect(journalTimer, &QTimer::timeout, this, &MainWindow::updateDMcode);

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
    connect(ui->RegisterEndPackingPage311Widget, SIGNAL(),this , SLOT(StartAgregation()) ) ;
    connect(ui->RegisterEndPackingPage311Widget, &RegisterEndPackingWidget311::setScannerLanguage, this, &MainWindow::setLanguageswitcher) ;
    connect(this, &MainWindow::SendMedicamentSignal, ui->RegisterEndPackingPage311Widget, &RegisterEndPackingWidget311::GetMedicament) ;
    connect(this, &MainWindow::SendCompaniesDBList, ui->RegisterEndPackingPage311Widget, &RegisterEndPackingWidget311::GetCompaniesDBList) ;
    connect(ui->RegisterEndPackingPage311Widget, SIGNAL(RegistrationCompleted(QList<medicament*>,manufacturer*,manufacturer*,int,QDateTime)), this, SLOT(CreateXML311Doc(QList<medicament*>,manufacturer*,manufacturer*,int,QDateTime)));

    // сигналы и слоты для 312 бизнес процесса
    connect(this, &MainWindow::Start312Process, ui->ExtractWidget, &UnitExtractWidget::StartRegistrationProcess ) ;
    connect(this, &MainWindow::Stop312Process, ui->ExtractWidget, &UnitExtractWidget::StopRegistrationProcess ) ;
    connect(this, SIGNAL(SendMedicamentSignal(medicament*)), ui->ExtractWidget, SLOT(GetMedicament(medicament*))) ;
    connect(ui->ExtractWidget, &UnitExtractWidget::RegistrationCompleted, this, &MainWindow::CreateXML312Doc) ;
    connect(ui->ExtractWidget, &UnitExtractWidget::RegistrationCompleted,this , &MainWindow::StopAgregation ) ;
    connect(ui->ExtractWidget, SIGNAL(RegistrationStarted()),this , SLOT(StartAgregation()) ) ;

    // сигналы и слоты для 313 бизнес процесса, пока он в форме mainwindow, позже нужно будет создать отдельный виджет
    connect(ui->agregationStartButton, &QAbstractButton::pressed, this, &MainWindow::Toggle313Process) ;
    connect(this, &MainWindow::agregationstatusToggled, this, &MainWindow::updateAgregationGUI) ;
    connect(this, &MainWindow::ParcingEnded, this, &MainWindow::updateAgregationGUI) ;
    connect(this, &MainWindow::register_product_emission_QR_Scanned, this, &MainWindow::RegisterProductEmissionPageOpen) ;
    connect(this, &MainWindow::register_control_samples_QR_Scanned, this, &MainWindow::RegisterControlSamplesPageOpen) ;
    connect(this, &MainWindow::register_end_packing_QR_Scanned, this, &MainWindow::RegisterEndPackingPageOpen) ;
    connect(this, &MainWindow::SendMedicamentSignal,this , &MainWindow::GetMedicament) ;


    // сигналы и слоты для 415 бизнес процесса

    connect(ui->MoveOrderWidget, &MoveOrder415::RegistrationCompleted, this, &MainWindow::CreateXML415Doc) ;
    connect(ui->MoveOrderWidget, &MoveOrder415::setScannerLanguage, this, &MainWindow::setLanguageswitcher) ;
    connect(this, &MainWindow::SendMedicamentSignal, ui->MoveOrderWidget, &MoveOrder415::GetMedicament) ;
    connect(this, &MainWindow::SendCompaniesDBList, ui->MoveOrderWidget, &MoveOrder415::GetCompaniesDBList) ;

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

    // ПРИСВАИВАЕМ КАЖДОМУ СИГНАЛУ КНОПКИ ИНДЕКС
    signalMapper -> setMapping (ui->printControlButton, 0) ;
    signalMapper -> setMapping (ui->programOptionsButton, 1) ;
    signalMapper -> setMapping (ui->agregationButton, 2) ;
    signalMapper -> setMapping (ui->statisticksButton, 3) ;

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

    updateAgregationGUI();
    setStackedPage(2);

    SQLInit();

    Organizacia = CompaniesListFromDB.at(1);

    ui->CompaniesCombobox->clear();
    ui->DrugsComboBox->clear();
    ui->DrugsComboBox->addItems(drugs);
    ui->CompaniesCombobox->addItems(companies);

    //    QElapsedTimer timer = SQLInsertSpeedTest();
//    QElapsedTimer timer = SQLSelectSpeedTest();
//    ui->dbLog->setText("загрузка " +  QString::number(testitemscount) + "элементов заняла " +  QString::number(timer.elapsed()/1000 ) + '.'  + QString::number(timer.elapsed()%1000 ) +  "seconds");

    // adding TCP Client
    connectTcp(TCPaddress, TCPPort);
    StopAgregation();

    ui->MedicamentsTable->horizontalHeader()->setVisible(true);

    QCoreApplication::addLibraryPath(QDir::currentPath());
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

QString MainWindow::getGuiTNVED()
{
    return ui->TNVEDVal->toPlainText();
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

    if (ui->journalList->count()>100)
    {
        ui->journalList->clear();
    }

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
    ui->dbLog->setText(m);
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

QString MainWindow::GetDOCDate()
{
    QString DOCdate;
    // формат <doc_date>07.10.2017</doc_date>
    DOCdate = QDateTime::currentDateTime().toTimeSpec(Qt::LocalTime).toString("dd.MM.yyyy");
    return DOCdate;
}

void MainWindow::CreateXML313Doc(manufacturer * organization, QList<medicament *> MedList)
{
    setRunningBuisenessProcess(false);
    setLanguageswitcher(false);

    //  если пустой список то и генерировать то нечего
    if (MedList.length() == 0)
        return;

    QDomDocument document;
    QDomElement root = document.createElement("documents");
    document.appendChild(root);

    QDomElement reg_prod_emis_elem  = document.createElement("register_product_emission");
    reg_prod_emis_elem.setAttribute("action_id", ActionIDTypeEnum::RegisterProductEmission);
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

    operationdatetext.setNodeValue( GetISODate());
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

    QString filepath = QDir::currentPath()   + "/313-register_product_emission (" + QDateTime::currentDateTime().toTimeSpec(Qt::LocalTime).toString("hh-mm dd-MM-yyyy") + ").xml";

    qDebug() <<filepath;

    QFile file(filepath);

    if ( !file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open";
        return;
    }
    else
    {
        QTextStream stream(&file);
        stream<< document.toString();
        file.close();
    }
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

    QString filepath = QDir::currentPath()   + "/415-move_order(" + QDateTime::currentDateTime().toTimeSpec(Qt::LocalTime).toString("hh-mm dd-MM-yyyy") + ").xml";

    qDebug() <<filepath;

    QFile file(filepath);

    if ( !file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        //        qDebug() << "Failed to open";
        return;
    }
    else
    {
        QTextStream stream(&file);
        stream<< document.toString();
        file.close();
    }

    QDesktopServices::openUrl(filepath);  // раскомментить если мы хотим чтобы по окончании агрегации открывался XML файл
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

    QString filepath ="C:/Work/Generated XML/811-unit_pack(" + QDateTime::currentDateTime().toTimeSpec(Qt::LocalTime).toString("hh-mm dd-MM-yyyy") + ").xml";

    qDebug() <<filepath;

    QFile file(filepath);

    if ( !file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open";
        return;
    }
    else
    {
        QTextStream stream(&file);
        stream<< document.toString();
        file.close();
    }
}

void MainWindow::CreateXML911Doc(QList<medicament *> MedList, manufacturer *companysender, QDateTime operation_date)
{
    setRunningBuisenessProcess(false);
    setLanguageswitcher(false);

    if (MedList.length() <=0)
        return ;

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
    QString address = "623704, Свердловская область, г. Березовский, ул. Кольцевая, 13а";

    EticetkaBFZ = new eticetka(companysender->get_organisation_name(),dose,address,MedList.at(0)->medicament_name,MedList.length(),MedList.at(0)->GTIN,MedList.at(0)->BatchNumber,operation_date.toTimeSpec(Qt::LocalTime).toString("dd.MM.yyyy"),ExperyDate.toString("dd.MM.yyyy"),conditions,"0000",SSCCCode128 );

    if ( PrintBIGEtiketka(EticetkaBFZ)!= true)
        return;

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

    //    QString filepath = QDir::currentPath()   + "/911-unit_pack(" + QDateTime::currentDateTime().toTimeSpec(Qt::LocalTime).toString("hh-mm dd-MM-yyyy") + ").xml";
    QString filepath ="C:/Work/Generated XML/911-unit_pack(" + QDateTime::currentDateTime().toTimeSpec(Qt::LocalTime).toString("hh-mm dd-MM-yyyy") + ").xml";

    qDebug() <<filepath;

    QFile file(filepath);

    if ( !file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open";
        return;
    }
    else
    {
        QTextStream stream(&file);
        stream<< document.toString();
        file.close();
    }
}

void MainWindow::CreateXML312Doc( QList<medicament *> MedList, uint8_t controlsamplestype)
{

    setRunningBuisenessProcess(false);
    setLanguageswitcher(false);

    manufacturer *organization = getcompany();

    //  если пустой список то и генерировать то нечего
    if (MedList.length() == 0)
        return;

    QDomDocument document;
    QDomElement root = document.createElement("documents");
    document.appendChild(root);

    QDomElement reg_ctrl_smples_elem  = document.createElement("register_control_samples");
    reg_ctrl_smples_elem.setAttribute("action_id", ActionIDTypeEnum::RegisterControlSamples);
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

    QString filepath = QDir::currentPath()   + "/312-register_control_samples (" + QDateTime::currentDateTime().toTimeSpec(Qt::LocalTime).toString("hh-mm dd-MM-yyyy") + ").xml";

    qDebug() <<filepath;
    QFile file(filepath);

    if ( !file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open";
        return;
    }
    else
    {
        QTextStream stream(&file);
        stream<< document.toString();
        file.close();
    }

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
    //qDebug() << "sender" << sender->get_organisation_name();
    //qDebug() << "owner" << owner->get_organisation_name();

    setRunningBuisenessProcess(false);
    setLanguageswitcher(false);

    if (MedList.length() <=0)
        return ;

    QDomDocument document;
    QDomElement root = document.createElement("documents");
    document.appendChild(root);

    QDomElement reg_end_pack_elem  = document.createElement("register_end_packing");
    reg_end_pack_elem.setAttribute("action_id", ActionIDTypeEnum::RegisterEndPacking);
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

    QString filepath = QDir::currentPath()   + "/311-register_end_packing(" + QDateTime::currentDateTime().toTimeSpec(Qt::LocalTime).toString("hh-mm dd-MM-yyyy") + ").xml";

    qDebug() <<filepath;

    QFile file(filepath);

    if ( !file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open";
        return;
    }
    else
    {
        QTextStream stream(&file);
        stream<< document.toString();
        file.close();
    }

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
        emit register_product_emission_QR_Scanned();
        qDebug() << "1";
        return;
    }

    if (stringforparse == register_control_samples_QR_string)
    {
        emit register_control_samples_QR_Scanned();
        qDebug() << "2";
        return;
    }

    if (stringforparse == register_end_packing_QR_string)
    {
        emit register_end_packing_QR_Scanned();
        qDebug() << "3";
        return;
    }

    if (stringforparse == printControlQRCode)
    {
        emit printControlQRCodeScanned();
        qDebug() << "4";
        return;
    }

    if (stringforparse == programOptionsQRCode)
    {
        emit programOptionsQRCodeScanned();
        qDebug() << "5";
        return;
    }

    if (stringforparse == agregationQRCode)
    {
        emit agregationQRCodeScanned();
        qDebug() << "6";
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

    // если прочитали неверную дату годности
    //    if( IsDateProper(expstring) == false)
    //    {
    //        expstring = NotFoundString;
    //    }



    // кончаем разбирать Срок Годности

    // начинаем разбирать Партию

    batchstring = GetRegularString(stringforparse, BatchRegularexpression);
    batchstring.remove(0,2);
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
    ui->packagePicture->setScene(scene);
    scene->addPixmap(image);
    scene->setSceneRect(image.rect());
    ui->packagePicture->show();

}

void MainWindow::updateDMcode()
{
    ui->DMcodeValue->setText(GenerateDMcode());
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
        CreateXML313Doc(Organizacia,MedicamentsList);
        inputDataStringFromScaner.clear();
    }

    MedicamentsList.clear();
    agregation = set;
    emit agregationstatusToggled();
}


manufacturer *MainWindow::getcompany() const
{
    return Organizacia;
}

void MainWindow::setcompany(manufacturer *value)
{
    Organizacia = value;
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
    if (getAgregation() == true)
    {
        ui->agregationStartButton->setText("Закончить регистрацию");
        ui->GTINTextAgregation->setEnabled(true);
        ui->batchnumberTextAgregation->setEnabled(true);
        ui->expirationdateAgregation->setEnabled(true);
        ui->TNVEDValueAgregation->setEnabled(true);
        ui->ScannedCode->setEnabled(true);
        ui->serialNumberAgregationValue->setEnabled(true);

        pixmapqr->load(QDir::currentPath() + "/stopapp.jpg");
        ui->qrstartstop->setPixmap(*pixmapqr);
    }
    else
    {
        ui->agregationStartButton->setText("Начать регистрацию");
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

        ui->MedicamentsTable->clearContents();
        ui->MedicamentsTable->setRowCount(0);
        ui->qrstartstop->show();
        ui->qrstartstop->setScaledContents(1);
    }

    if (inputDataStringFromScaner.isEmpty())
        ui->ScannedCode->clear();

    if ( (inputDataStringFromScaner!= Start313ProcessQRString) && (inputDataStringFromScaner!= Stop313ProcessQRString) )
    {
        ui->ScannedCode->setText(inputDataStringFromScaner);
    }

    ui->GTINTextAgregation->setText(gtinstring);
    ui->serialNumberAgregationValue->setText(SNstring);
    ui->batchnumberTextAgregation->setText(batchstring);
    ui->expirationdateAgregation->setText(expstring);
    ui->TNVEDValueAgregation->setText(tnvedstring);
}

void MainWindow::AddMedicamentToTable(medicament * m)
{
    if (getAgregation() )
    {
        ui->MedicamentsTable->insertRow(0);

        ui->MedicamentsTable->setItem(0, 0, new QTableWidgetItem(m->medicament_name));
        ui->MedicamentsTable->setItem(0, 1, new QTableWidgetItem(m->GTIN));
        ui->MedicamentsTable->setItem(0, 2, new QTableWidgetItem(m->BatchNumber));
        ui->MedicamentsTable->setItem(0, 3, new QTableWidgetItem(m->SerialNumber));
        ui->MedicamentsTable->setItem(0, 4, new QTableWidgetItem(m->TNVED));
        ui->MedicamentsTable->setItem(0, 5, new QTableWidgetItem(m->ExperyDate));
        ui->MedicamentsTable->scrollToTop();
        ui->MedicamentsTable->horizontalHeader()->setVisible(true);
    }
}

void MainWindow::AddMedicamentToDB(medicament *m)
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
    setStackedPage(7);
}

void MainWindow::RegisterControlSamplesPageOpen()
{
    setStackedPage(5);
}

void MainWindow::RegisterEndPackingPageOpen()
{
    setStackedPage(6);
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
}

void MainWindow::SendCommandToVideoJet(QString a)
{
    serverWrite(a);
    a ="\r"; // посылаем знак завершения посылки.
    serverWrite(a);
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

    QRCodeToQLabelConverter(ui->register_product_emission_QRLabel, register_product_emission_QR_string ,2.2,  versionIndex, levelIndex, bExtent, maskIndex);
    QRCodeToQLabelConverter(ui->register_control_samples_Label, register_control_samples_QR_string,2.2, versionIndex, levelIndex, bExtent, maskIndex);
    QRCodeToQLabelConverter(ui->register_end_packing_Label, register_end_packing_QR_string,2.2, versionIndex, levelIndex, bExtent, maskIndex);

    QRCodeToQLabelConverter(ui->printControlLabel, printControlQRCode,2.2, versionIndex, levelIndex, bExtent, maskIndex);
    QRCodeToQLabelConverter(ui->programOptionsLabel, programOptionsQRCode,2.2, versionIndex, levelIndex, bExtent, maskIndex);
    QRCodeToQLabelConverter(ui->agregationLabel, agregationQRCode,2.2, versionIndex, levelIndex, bExtent, maskIndex);
    QRCodeToQLabelConverter(ui->statisticsLabel, statisticsQRCode,2.2, versionIndex, levelIndex, bExtent, maskIndex);

}

void MainWindow::addSymbolToInputString(QString str)
{
    DMCodeUpdateTimeoutTimer->start();

    //  qDebug() << "start timeout timer";
    QString wastext = inputDataStringFromScaner;

    if (getLanguageswitcher() == true)
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
        Socket->waitForBytesWritten(1000); // ожидаем запись
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

    ui->GTINVal->setText(gtin);
    ui->TNVEDVal->setText(tnved);
    ui->DoseVal->setText(dose);
    ui->conditions->clear();
    ui->conditions->appendPlainText(conditions);
}

void MainWindow::GetMedicament(medicament *med)
{
    //updateWidgetGui(ScannedMedicament->GTIN, ScannedMedicament->SerialNumber, ScannedMedicament->TNVED, ScannedMedicament->ExperyDate, ScannedMedicament->BatchNumber);
    if (getAgregation())
    {
        // проверяем если пачка с таким же номером партии и серийником была просканирована недавно
        foreach ( medicament * listmed , MedicamentsList)
        {
            if ( (med->SerialNumber == listmed->SerialNumber)&&(med->BatchNumber == listmed->BatchNumber) )
            {
                qDebug() << "такой медикамент уже есть";
                ui->errorLabel->setText("Медикамент уже просканирован");
                return;
            }

            if ( (med->GTIN != listmed->GTIN ) )
            {
                qDebug() << "неверный GTIN, препарат должен иметь GTIN = " + MedicamentsList.at(0)->GTIN;
                ui->errorLabel->setText("неверный GTIN");
            }

            if ( (med->ExperyDate != listmed->ExperyDate ) )
            {
                qDebug() << "неверная дата годности, верная -  " + MedicamentsList.at(0)->ExperyDate;
                ui->errorLabel->setText("неверная Дата");

            }

            if ( (med->BatchNumber != listmed->BatchNumber ) )
            {
                qDebug() << "неверная партия, верная -  " + MedicamentsList.at(0)->BatchNumber;
                ui->errorLabel->setText("неверная партия");
            }

            if ( (med->TNVED != listmed->TNVED ) )
            {
                qDebug() << "неверная TNVED, верная -  " + MedicamentsList.at(0)->TNVED;
                ui->errorLabel->setText("неверная ТНВЭД");
            }

            if (( (med->GTIN != listmed->GTIN ) ) ||( (med->ExperyDate != listmed->ExperyDate ) )|| ( (med->BatchNumber != listmed->BatchNumber ) ) || ( (med->TNVED != listmed->TNVED ) ))
            {
                return;
            }
        }

        if (CheckMedicamentinDB(med))
        {
            qDebug() << "такой медикамент уже есть в базе данных";
            ui->errorLabel->setText("Медикамент есть в БД");
            return;
        }

        MedicamentsList.append(med);
        AddMedicamentToTable(med);
        AddMedicamentToDB(med);

        ui->errorLabel->clear();
        ui->countMedicamentValue->setText(QString::number(MedicamentsList.length()));
    }
}

void MainWindow::on_move_order_Button_clicked()
{
    setStackedPage(8);
}

void MainWindow::on_releabeling_Button_clicked()
{
    setStackedPage(9);
}

void MainWindow::on_unit_pack_Button_clicked()
{
    setStackedPage(10);
}

void MainWindow::SendParamsToVideoJet()
{
    QString printerdate = getGuiExperyDate().toString("yyMMdd") ;
    QString humandate = getGuiExperyDate().toString("dd.MM.yyyy") ;
    QString randstr = generateSN(11);
    QString a = QString("SLA|%1|gtinvalue=%2|batchvalue=%3|expdatevalue=%4|exphumandatevalue=%5|TNVEDvalue=%6|randomvalue=%7|").arg(VideoJetFileName, getGuiGTIN(), getGuiBatchValue(), printerdate, humandate,getGuiTNVED() , randstr);
    qDebug() << a ;
    SendCommandToVideoJet(a);
}

void MainWindow::SendRandomToVideoJet()
{
    QString printerdate = getGuiExperyDate().toString("yyMMdd") ;
    QString humandate = getGuiExperyDate().toString("dd.MM.yyyy") ;
    QString randstr = generateSN(11);
    QString a = QString("SLA|%1|gtinvalue=%2|batchvalue=%3|expdatevalue=%4|exphumandatevalue=%5|TNVEDvalue=%6|randomvalue=%7|").arg(VideoJetFileName, getGuiGTIN(), getGuiBatchValue(), printerdate, humandate,getGuiTNVED() , randstr);
    qDebug() << a ;
    SendCommandToVideoJet(a);
}

void MainWindow::on_pushButton_2_clicked()
{
    SendParamsToVideoJet();
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
    OrgText->setFont(QFont("Helvetica", 25, QFont::Bold ));
    all_etiketka.addItem(OrgText);

    AddressItem = new QGraphicsTextItem("623704, Свердловская область, г. Березовский, ул. Кольцевая, 13а");
    AddressItem->setPos(100,1000);
    AddressItem->setRotation(-90);
    AddressItem->setFont(QFont("Helvetica", 20 , QFont::Bold));
    all_etiketka.addItem(AddressItem);


    PreparatItem = new QGraphicsTextItem("Лефлуномид");
    PreparatItem->setPos(140,800);
    PreparatItem->setRotation(-90);
    PreparatItem->setFont(QFont("Helvetica", 60 , QFont::Bold));
    all_etiketka.addItem(PreparatItem);

    Dose = new QGraphicsTextItem("таблетки 10 мг №30");
    Dose->setPos(230,700);
    Dose->setRotation(-90);
    Dose->setFont(QFont("Helvetica", 25 , QFont::Bold));
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
    kolichestvouoakovok->setFont(QFont("Helvetica", 25 , QFont::Bold));
    all_etiketka.addItem(kolichestvouoakovok);


    GTIN = new QGraphicsTextItem("GTIN: ");
    GTIN ->setPos(400 + vertikotstup,1275);
    GTIN ->setRotation(-90);
    GTIN ->setFont(QFont("Helvetica", 25 , QFont::Bold));
    all_etiketka.addItem(GTIN );

    Seria = new QGraphicsTextItem("Серия: ");
    Seria ->setPos(400 + vertikotstup*2,1275);
    Seria ->setRotation(-90);
    Seria ->setFont(QFont("Helvetica", 25 , QFont::Bold));
    all_etiketka.addItem(Seria );

    dataproizvodstva = new QGraphicsTextItem("Дата производства: ");
    dataproizvodstva ->setPos(400 + vertikotstup*3,1275);
    dataproizvodstva ->setRotation(-90);
    dataproizvodstva ->setFont(QFont("Helvetica", 25 , QFont::Bold));
    all_etiketka.addItem(dataproizvodstva );

    srokgodnosti = new QGraphicsTextItem("Срок годности: ");
    srokgodnosti ->setPos(400 + vertikotstup*4,1275);
    srokgodnosti ->setRotation(-90);
    srokgodnosti ->setFont(QFont("Helvetica", 25 , QFont::Bold));
    all_etiketka.addItem(srokgodnosti);


    usloviahranenia = new QGraphicsTextItem("Хранить и транспортировать \nпри температуре от 15 до 30 C°");
    usloviahranenia ->setPos(400 + vertikotstup*5,1275);
    usloviahranenia ->setRotation(-90);
    usloviahranenia ->setFont(QFont("Helvetica", 25 , QFont::Bold));
    all_etiketka.addItem(usloviahranenia);

    SSCCCode = new QGraphicsTextItem(ssccString);
    SSCCCode ->setPos(810,490);
    SSCCCode ->setRotation(-90);
    SSCCCode ->setFont(QFont("Helvetica", 25 , QFont::Bold));
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
    OrgText->setFont(QFont("Helvetica", 25, QFont::Bold ));
    all_etiketka.addItem(OrgText);

    AddressItem = new QGraphicsTextItem(Addresstext);
    AddressItem->setPos(100,1000);
    AddressItem->setRotation(-90);
    AddressItem->setFont(QFont("Helvetica", 20 , QFont::Bold));
    all_etiketka.addItem(AddressItem);


    PreparatItem = new QGraphicsTextItem(PreparatText);
    PreparatItem->setPos(140,800);
    PreparatItem->setRotation(-90);
    PreparatItem->setFont(QFont("Helvetica", 60 , QFont::Bold));
    all_etiketka.addItem(PreparatItem);

    Dose = new QGraphicsTextItem(Dosetext);
    Dose->setPos(230,700);
    Dose->setRotation(-90);
    Dose->setFont(QFont("Helvetica", 25 , QFont::Bold));
    all_etiketka.addItem(Dose);

    mainrect = new QGraphicsRectItem(0,0,850,1270);
    mainrect->setPos(10,10);
    mainrect->setPen(QPen(Qt::black,3));
    all_etiketka.addItem(mainrect);

    QPixmap pixmap("C:/Work/Application/BFZLogo.jpg");
    //    QPixmap pixmap("C://Work/Application//BFZLogo.bmp");

    logo = new QGraphicsPixmapItem(pixmap);
    logo ->setPos(15,1275);
    logo->setRotation(-90);
    logo->setScale(0.25);
    all_etiketka.addItem(logo);

    kolichestvouoakovok = new QGraphicsTextItem("Количество упаковок: " + QString::number(kolvoupakovoktext));
    kolichestvouoakovok->setPos(300,1275);
    kolichestvouoakovok->setRotation(-90);
    kolichestvouoakovok->setFont(QFont("Helvetica", 25 , QFont::Bold));
    all_etiketka.addItem(kolichestvouoakovok);


    GTIN = new QGraphicsTextItem("GTIN: " + gtinText);
    GTIN ->setPos(400 + vertikotstup,1275);
    GTIN ->setRotation(-90);
    GTIN ->setFont(QFont("Helvetica", 25 , QFont::Bold));
    all_etiketka.addItem(GTIN );

    Seria = new QGraphicsTextItem("Серия: " + SeriaText);
    Seria ->setPos(400 + vertikotstup*2,1275);
    Seria ->setRotation(-90);
    Seria ->setFont(QFont("Helvetica", 25 , QFont::Bold));
    all_etiketka.addItem(Seria );

    dataproizvodstva = new QGraphicsTextItem("Дата производства: " + dataproizvodstvaText);
    dataproizvodstva ->setPos(400 + vertikotstup*3,1275);
    dataproizvodstva ->setRotation(-90);
    dataproizvodstva ->setFont(QFont("Helvetica", 25 , QFont::Bold));
    all_etiketka.addItem(dataproizvodstva );

    srokgodnosti = new QGraphicsTextItem("Срок годности: " + SrokgodnostiText);
    srokgodnosti ->setPos(400 + vertikotstup*4,1275);
    srokgodnosti ->setRotation(-90);
    srokgodnosti ->setFont(QFont("Helvetica", 25 , QFont::Bold));
    all_etiketka.addItem(srokgodnosti);


    usloviahranenia = new QGraphicsTextItem(usloviahraneniaText);
    usloviahranenia ->setPos(400 + vertikotstup*5,1275);
    usloviahranenia ->setRotation(-90);
    usloviahranenia ->setFont(QFont("Helvetica", 25 , QFont::Bold));
    all_etiketka.addItem(usloviahranenia);

    SSCCCode = new QGraphicsTextItem(ssccString);
    SSCCCode ->setPos(810,570);
    SSCCCode ->setRotation(-90);
    SSCCCode ->setFont(QFont("Helvetica", 25 , QFont::Bold));
    all_etiketka.addItem(SSCCCode);

    all_etiketka.update();
}

void MainWindow::on_batchnumberText_textChanged()
{
    if (ui->batchnumberText->toPlainText().length()>maxserialnumberlenght)
    {
        QString s = ui->batchnumberText->toPlainText();
        qDebug() << s;
        s.truncate(maxserialnumberlenght);
        qDebug() << s;
        ui->batchnumberText->clear();
        ui->batchnumberText->appendPlainText(s);
    }
}

void MainWindow::on_pushButton_clicked()
{
    SQLInit();
}
