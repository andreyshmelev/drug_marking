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

int t;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    updateQRLabels();

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

    connect(ui->agregationStartButton, SIGNAL(pressed()), this, SLOT(toggleAgregation())) ;
    connect(this, SIGNAL(agregationstatusToggled()), this, SLOT(updateAgregationGUI())) ;
    connect(this, SIGNAL(ParcingEnded()), this, SLOT(updateAgregationGUI())) ;
    connect(this, SIGNAL(ParcingEnded()), this, SLOT(updateTable())) ;

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

    agregation = false;

    // создаём производителя БФЗ
    BFZ = new manufacturer ( QString (" 343 374 35 66" ),QString ("ЗАО \"Берёзовский фармацевтический завод\""),QString ("info@uralbfz.ru"), QString ("6604012225"), QString ("6604012225"), QString ("6604012225"), QString ("667801001"), QString ("contractownerID") );
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
    const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
    const int randomStringLength = SNlenght; // assuming you want random strings of 12 characters

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

    if (ui->journalList->count()>100)
    {
        ui->journalList->clear();
    }

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

    QString filepath = QDir::currentPath()   + "/313-register_product_emission.xml";

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

    // QDesktopServices::openUrl(filepath);  // раскомментить если мы хотим чтобы по окончании агрегации открывался XML файл
}

