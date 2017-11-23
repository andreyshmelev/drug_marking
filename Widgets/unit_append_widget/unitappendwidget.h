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

private slots:
    void getparsedStringemit (QString gtinstring, QString  SNstring,QString  tnvedstring, QString expstring, QString batchstring, QString sGTINString); // испускаем сигнал что закончили парсинг строки c параметрами естественно чтобы передать в другие виджеты
};

#endif // UNITAPPENDWIDGET_H
