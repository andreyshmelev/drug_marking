#include "unitappendwidget.h"
#include "ui_unitappendwidget.h"

UnitAppendWidget::UnitAppendWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UnitAppendWidget)
{
    ui->setupUi(this);
}

UnitAppendWidget::~UnitAppendWidget()
{
    delete ui;
}
