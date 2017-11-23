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
    void updateWidgetGui(QString gtinstring, QString batchstring, QString SNstring, QString tnvedstring, QString expstring);

    bool registration ;

public slots:

    GetParsedString (QString  gtinstring, QString SNstring, QString tnvedstring, QString expstring, QString batchstring, QString sGTINString);
    void ToggleRegistration();

private slots:

    void on_RegistrationStartButton_clicked();
    void updateGUI();

signals:

    StartRegisterControlSamples();
    StopRegisterControlSamples();
    RegistrationToggled();
};

#endif // UNITEXTRACTWIDGET_H
