#include "xmlviewerwidget.h"
#include "ui_xmlviewerwidget.h"
#include <QDebug>

XMLViewerWidget::XMLViewerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::XMLViewerWidget)
{
    ui->setupUi(this);
}


void XMLViewerWidget::Settext(QString text)
{
    ui->textBrowser->setText(text);
}


XMLViewerWidget::~XMLViewerWidget()
{
    delete ui;
    this->deleteLater();
}

void XMLViewerWidget::on_CloseButton_clicked()
{
    this->deleteLater();
}

