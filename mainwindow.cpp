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
    journalTimer->start();

    messages = new QStringList();
    messages->append("Брак");
    messages->append("Ок");
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

    qDebug() << val;
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
