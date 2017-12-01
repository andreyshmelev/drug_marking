#include "relabelingwidget811.h"
#include "ui_relabelingwidget811.h"

RelabelingWidget811::RelabelingWidget811(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RelabelingWidget811)
{
    ui->setupUi(this);
}

RelabelingWidget811::~RelabelingWidget811()
{
    delete ui;
}
