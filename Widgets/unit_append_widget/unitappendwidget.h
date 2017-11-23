#ifndef UNITAPPENDWIDGET_H
#define UNITAPPENDWIDGET_H

#include <QMainWindow>

namespace Ui {
class UnitAppendWidget;
}

class UnitAppendWidget : public QMainWindow
{
    Q_OBJECT

public:
    explicit UnitAppendWidget(QWidget *parent = 0);
    ~UnitAppendWidget();

private:
    Ui::UnitAppendWidget *ui;
};

#endif // UNITAPPENDWIDGET_H
