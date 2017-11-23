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

    const QString Start311ProcessQRString = "501301";
    const QString Stop311ProcessQRString  = "641195";

private:
    Ui::UnitAppendWidget *ui;
};

#endif // UNITAPPENDWIDGET_H
