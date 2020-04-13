#include "viewcontroller.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    viewController w;
    w.show();
    return a.exec();
}
