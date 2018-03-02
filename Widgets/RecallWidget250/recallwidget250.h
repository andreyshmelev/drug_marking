#ifndef RECALLWIDGET250_H
#define RECALLWIDGET250_H

#include <QWidget>

namespace Ui {
class RecallWidget250;
}

class RecallWidget250 : public QWidget
{
    Q_OBJECT

public:
    explicit RecallWidget250(QWidget *parent = 0);
    ~RecallWidget250();

private:
    Ui::RecallWidget250 *ui;
};

#endif // RECALLWIDGET250_H
