#include "refusalsenderwidget251.h"
#include "ui_refusalsenderwidget251.h"

RefusalSenderWidget251::RefusalSenderWidget251(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RefusalSenderWidget251)
{
    ui->setupUi(this);
}

RefusalSenderWidget251::~RefusalSenderWidget251()
{
    delete ui;
}
