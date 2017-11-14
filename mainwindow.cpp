#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    journalTimer = new QTimer();
    journalTimer->setInterval(550);

    connect(journalTimer, SIGNAL(timeout()), this, SLOT(addMessageToJournal()));
    connect(journalTimer, SIGNAL(timeout()), this, SLOT(updateDMPicture()));

    signalMapper = new QSignalMapper (this) ;
//    signalMapper -> setMapping (action1, 1) ;
//    signalMapper -> setMapping (action5, 5) ;
//    signalMapper -> setMapping (action10, 10) ;
//    signalMapper -> setMapping (action25, 25) ;
//    signalMapper -> setMapping (action50, 50) ;

    connect(ui->printControlButton, SIGNAL(pressed()), signalMapper, SLOT(map())) ;
    connect(ui->programOptionsButton, SIGNAL(pressed()), signalMapper, SLOT(map())) ;
    connect(ui->agregationButton, SIGNAL(pressed()), signalMapper, SLOT(map())) ;
    connect(ui->statisticksButton, SIGNAL(pressed()), signalMapper, SLOT(map())) ;
    connect(ui->XMLButton, SIGNAL(pressed()), signalMapper, SLOT(map())) ;

    // ПРИСВАИВАЕМ КАЖДОМУ СИГНАЛУ КНОПКИ ИНДЕКС
    signalMapper -> setMapping (ui->printControlButton, 0) ;
    signalMapper -> setMapping (ui->programOptionsButton, 1) ;
    signalMapper -> setMapping (ui->agregationButton, 2) ;
    signalMapper -> setMapping (ui->statisticksButton, 3) ;
    signalMapper -> setMapping (ui->XMLButton, 4) ;

    connect (signalMapper, SIGNAL(mapped(int)), this, SLOT(setStackedPage(int)));

    journalTimer->start();
    scene = new QGraphicsScene();
    view = new QGraphicsView(scene);
    messages = new QStringList();
    messages->append("Брак");
    messages->append("Ок");

    imageObject = new QImage();
    imageObject->load("C:/Work/Application/DM1.JPG");
    image = QPixmap::fromImage(*imageObject);

    scene = new QGraphicsScene(this);
    scene->addPixmap(image);
    scene->setSceneRect(image.rect());
    ui->packagePicture->setScene(scene);
    ui->packagePicture->show();

    setStackedPage(0);

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

void MainWindow::updateDMPicture()
{
    int val = GenerateNumber(3, 1);

    imageObject->load("C:/Work/Application/DM" + QString::number(val) + ".JPG");
    image = QPixmap::fromImage(*imageObject);
    ui->packagePicture->setScene(scene);
    scene->addPixmap(image);
    scene->setSceneRect(image.rect());
    ui->packagePicture->show();
}

void MainWindow::setStackedPage(int newindex)
{
    ui->stackedWidget->setCurrentIndex(newindex);
}
