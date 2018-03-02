#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QStringList>
#include <QFileDialog>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QXmlStreamAttribute>
#include <QMessageBox>
#include <QFile>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QSignalMapper>
#include <QPixmap>
#include <QtXml>
#include <QtSql>
#include <QTcpSocket>
#include <QGraphicsScene>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "code128item.h"
#include "QRCodeGenerator.h"
#include "manufacturer.h"
#include "medicament.h"
#include "basetypes.h"
#include "sql.h"
#include "serializationline.h"
#include "apimark.h"


namespace Ui {
class MainWindow;
}


class eticetka
{
public:
    eticetka();
    eticetka(QString OrgTextstring, QString Dosetext,   QString Addresstext, QString PreparatText, int kolvoupakovoktext, QString gtinText, QString SeriaText,  QString dataproizvodstvaText,  QString SrokgodnostiText, QString usloviahraneniaText, QString regnomerText, QString SSCCCodetext  );

    QGraphicsRectItem* mainrect ;
    QGraphicsRectItem* logorect ;
    QGraphicsTextItem* OrgText ;
    QGraphicsTextItem* Dose ;
    QGraphicsTextItem* AddressItem;
    QGraphicsTextItem* PreparatItem;
    QGraphicsTextItem* kolichestvouoakovok;
    QGraphicsTextItem* GTIN;
    QGraphicsTextItem* Seria;
    QGraphicsTextItem* dataproizvodstva;
    QGraphicsTextItem* srokgodnosti;
    QGraphicsTextItem* usloviahranenia;
    QGraphicsTextItem* regnomer;
    QGraphicsTextItem* SSCCCode;

    QGraphicsScene m_Scene;
    QGraphicsScene all_etiketka;
    Code128Item * m_Barcode;
    QGraphicsPixmapItem * logo;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    int GenerateNumber(int High, int Low);
    static QString GetRegularString(QString stringforparse, QString SNRegularexpression);
    static QImage QRCodeToQImageConverter(QString textcode, int scale, int versionIndex, int levelIndex, bool bExtent, int maskIndex);

    //        const QString TCPaddress = "127.0.0.1";
    //       const int TCPPort = 1234;

    const QString TCPaddress = "192.168.1.180";
    const int TCPPort = 2030;

    const QString SSCCGS1Prefix = "4649728";

    manufacturer *getSerializationCompanySender() const;
    void SetSerializationCompanySender(manufacturer *value);


    eticetka * EticetkaBFZ;

    bool getRunningBuisenessProcess() const;
    void setRunningBuisenessProcess(bool value);
    void GetCompaniesList(QString company_inn, QString fromcompany, QString company_owner_id, QString company_email, QString wherecompany, QString company_kpp, QString companyname, QString company_subject_id, QString company_ul, QString company_fl);
    void GetCompaniesDBList();
    void GetStatisticsFromDB();
    bool IsDateProper(QString stringtotest);
    void SQLInit();

    bool getAutoupakovka() const;
    void setAutoupakovka(bool value);

    bool getAutoagregation() const;
    void setAutoagregation(bool value);

    bool getAutoprogramagregation() const;
    void setAutoprogramagregation(bool value);

    QString getSerializationGTIN() const;
    void setSerializationGTIN(const QString &value);

    QString getSerializationSN() const;
    void setSerializationSN(const QString &value);

    QString getSerializationBatchName() const;
    void setSerializationBatchName(const QString &value);

    QString getSerializationExpery() const;
    void setSerializationExpery(const QString &value);

    QString getSerializationDrugName() const;
    void setSerializationDrugName(const QString &value);

    QString getSerializationDose() const;
    void setSerializationDose(const QString &value);

    QString getSerializationTNVED() const;
    void setSerializationTNVED(const QString &value);

    void GUIMainWindowUpdate();

    QString getSerializationQuantity() const;
    void setSerializationQuantity(const QString &value);

    QString getSerializationBatchValue() const;
    void setSerializationBatchValue(const QString &value);

