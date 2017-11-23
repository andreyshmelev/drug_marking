#include "unitextractwidget.h"
#include "ui_unitextractwidget.h"
#include <QDebug>

UnitExtractWidget::UnitExtractWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UnitExtractWidget)
{
    ui->setupUi(this);
}

UnitExtractWidget::~UnitExtractWidget()
{
    delete ui;
}

UnitExtractWidget::GetParcedString(QString gtinstring, QString SNstring, QString tnvedstring, QString expstring, QString batchstring, QString sGTINString)
{
    qDebug() << gtinstring<< SNstring<< tnvedstring<< expstring<< batchstring<< sGTINString;
}
