#include "multipackwidget915.h"
#include "ui_multipackwidget915.h"

MultiPackWidget915::MultiPackWidget915(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MultiPackWidget915)
{
    ui->setupUi(this);
}

MultiPackWidget915::~MultiPackWidget915()
{
    delete ui;
}