    int getSerializationOrderType() const;
    void setSerializationOrderType(int value);

    manufacturer *getSerializationCompanyOwner() const;
    void setSerializationCompanyOwner(manufacturer *value);

    QDateTime GetISODateTime();

    bool getBSerializationStarted() const;
    void setBSerializationStarted(bool value);

    bool getBSerializationStopped() const;
    void setBSerializationStopped(bool value);

    bool getBSerializationPaused() const;
    void setBSerializationPaused(bool value);

    void ShowMessageBox(QString message);

    void ReleabilingPageOpen();

    void ISMArkPageOpen();
    void ClearApiLog();

    void MakeStatisticsPDFReport();

    void UnitUnpackPageOpen();

    void UnitExtractPageOpen();

public slots:

    void CreateXML311Doc(QList<medicament *> MedList, manufacturer * sender, manufacturer * owner,  int ordertype , QDateTime operation_date);
    void CreateXML312Doc(QList<medicament *> MedList , quint8 controlsamplestype);
    void CreateXML313Doc(QList<medicament *> MedList, QDateTime operation_date);
    void CreateXML415Doc(QList<medicament *> MedList, manufacturer * companyreciver, manufacturer * companysender, QDateTime operation_date, QString DocNum, QDate doc_date, int turnovertype, int source, int contracttype, QString Price, QString Vat);
    void CreateXML811Doc(QList<medicament *> MedListOld, QList<medicament *> MedListNew, manufacturer * companysender, QDateTime operation_date);
    void CreateXML911Doc(QList<medicament *> MedList, manufacturer * companysender, QDateTime operation_date);
    void StartAgregation();
    void StopAgregation();
    bool getLanguageswitcher() const;
    void setLanguageswitcher(bool value);
    void Getmessage(QString m);
    void PrintSSCCCode(QString newcode);
    bool PrintBIGEtiketka(eticetka * et);
    void AddMedicamentToDBTable(medicament *m, QString tablename);
    void AddStatisticsToDB(QString bisnessprocessname, medicament *m, QDateTime datetime, int count,QString XMLFileName);
    void AddStatisticsToDB(QString bisnessprocessname, QString GTIN, QString medicament_name, QString BatchNumber, QDateTime datetime, int count, QString XMLFileName);

    //слоты сериализации

    void StartSerialization();
    void StopSerialization();
    void PauseSerialization();
    void ContinueSerialization();

    void GetCompaniesDBList(QList<manufacturer *> man);
    void ResponseFromLineRecieved(QString address, quint16 port, QString message);
    void DrugRecievedFromEmulator(QString BatchName, QString ExperyDate, QString GTIN, QString SerialNumber, QString Tnved);


    void replyfinished(QNetworkReply *reply);
    void ADDApiLOG(QString Logstring);
private:

    QString getGuiGTIN();
    QString getSN();
    QString generateSN(int lenght);
    QString generateCode128 (int lenght);
    QString getGuiBatchNumber();
    QString getGuiBatchValue();
    QString getGuiExpery();
    QString getGuiTNVED();
    QString getGuiDrugsName();
    QString getGuiDose();
    QDateTime getGuiExperyDate();

    bool autoupakovka;
    bool autoagregation;
    bool autoprogramagregation;

    void updateQRImage();
    void updateQRLabels();
    void addSymbolToInputString(QString str);
    void setScale(int);

    void SetSN( QString  newSN);
    CQR_Encode qrEncode;
    bool successfulEncoding;
    int encodeImageSize;
    bool getAgregation(void);
    void Start313Process(bool set);

    const quint8 maxserialnumberlenght = 7;
    manufacturer * SerializationCompanySender;
    manufacturer * SerializationCompanyOwner;
    medicament * ScannedMedicament;

    int SerializationOrderType;

    QList<medicament *> MedicamentsList;
    QList<medicament *> MedicamentsListFromDB;
    QList<medicament *> MedicamentsSerialization;
    QList<manufacturer *> CompaniesListFromDB;

