#include "withdrawalwidget552.h"
#include "ui_withdrawalwidget552.h"

WithdrawalWidget552::WithdrawalWidget552(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WithdrawalWidget552)
{
    ui->setupUi(this);
}

WithdrawalWidget552::~WithdrawalWidget552()
{
    delete ui;
}
