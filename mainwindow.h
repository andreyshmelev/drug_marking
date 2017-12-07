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
#include "QRCodeGenerator.h"
#include "manufacturer.h"
#include "medicament.h"
#include "basetypes.h"
#include "sql.h"

namespace Ui {
class MainWindow;
}

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

    const QString TCPaddress = "192.168.1.196";
    const int TCPPort = 3002;

    const QString SSCCGS1Prefix = "4649728";

    manufacturer *getcompany() const;
    void setcompany(manufacturer *value);



    bool getRunningBuisenessProcess() const;
    void setRunningBuisenessProcess(bool value);

    void GetCompaniesList(QString company_inn, QString fromcompany, QString company_owner_id, QString company_email, QString wherecompany, QString company_kpp, QString companyname, QString company_subject_id, QString company_ul, QString company_fl);
    void GetCompaniesDBList();


public slots:

    void CreateXML311Doc(QList<medicament *> MedList, manufacturer * sender, manufacturer * owner,  int ordertype , QDateTime operation_date);
    void CreateXML312Doc(QList<medicament *> MedList , uint8_t controlsamplestype);
    void CreateXML313Doc(manufacturer * organization, QList<medicament *> MedList);
    void CreateXML415Doc(QList<medicament *> MedList, manufacturer * companyreciver, manufacturer * companysender, QDateTime operation_date, QString DocNum, QDate doc_date, int turnovertype, int source, int contracttype, QString Price, QString Vat);
    void StartAgregation();
    void StopAgregation();
    bool getLanguageswitcher() const;
    void setLanguageswitcher(bool value);

private:
    QString getGuiGTIN();
    QString getSN();
    QString generateSN(int lenght);
    QString getGuiBatchNumber();
    QString getGuiBatchValue();
    QString getGuiExpery();
    QString getGuiTNVED();
    QDateTime getGuiExperyDate();

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

    manufacturer * Organizacia;
    medicament * ScannedMedicament;

    QList<medicament *> MedicamentsList;
    QList<medicament *> MedicamentsListFromDB;
    QList<manufacturer *> CompaniesListFromDB;

    QStringList drugs;
    QStringList companies;

    int temp;

private:

    SQL  * sqlDB;
    QTimer * journalTimer;
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
    const QString VideoJetFileName = "JET8510BFZTemplatev2";

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

    static QString GetISODate();
    static QString GetDOCDate();
    void addXMLTextNode(QDomElement reg_end_pack_elem, QString nodevalue, QString nodename, QDomDocument document);
    QImage QRCodeToQLabelConverter(QLabel* dddd,QString textcode, int scale, int versionIndex, int levelIndex, bool bExtent, int maskIndex);

protected:
    bool eventFilter(QObject* obj, QEvent* event);


private slots:
    void addMessageToJournal();
    void updateTimeDate();
    void updateReadedDMCode();
    void ParseHandScannerData( QString stringtoparse);
    void updateDMPicture();
    void updateDMcode();
    void Toggle313Process( void );
    void updateAgregationGUI();
    void AddMedicamentToTable(medicament * m);
    void AddMedicamentToDB(medicament * m);
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

    void GetMedicament (medicament * m);

    void on_move_order_Button_clicked();

    void on_releabeling_Button_clicked();

    void on_unit_pack_Button_clicked();


    void on_pushButton_2_clicked();

    void on_agregationStartButton_clicked();

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
    QByteArray QstringToQbytearray(QString str);
    QElapsedTimer SQLInsertSpeedTest();
    QElapsedTimer SQLSelectSpeedTest();
    void SendCommandToVideoJet(QString a);
    void AddHandScannerLOG();
    bool languageswitcher;
    bool runningBuisenessProcess;
    void SendParamsToVideoJet();
    void SendRandomToVideoJet();
};

#endif // MAINWINDOW_H
