#ifndef MULTIPACKWIDGET915_H
#define MULTIPACKWIDGET915_H

#include <QWidget>

namespace Ui {
class MultiPackWidget915;
}

class MultiPackWidget915 : public QWidget
{
    Q_OBJECT

public:
    explicit MultiPackWidget915(QWidget *parent = 0);
    ~MultiPackWidget915();

private:
    Ui::MultiPackWidget915 *ui;
};

#endif // MULTIPACKWIDGET915_H
