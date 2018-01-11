#include "widget313.h"
#include "ui_widget313.h"

widget313::widget313(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::widget313)
{
    ui->setupUi(this);
}

widget313::~widget313()
{
    delete ui;
}