    QStringList drugs;
    QStringList companies;

    QStringList statisticsbisnessprocesses;
    QStringList statisticsmedicaments;
    QStringList statisticsbatches;

    int temp;
    QGraphicsScene m_Scene;
    Code128Item * m_Barcode;

private:

    SQL  * sqlDB;
    QTimer * ScannerLiniaEmulate;
    QTimer * datetimeTimer;
    QTimer * DMCodeUpdateTimeoutTimer;
    QTimer *RandomStringSenderToVideoJetTimer;
    Ui::MainWindow *ui;
    QStringList * messages;
    QString KeyspressedString;
    QString SN;
    QPixmap image;
    QImage  *imageObject;
    QGraphicsScene *scene;
    QGraphicsView * view;
    QSignalMapper * signalMapper;
    QString keyString;
    QString inputDataStringFromScaner;
    QPixmap * pixmapqr;
    APIMARK * apiclient;
    bool agregation;

    // сгенерированные QR коды

    const QString register_product_emission_QR_string = "497157";
    const QString register_control_samples_QR_string = "709109";
    const QString register_end_packing_QR_string = "914088";
    const QString unit_pack_QR_string = "960559";
    const QString move_order_QR_string = "233002";
    const QString releabeling_QR_string = "530848";

    const QString printControlQRCode = "306656";
    const QString programOptionsQRCode = "783146";
    const QString agregationQRCode = "793817";
    const QString statisticsQRCode = "643701";

    const QString Start311ProcessQRString = "501301";
    const QString Stop311ProcessQRString  = "641195";
    const QString Start312ProcessQRString = "909628";
    const QString Stop312ProcessQRString  = "272080";
    const QString Start313ProcessQRString = "085987";
    const QString Stop313ProcessQRString = "883940";

    const QString Start415ProcessQRString = "497222";
    const QString Stop415ProcessQRString  = "831533";
    const QString Start811ProcessQRString = "091653";
    const QString Stop811ProcessQRString = "831770";
    const QString Start911ProcessQRString = "970500";
    const QString Stop911ProcessQRString = "473786";

    const QString GTINid = "01";
    const QString SNid = "21";
    const QString Batchid = "10";
    const QString Experyid = "17";
    const QString TNVEDid = "240";

    const QString GSSymbol = " ";
    const QString Emptystring = "";

    const QString SNRegularexpression = "21\\w{13}" ; // строка начинается  с 21, имеет длину 13 и начинается символом разделителя
    const QString TNVEDRegularexpression =  GSSymbol + "240\\w{4}" ;
    const QString ExpRegularexpression = GSSymbol +  "17\\w{6}" ;
    const QString BatchRegularexpression = GSSymbol + "10\\w{1,20}";
    const QString NotFoundString = "Not found";

    const uint Gtinlenght = 14;
    const uint SNlenght = 13 ;
    const uint ExpLenght = 6;
    const uint TNVEDLenght = 4;

    QString sGTINString;
    QString gtinstring;
    QString SNstring;
    QString batchstring;
    QString expstring;
    QString tnvedstring;

    // параметры сериализации

    QString SerializationGTIN;
    QString SerializationSN;
    QString SerializationBatchName;
    QString SerializationBatchValue;
    QString SerializationExpery;
    QString SerializationDrugName;
    QString SerializationDose;
    QString SerializationTNVED;
    QString SerializationQuantity;

    bool bSerializationStarted;
    bool bSerializationStopped;
    bool bSerializationPaused;

    static QString GetISODate();
    static QString GetDOCDate();
    void addXMLTextNode(QDomElement reg_end_pack_elem, QString nodevalue, QString nodename, QDomDocument document);
    QImage QRCodeToQLabelConverter(QLabel* dddd,QString textcode, int scale, int versionIndex, int levelIndex, bool bExtent, int maskIndex);

protected:
    bool eventFilter(QObject* obj, QEvent* event);

private slots:

