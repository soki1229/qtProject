#include "app/AppBasicCalculator.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AppBasicCalculator w;
    w.show();
    return a.exec();
}
