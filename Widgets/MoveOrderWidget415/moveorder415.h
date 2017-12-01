#ifndef MOVEORDER415_H
#define MOVEORDER415_H

#include <QWidget>

namespace Ui {
class MoveOrder415;
}

class MoveOrder415 : public QWidget
{
    Q_OBJECT

public:
    explicit MoveOrder415(QWidget *parent = 0);
    ~MoveOrder415();

private:
    Ui::MoveOrder415 *ui;
};

#endif // MOVEORDER415_H
