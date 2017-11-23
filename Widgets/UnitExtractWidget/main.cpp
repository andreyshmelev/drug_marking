#include "unitextractwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UnitExtractWidget w;
    w.show();

    return a.exec();
}
