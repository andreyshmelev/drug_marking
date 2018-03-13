#ifndef XMLVIEWERWIDGET_H
#define XMLVIEWERWIDGET_H

#include <QWidget>
#include <QString>

namespace Ui {
class XMLViewerWidget;
}

class XMLViewerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit XMLViewerWidget(QWidget *parent = 0);
    ~XMLViewerWidget();

public slots:
    void Settext(QString text);
    void SetFilePath(QString text);

private slots:

    void on_CloseButton_clicked();


private:
    Ui::XMLViewerWidget *ui;
};

#endif // XMLVIEWERWIDGET_H
