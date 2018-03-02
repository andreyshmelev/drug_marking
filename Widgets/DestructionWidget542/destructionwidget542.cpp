#include "destructionwidget542.h"
#include "ui_destructionwidget542.h"

DestructionWidget542::DestructionWidget542(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DestructionWidget542)
{
    ui->setupUi(this);
}

DestructionWidget542::~DestructionWidget542()
{
    delete ui;
}
