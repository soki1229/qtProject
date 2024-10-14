#ifndef APPBASICCALCULATOR_H
#define APPBASICCALCULATOR_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class AppBasicCalculator;
}
QT_END_NAMESPACE


class Element
{
public:
    enum class Type
    {
        Numeric,
        Bracket,
        Operator
    };

    Element();
    Element(Type type);
    Element(float f);
    Element(char c);
    ~Element(){};

    Type    type_;
    QString str_;
    float   num_;
    bool    isNegative_;
};

using FormulaElements = std::vector<std::unique_ptr<Element>>;

class AppBasicCalculator : public QMainWindow
{
    Q_OBJECT
public:
    AppBasicCalculator(QWidget *parent = nullptr);
    ~AppBasicCalculator();

    void initSignalSlots();

    void onNumericInput(QChar numChar);
    void onOperatorPressed(QChar opChar);
    void setArchive();
    void initialize(bool continuous = false);
    void print();

    bool wasLastType(Element::Type type);
    bool isOperatorAvailable();
    bool calcualteFromula(FormulaElements::iterator begin, FormulaElements::iterator end);

signals:

private slots:
    void onN0Pressed()              {onNumericInput('0');}
    void onN1Pressed()              {onNumericInput('1');}
    void onN2Pressed()              {onNumericInput('2');}
    void onN3Pressed()              {onNumericInput('3');}
    void onN4Pressed()              {onNumericInput('4');}
    void onN5Pressed()              {onNumericInput('5');}
    void onN6Pressed()              {onNumericInput('6');}
    void onN7Pressed()              {onNumericInput('7');}
    void onN8Pressed()              {onNumericInput('8');}
    void onN9Pressed()              {onNumericInput('9');}

    void onErasePressed();
    void onEraseAllPressed();
    void onPNStatusPressed();
    void onBracketPressed();

    void onAdditionPressed()        {onOperatorPressed('+');}
    void onSubstractionPressed()    {onOperatorPressed('-');}
    void onMultipulationPressed()   {onOperatorPressed('*');}
    void onDivisionPressed()        {onOperatorPressed('/');}

    void onReturnsPressed();

private:
    Ui::AppBasicCalculator *ui;

    FormulaElements formula;
    QString history;

    bool isBracketOpened = false;
    int openedIndex = 0;

};

#endif // APPBASICCALCULATOR_H
