#include "moveorder415.h"
#include "ui_moveorder415.h"

MoveOrder415::MoveOrder415(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MoveOrder415)
{
    ui->setupUi(this);
}

MoveOrder415::~MoveOrder415()
{
    delete ui;
}
