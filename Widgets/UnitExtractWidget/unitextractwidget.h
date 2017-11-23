#ifndef UNITEXTRACTWIDGET_H
#define UNITEXTRACTWIDGET_H

#include <QWidget>

namespace Ui {
class UnitExtractWidget;
}

class UnitExtractWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UnitExtractWidget(QWidget *parent = 0);
    ~UnitExtractWidget();

private:
    Ui::UnitExtractWidget *ui;

public slots:

    GetParcedString (QString  gtinstring, QString SNstring, QString tnvedstring, QString expstring, QString batchstring, QString sGTINString);
};

#endif // UNITEXTRACTWIDGET_H
