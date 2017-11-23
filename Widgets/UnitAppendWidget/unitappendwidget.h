#ifndef UNITAPPENDWIDGET_H
#define UNITAPPENDWIDGET_H

#include <QWidget>

namespace Ui {
class UnitAppendWidget;
}

class UnitAppendWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UnitAppendWidget(QWidget *parent = 0);
    ~UnitAppendWidget();

private:
    Ui::UnitAppendWidget *ui;
};

#endif // UNITAPPENDWIDGET_H