void MainWindow::CreateXML312Doc(manufacturer *organization, QList<medicament *> MedList)
{
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

    control_samples_type_text.setNodeValue( QString::number( ControlSamplesTypeEnum::ToQualityControl ) );
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

    QString filepath = QDir::currentPath()   + "/312-register_control_samples.xml";

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

void MainWindow::CreateXML311Doc(manufacturer *organization, QList<medicament *> MedList, OrderTypeEnum ordertype)
{

    if (MedList.length() <=0)
        return ;

    QDomDocument document;
    QDomElement root = document.createElement("documents");
    document.appendChild(root);

    QDomElement reg_end_pack_elem  = document.createElement("register_end_packing");
    reg_end_pack_elem.setAttribute("action_id", ActionIDTypeEnum::RegisterEndPacking);
    root.appendChild(reg_end_pack_elem);

    // добавляем subject_id
    addXMLTextNode(reg_end_pack_elem,  organization->get_subject_id() , "subject_id", document);
    // добавили subject_id

    // добавляем operation_date
    addXMLTextNode(reg_end_pack_elem,  GetISODate() , "operation_date", document);
    // добавили operation_date

    // добавляем order_type
    addXMLTextNode(reg_end_pack_elem,  QString::number(ordertype) , "order_type", document);
    // добавили order_type

    // если у нас контрактное производство то мы вводим идентификатор собственника
    if (ordertype == ContractProduction)
    {
        // добавляем owner_id
        addXMLTextNode(reg_end_pack_elem, organization->get_owner_id(), "owner_id", document);
        // добавили owner_id
    }

    // добавляем series_number - номер производственной серии (не серийник потребит.упак. а именно партия)
    addXMLTextNode(reg_end_pack_elem, MedicamentsList.at(0)->BatchNumber, "series_number", document);
    // добавили series_number


    // добавляем expiration_date - срок годности препарата
    addXMLTextNode(reg_end_pack_elem, MedicamentsList.at(0)->ExperyDate, "expiration_date", document);
    // добавили expiration_date

    // добавляем gtin - срок годности препарата
    addXMLTextNode(reg_end_pack_elem, MedicamentsList.at(0)->GTIN, "gtin", document);
    // добавили  gtin

    // добавляем tnved_code
    addXMLTextNode(reg_end_pack_elem, MedicamentsList.at(0)->TNVED, "tnved_code", document);
    // добавили  tnved_code



    // добавляем signs (для первичной агрегации это sGTINs) а для вторичной это SSCC

    QDomElement signs_element  = document.createElement("signs");
    reg_end_pack_elem.appendChild(signs_element);


    // следуя документу, sgtin  - Индивидуальный серийный номер вторичной упаковки, то есть серийный номер (который генерируется)
    // добавляем sgtin

    for (int var = 0; var < MedList.length(); ++var) {

        addXMLTextNode(signs_element, MedList.at(var)->sGTIN, "sgtin", document);

    }

    //addXMLTextNode(sgtin_element,MedList.at(var)->sGTIN, "sgtin", document);

    // добавили signs

    QString filepath = QDir::currentPath()   + "/311-register_end_packing.xml";

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

void MainWindow::ParseDMCode(QString stringforparse)
{
    if (stringforparse == startcodestring) // если считали QR код начала агрегации то запускаем режим агрегации
    {
        setAgregation(1);
        return;
    }
    if (stringforparse == stopcodestring) // если считали QR код окончания агрегации то останавливаем режим агрегации
    {
        setAgregation(0);
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

    if(getAgregation()) // если у нас агрегация
    {
        ScannedMedicament = new medicament("Nimesulid",gtinstring,SNstring,batchstring,expstring,sGTINString,tnvedstring);
        MedicamentsList.append(ScannedMedicament);
        emit ParcingEnded(); // испускаем сигнал что закончили парсинг строки
    }
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
    if (set == true) // если запускаем агрегацию
    {
        inputDataStringFromScaner.clear();
    }
    else    // если останавливаем агрегацию
    {
        CreateXML311Doc(BFZ,MedicamentsList,OrderTypeEnum::ContractProduction);
        CreateXML312Doc(BFZ,MedicamentsList);
        CreateXML313Doc(BFZ,MedicamentsList);
        inputDataStringFromScaner.clear();
    }

    MedicamentsList.clear();

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

        ui->MedicamentsTable->clearContents();

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

void MainWindow::updateTable()
{
    ui->MedicamentsTable->insertRow(0);
    ui->MedicamentsTable->setItem(0, 0, new QTableWidgetItem(ScannedMedicament->medicament_name));
    ui->MedicamentsTable->setItem(0, 1, new QTableWidgetItem(ScannedMedicament->GTIN));
    ui->MedicamentsTable->setItem(0, 2, new QTableWidgetItem(ScannedMedicament->BatchNumber));
    ui->MedicamentsTable->setItem(0, 3, new QTableWidgetItem(ScannedMedicament->SerialNumber));
    ui->MedicamentsTable->setItem(0, 4, new QTableWidgetItem(ScannedMedicament->TNVED));
    ui->MedicamentsTable->setItem(0, 5, new QTableWidgetItem(ScannedMedicament->ExperyDate));
    ui->MedicamentsTable->scrollToTop();
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
}

QImage MainWindow::QRCodeToQimage(QLabel* dddd, QString code, int qrImageSize, int versionIndex, int levelIndex, bool bExtent, int maskIndex)
{
    successfulEncoding = qrEncode.EncodeData( levelIndex, versionIndex, bExtent, maskIndex, code.toUtf8().data() );
    if ( !successfulEncoding )
    {
        ui->register_control_samples_Label->clear();
        ui->register_control_samples_Label->setText( tr("QR Code...") );
    }

    qrImageSize = qrEncode.m_nSymbleSize;

    // Создаем двумерный образ кода

    encodeImageSize = qrImageSize + ( QR_MARGIN * 2 );
    QImage encodeImage2( encodeImageSize, encodeImageSize, QImage::Format_Mono );
    encodeImage2.fill( 1 );

    // Создать двумерный образ кода
    for ( int i = 0; i < qrImageSize; i++ )
        for ( int j = 0; j < qrImageSize; j++ )
            if ( qrEncode.m_byModuleData[i][j] )
                encodeImage2.setPixel( i + QR_MARGIN, j + QR_MARGIN, 0 );

    dddd->setPixmap( QPixmap::fromImage( encodeImage2 ) );

    return encodeImage2;
}

void MainWindow::updateQRLabels()
{
    int levelIndex = 1;
    int versionIndex = 0;
    bool bExtent = true;
    int maskIndex = -1;

    successfulEncoding = qrEncode.EncodeData( levelIndex, versionIndex, bExtent, maskIndex, register_product_emission_QR_string.toUtf8().data() );
    if ( !successfulEncoding )
    {
        ui->register_product_emission_QRLabel->clear();
        ui->register_product_emission_QRLabel->setText( tr("QR Code...") );
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

    ui->register_product_emission_QRLabel->setPixmap( QPixmap::fromImage( encodeImage ) );

    ////

     QLabel  * dddd = ui->register_control_samples_Label;

    QImage encodeImage2 = QRCodeToQimage(dddd, "register_control_samples_QR_string", qrImageSize, versionIndex, levelIndex, bExtent, maskIndex);


    encodeImage2= encodeImage2;
    ////



    // увеличиваем в размерах
    if ( successfulEncoding )
    {
        int scale_size = encodeImageSize * 2;

        QPixmap scale_image = ui->register_product_emission_QRLabel->pixmap()->scaled( scale_size, scale_size );
        ui->register_product_emission_QRLabel->setPixmap( scale_image );
        scale_image = ui->register_control_samples_Label->pixmap()->scaled( scale_size, scale_size );
        ui->register_control_samples_Label->setPixmap( scale_image );
    }

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
            return QObject::eventFilter(obj, event);
        }
        return true;
    } else {
        return QObject::eventFilter(obj, event);
    }
    return false;
}

void MainWindow::openRegisterProductEmissionPage()
{
    setStackedPage(7);
}

void MainWindow::on_register_product_emission_Button_clicked()
{
    openRegisterProductEmissionPage();
}
