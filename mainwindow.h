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
#include "code128item.h"
#include "QRCodeGenerator.h"
#include "manufacturer.h"
#include "medicament.h"
#include "basetypes.h"
#include "sql.h"
#include "serializationline.h"

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

    const QString TCPaddress = "192.168.12.11";
    const int TCPPort = 3007;

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

public slots:

    void CreateXML311Doc(QList<medicament *> MedList, manufacturer * sender, manufacturer * owner,  int ordertype , QDateTime operation_date);
    void CreateXML312Doc(QList<medicament *> MedList , quint8 controlsamplestype);
    void CreateXML313Doc(manufacturer * organization, QList<medicament *> MedList, QDateTime operation_date);
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

    //слоты сериализации

    void StartSerialization();
    void StopSerialization();
    void PauseSerialization();
    void ContinueSerialization();

        void GetCompaniesDBList(QList<manufacturer *> man);

        void ResponseFromLineRecieved(QString address, quint16 port, QString message);
        void DrugRecievedFromEmulator(QString BatchName, QString ExperyDate, QString GTIN, QString SerialNumber, QString Tnved);
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
    QPoint lastPos;
    bool getAgregation(void);
    void Start313Process(bool set);


    const quint8 maxserialnumberlenght = 7;
    manufacturer * SerializationCompanySender;
    manufacturer * SerializationCompanyOwner;
    int SerializationOrderType;
    medicament * ScannedMedicament;

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
    bool agregation;

    // сгенерированные QR коды

    const QString register_product_emission_QR_string = "497157";
    const QString register_control_samples_QR_string = "709109";
    const QString register_end_packing_QR_string = "914088";

    const QString printControlQRCode = "306656";
    const QString programOptionsQRCode = "783146";
    const QString agregationQRCode = "793817";
    const QString statisticsQRCode = "643701";

    const QString Start311ProcessQRString = "501301";
    const QString Stop311ProcessQRString  = "641195";
    const QString Start312ProcessQRString = "909628";
    const QString Stop312ProcessQRString  = "272080";
    const QString Start313ProcessQRString = "5021";
    const QString Stop313ProcessQRString = "8619";

    const QString GTINid = "01";
    const QString SNid = "21";
    const QString Batchid = "10";
    const QString Experyid = "17";
    const QString TNVEDid = "240";

    const QString GSSymbol = "002#";
    const QString Emptystring = "";
    const QString VideoJetFileName = "BFZTest3";

    const QString SNRegularexpression = "21\\w{12,14}" + GSSymbol; // строка начинается  с 21, имеет длину 14 и заказчивается символом 0029
    const QString TNVEDRegularexpression = "240\\w{4}" + GSSymbol;
    const QString ExpRegularexpression = "17\\w{6}" ;
    const QString BatchRegularexpression = "10\\w{1,20}" + GSSymbol ;
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

    void addMessageToJournal(QString message, QColor textcolor,QColor backcolor);
    void updateTimeDate();
    void updateReadedDMCode();
    void ParseHandScannerData( QString stringtoparse);
    void updateDMPicture();
    void EmulateAutomaticMedicamentScan();
    void Toggle313Process( void );
    void updateAgregationGUI();
    void AddMedicamentToTable(medicament * m);
    bool CheckMedicamentinDB(medicament * m);
    void setStackedPage(int newindex);
    QString GenerateDMcode();

    void RegisterProductEmissionPageOpen();
    void RegisterControlSamplesPageOpen();
    void RegisterEndPackingPageOpen();

    void PrintControlPageOpen();
    void productOptionsPageOpen();
    void agregationOptionsPageOpen();
    void statisticsPageOpen();
    void on_register_product_emission_Button_clicked();
    void on_register_control_samples_Button_clicked();
    void on_register_end_packing_Button_clicked();
    void connectTcp(QString address, int port);
    void serverWrite(QString str);
    void on_DrugsComboBox_currentIndexChanged(int index);
    void GetMedicamentSerialization (medicament * m);
    void on_move_order_Button_clicked();
    void on_releabeling_Button_clicked();
    void on_unit_pack_Button_clicked();
    void on_pushButton_2_clicked();
    void on_agregationStartButton_clicked();
    void on_batchnumberText_textChanged();
    void on_pushButton_clicked();
    void on_StatistFindButton_clicked();
    void on_SerializAutoUpakovkaCheckBox_toggled(bool checked);
    void on_SerializAutoAgregationCheckBox_toggled(bool checked);
    void on_SerializAutoAgregationProgramCheckBox_toggled(bool checked);
    void on_SerializAutoUpakovkaCheckBox_stateChanged(int arg1);

    void on_optionsButton_clicked();

    void on_SetSerializationOptionsButton_clicked();

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
    void SendCommandToVideoJet(QString a);
    void AddHandScannerLOG();
    bool languageswitcher;
    bool runningBuisenessProcess;
    void SendParamsToVideoJet();
    void SendRandomToVideoJet();
    void GenerateCode128(QString newcode);

    void SetLibrariesPath();
    void SetStyleSheets();
    QDateTime getoperationDate();
};

#endif // MAINWINDOW_H
