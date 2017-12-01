#include "unitappendwidget.h"
#include "ui_unitappendwidget.h"
#include <C:\Work\Application\Application\basebuisnessclass.h>


UnitAppendWidget::UnitAppendWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UnitAppendWidget)
{
    ui->setupUi(this);
    BaseClass = new BaseBuisnessClass(this);

    BaseClass->RegistrationStarted();
}

UnitAppendWidget::~UnitAppendWidget()
{
    delete ui;
}