    QString GenerateDMcode();

    void addMessageToJournal(QString message, QColor textcolor,QColor backcolor);
    void updateTimeDate();
    void updateReadedDMCode();
    void ParseHandScannerData( QString stringtoparse);
    void updateDMPicture();
    void EmulateAutomaticMedicamentScan();
    void Toggle313Process( void );
    void updateAgregationGUI();
    bool CheckMedicamentinDB(medicament * m);
    void setStackedPage(int newindex);
    void RegisterProductEmissionPageOpen();
    void RegisterControlSamplesPageOpen();
    void RegisterEndPackingPageOpen();
    void PrintControlPageOpen();
    void productOptionsPageOpen();
    void agregationOptionsPageOpen();
    void statisticsPageOpen();
    void connectTcp(QString address, int port);
    void serverWrite(QString str);
    void GetMedicamentSerialization (medicament * m);
    void on_register_product_emission_Button_clicked();
    void on_register_control_samples_Button_clicked();
    void on_register_end_packing_Button_clicked();
    void on_DrugsComboBox_currentIndexChanged(int index);
    void on_move_order_Button_clicked();
    void on_releabeling_Button_clicked();
    void on_unit_pack_Button_clicked();
    void on_pushButton_2_clicked();
    void on_batchnumberText_textChanged();
    void on_StatistFindButton_clicked();
    void on_SerializAutoUpakovkaCheckBox_toggled(bool checked);
    void on_SerializAutoAgregationCheckBox_toggled(bool checked);
    void on_SerializAutoAgregationProgramCheckBox_toggled(bool checked);
    void on_SetSerializationOptionsButton_clicked();
    void on_optionsButton_clicked();
    void on_keyboardButton_clicked();
    void on_sendISMarkButton_clicked();
    void on_sendFileApiButton_clicked();
    void on_AuthAPIButton_clicked();
    void on_getCurrentUserAPIButton_clicked();
    void on_sendFileApiButton_2_clicked();
    void on_getFileListApiButton_clicked();
    void on_getFileListApiButton_2_clicked();
    void on_downloadFileApiButton_clicked();
    void on_downloadFileApiButton_2_clicked();
    void on_getRulesButton_clicked();

    void on_StatistFindButton_2_clicked();

    void on_unit_unpack_Button_clicked();

    void on_unit_extract_Button_clicked();

    void on_unit_append_Button_clicked();

    void on_multi_pack_Button_clicked();

    void on_recall_Button_clicked();

    void on_refusal_sender_Button_clicked();

    void on_refusal_receiver_Button_clicked();

signals:

    void agregationstatusToggled();
    void ParcingEnded();
    void SendMedicamentSignal(medicament * );
    void SendCompaniesDBList(QList<manufacturer *> );
    void register_product_emission_QR_Scanned();
    void register_control_samples_QR_Scanned();
    void register_end_packing_QR_Scanned();
    void printControlQRCodeScanned();
    void programOptionsQRCodeScanned();
    void agregationQRCodeScanned();
    void statisticsQRCodeScanned();

signals:
    // для виджетов
    void Start312Process();
    void Stop312Process();
    void Start311Process();
    void Stop311Process();
    void Start313Process();
    void Stop313Process();

private:
    QTcpSocket *Socket ;
    SerializationLine *SerializationLine1;
    QByteArray QstringToQbytearray(QString str);
    QElapsedTimer SQLInsertSpeedTest();
    QElapsedTimer SQLSelectSpeedTest();
    QDateTime getoperationDate();
    void SendCommandToVideoJet(QString a);
    void AddHandScannerLOG();
    bool languageswitcher;
    bool runningBuisenessProcess;
    void SendParamsToVideoJet();
    void SendRandomToVideoJet();
    void GenerateCode128(QString newcode);

    void SetLibrariesPath();
    void SetStyleSheets();
    void UnitPackPageOpen();
    void MoveOrderPageOpen();
};

#endif // MAINWINDOW_H
