#include "moveplacewidget431.h"
#include "ui_moveplacewidget431.h"

MovePlaceWidget431::MovePlaceWidget431(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MovePlaceWidget431)
{
    ui->setupUi(this);
}

MovePlaceWidget431::~MovePlaceWidget431()
{
    delete ui;
}
