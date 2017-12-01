#ifndef UNITPACKWIDGET911_H
#define UNITPACKWIDGET911_H

#include <QWidget>

namespace Ui {
class UnitPackWidget911;
}

class UnitPackWidget911 : public QWidget
{
    Q_OBJECT

public:
    explicit UnitPackWidget911(QWidget *parent = 0);
    ~UnitPackWidget911();

private slots:
    void on_pushButton_clicked();

private:
    Ui::UnitPackWidget911 *ui;
};

#endif // UNITPACKWIDGET911_H
