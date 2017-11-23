#include "unitappendwidget.h"
#include "ui_unitappendwidget.h"
#include <QDebug>

UnitAppendWidget::UnitAppendWidget(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UnitAppendWidget)
{
    ui->setupUi(this);
}

UnitAppendWidget::~UnitAppendWidget()
{
    delete ui;
}

void UnitAppendWidget::getparsedStringemit(QString gtinstring, QString SNstring, QString tnvedstring, QString expstring, QString batchstring, QString sGTINString)
{
    qDebug() <<gtinstring <<SNstring <<tnvedstring <<expstring <<batchstring <<sGTINString ;
}
