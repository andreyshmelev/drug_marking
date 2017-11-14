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
    QString getGuiBatch();
    QString getGuiExpery();
    QString getGuiTNVED();
    void SetSN( QString  newSN);

private:

    QTimer * journalTimer;
    QTimer * datetimeTimer;
    Ui::MainWindow *ui;
    QStringList * messages;

    QString SN;
    QPixmap image;
    QImage  *imageObject;
    QGraphicsScene *scene;
    QGraphicsView * view;
    QSignalMapper * signalMapper;


private slots:
    void addMessageToJournal();
    void updateTimeDate();
    void updateDMPicture();
    void updateDMcode();
    QString GenerateDMcode();
    void setStackedPage(int newindex);

    // QObject interface
public:

};

#endif // MAINWINDOW_H
