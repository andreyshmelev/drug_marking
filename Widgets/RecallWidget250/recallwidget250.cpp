#include "recallwidget250.h"
#include "ui_recallwidget250.h"

RecallWidget250::RecallWidget250(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RecallWidget250)
{
    ui->setupUi(this);
}

RecallWidget250::~RecallWidget250()
{
    delete ui;
}
