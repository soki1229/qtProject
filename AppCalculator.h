#ifndef APPCALCULATOR_H
#define APPCALCULATOR_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class AppCalculator;
}
QT_END_NAMESPACE

enum Type
{
    Numeric,
    Bracket,
    Operator
};

class Element
{
public:
    Type    type_;
    QString str_;
    bool    isNegative_;

};

class AppCalculator : public QMainWindow
{
    Q_OBJECT

public:
    AppCalculator(QWidget *parent = nullptr);
    ~AppCalculator();

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

    void onAdditionPressed()        {onOperatorPressed("+");}
    void onSubstractionPressed()    {onOperatorPressed("-");}
    void onMultipulationPressed()   {onOperatorPressed("*");}
    void onDivisionPressed()        {onOperatorPressed("/");}

private:
    Ui::AppCalculator *ui;

    std::vector<std::unique_ptr<Element>> printable;
    // std::vector<std::pair<Element, QString>> printable = {{Element::Numeric, "0"}};
    bool isBracketOpened = false;
};
#endif // APPCALCULATOR_H
