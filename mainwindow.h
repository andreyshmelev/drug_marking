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
#include "QRCodeGenerator.h"
#include "manufacturer.h"
#include "medicament.h"
#include "basetypes.h"

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
    void getregularString(QString stringforparse, QString SNRegularexpression);
    void getRegularFunction(QString SNRegularexpression, QString stringforparse);
    void Regtr(QString stringforparse, QString matched, QString SNRegularexpression);
    QString GetRegularString(QString stringforparse, QString SNRegularexpression);

    void werwe();

    void sss(int maskIndex, int levelIndex, int versionIndex, bool bExtent, int qrImageSize);

    void sssss(int versionIndex, int qrImageSize, int levelIndex, int maskIndex, bool bExtent);

    void ssssss(int qrImageSize, QLabel* dddd);

private:
    QString getGuiGTIN();
    QString getSN();
    QString generateSN();
    QString getGuiBatchNumber();
    QString getGuiExpery();
    QString getGuiTNVED();

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
    void setAgregation(bool set);

    manufacturer * BFZ;
    medicament * ScannedMedicament;
    QList<medicament *> MedicamentsList;

private:

    QTimer * journalTimer;
    QTimer * datetimeTimer;
    QTimer * DMCodeUpdateTimeoutTimer;
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
    const QString startcodestring = "50218";
    const QString stopcodestring = "86199";
    const QString register_product_emission_QR_string = "49157";
    const QString register_control_samples_QR_string = "70109";

    const QString GTINid = "01";
    const QString SNid = "21";
    const QString Batchid = "10";
    const QString Experyid = "17";
    const QString TNVEDid = "240";

    const QString GSSymbol = "002#";
    const QString Emptystring = "";

    const QString SNRegularexpression = "21\\w{13,14}" + GSSymbol; // строка начинается  с 21, имеет длину 14 и заказчивается символом 0029
    const QString TNVEDRegularexpression = "240\\w{4}" + GSSymbol;
    const QString ExpRegularexpression = "17\\w{6}" ;
    const QString BatchRegularexpression = "10\\w{1,20}" + GSSymbol ;
    const QString NotFoundString = "Not found";

    const uint8_t Gtinlenght = 14;
    const uint8_t SNlenght = 13 ;
    const uint8_t ExpLenght = 6;
    const uint8_t TNVEDLenght = 4;

    QString sGTINString;
    QString gtinstring;
    QString SNstring;
    QString batchstring;
    QString expstring;
    QString tnvedstring;

    void CreateXML313Doc(manufacturer * organization, QList<medicament *> MedList);
    void CreateXML312Doc(manufacturer * organization, QList<medicament *> MedList);
    void CreateXML311Doc(manufacturer * organization, QList<medicament *> MedList, OrderTypeEnum ordertype);

    static QString GetISODate();
    static QString GetDOCDate();

    void addXMLTextNode(QDomElement reg_end_pack_elem, QString nodevalue, QString nodename, QDomDocument document);

    void openRegisterProductEmissionPage();



    QImage QRCodeToQimage(QLabel* dddd,QString code, int qrImageSize, int versionIndex, int levelIndex, bool bExtent, int maskIndex);

protected:
    bool eventFilter(QObject* obj, QEvent* event);


private slots:
    void addMessageToJournal();
    void updateTimeDate();
    void updateReadedDMCode();
    void ParseDMCode( QString stringtoparse);
    void updateDMPicture();
    void updateDMcode();
    void toggleAgregation( void );
    void updateAgregationGUI();
    void updateTable();
    void setStackedPage(int newindex);
    QString GenerateDMcode();

    void on_register_product_emission_Button_clicked();

signals:
    agregationstatusToggled();
    ParcingEnded();
};

#endif // MAINWINDOW_H
