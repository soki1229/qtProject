#include "AppCalculator.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AppCalculator w;
    w.show();
    return a.exec();
}
