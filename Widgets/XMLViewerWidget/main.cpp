#include "xmlviewerwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    XMLViewerWidget w;
    w.show();

    return a.exec();
}
