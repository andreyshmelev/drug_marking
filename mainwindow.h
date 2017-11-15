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
#include "QRCodeGenerator.h"

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


private:
    QString getGuiGTIN();
    QString getSN();
    QString generateSN();
    QString getGuiBatchNumber();
    QString getGuiExpery();
    QString getGuiTNVED();

    void updateQRImage();
    void addSymbolToInputString(QString str);
    void setScale(int);

    void SetSN( QString  newSN);
    CQR_Encode qrEncode;
    bool successfulEncoding;
    int encodeImageSize;
    QPoint lastPos;
    bool getAgregation(void);
    void setAgregation(bool set);

private:

    QTimer * journalTimer;
    QTimer * datetimeTimer;
    QTimer * DMCodeUpdateTimeoutTimer;
    Ui::MainWindow *ui;
    QStringList * messages;

    QString SN;
    QPixmap image;
    QImage  *imageObject;
    QGraphicsScene *scene;
    QGraphicsView * view;
    QSignalMapper * signalMapper;

    QString keyString;
    QString inputDataStringFromScaner;

    bool agregation;

protected:
    bool eventFilter(QObject* obj, QEvent* event);


private slots:
    void addMessageToJournal();
    void updateTimeDate();
    void DMCodeUpdate();
    void updateDMPicture();
    void updateDMcode();
    void toggleAgregation( void );
    void updateagregationGUI();
    void setStackedPage(int newindex);
    QString GenerateDMcode();

signals:
    agregationstatusToggled();
};

#endif // MAINWINDOW_H
