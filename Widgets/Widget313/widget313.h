#ifndef WIDGET313_H
#define WIDGET313_H

#include <QWidget>

namespace Ui {
class widget313;
}

class widget313 : public QWidget
{
    Q_OBJECT

public:
    explicit widget313(QWidget *parent = 0);
    ~widget313();

private:
    Ui::widget313 *ui;
};

#endif // WIDGET313_H
