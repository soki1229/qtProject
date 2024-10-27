// #include "app/AppBasicCalculator.h"
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow mainWindow;
    // AppBasicCalculator w;
    mainWindow.show();
    return a.exec();
}
