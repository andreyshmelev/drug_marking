#include "unitpackwidget911.h"
#include "ui_unitpackwidget911.h"

UnitPackWidget911::UnitPackWidget911(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UnitPackWidget911)
{
    ui->setupUi(this);
}

UnitPackWidget911::~UnitPackWidget911()
{
    delete ui;
}

void UnitPackWidget911::on_pushButton_clicked()
{
    ui->pushButton->setText("qwewewe");
}
