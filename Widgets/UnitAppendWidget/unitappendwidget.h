#ifndef UNITAPPENDWIDGET_H
#define UNITAPPENDWIDGET_H

#include <QWidget>
#include <QObject>
#include <C:\Work\Application\Application\basebuisnessclass.h>

namespace Ui {
class UnitAppendWidget;
}

class UnitAppendWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UnitAppendWidget(QWidget *parent = 0);
    ~UnitAppendWidget();
    BaseBuisnessClass  * BaseClass;

private:
    Ui::UnitAppendWidget *ui;

};

#endif // UNITAPPENDWIDGET_H
