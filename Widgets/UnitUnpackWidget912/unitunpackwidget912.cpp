#include "unitunpackwidget912.h"
#include "ui_unitunpackwidget912.h"

UnitUnpackWidget912::UnitUnpackWidget912(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UnitUnpackWidget912)
{
    ui->setupUi(this);
}

UnitUnpackWidget912::~UnitUnpackWidget912()
{
    delete ui;
}
