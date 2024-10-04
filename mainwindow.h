#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

enum Type
{
    Numeric,
    Operator
};

class Element
{
public:
    Type    type_;
    QString str_;
    bool    isNegative_;

};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void initSignalSlots();

    void onNumberInput(QString numString);
    void onOperatorPressed(QString opString);
    void initialize();
    void print();

private slots:
    void onN0Pressed()              {onNumberInput("0");}
    void onN1Pressed()              {onNumberInput("1");}
    void onN2Pressed()              {onNumberInput("2");}
    void onN3Pressed()              {onNumberInput("3");}
    void onN4Pressed()              {onNumberInput("4");}
    void onN5Pressed()              {onNumberInput("5");}
    void onN6Pressed()              {onNumberInput("6");}
    void onN7Pressed()              {onNumberInput("7");}
    void onN8Pressed()              {onNumberInput("8");}
    void onN9Pressed()              {onNumberInput("9");}

    void onErasePressed();
    void onEraseAllPressed();
    void onPNStatusPressed();
    void onBracketPressed();

    void onAdditionPressed()        {onOperatorPressed(" + ");}
    void onSubstractionPressed()    {onOperatorPressed(" - ");}
    void onMultipulationPressed()   {onOperatorPressed(" * ");}
    void onDivisionPressed()        {onOperatorPressed(" / ");}

private:
    Ui::MainWindow *ui;

    std::vector<std::unique_ptr<Element>> printable;
    // std::vector<std::pair<Element, QString>> printable = {{Element::Numeric, "0"}};
    bool isNegative = false;
};
#endif // MAINWINDOW_H
