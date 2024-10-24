#include "AppBasicCalculator.h"
#include "ui_AppBasicCalculator.h"

Element::Element()
    : type_(Type::Numeric), str_(QString("0")), num_(0), isNegative_(false)
{
}

Element::Element(Type type) : Element()
{
    type_ = type;
}

Element::Element(char c) : Element()
{
    switch (c)
    {
    case '+':
    case '-':
    case '*':
    case '/':
    {
        type_ = Type::Operator;
        str_  = " " + QString(c) + " ";
        break;
    }
    default:
        break;
    }
}

Element::Element(double f) : type_(Type::Numeric), num_(f)
{
    isNegative_ = (f < 0);
    str_ = QString::number(f * (isNegative_? -1 : 1));
}

AppBasicCalculator::AppBasicCalculator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AppBasicCalculator)
{
    ui->setupUi(this);

    initSignalSlots();

    initialize();

    print();
}

AppBasicCalculator::~AppBasicCalculator()
{
    delete ui;
}

void AppBasicCalculator::initSignalSlots()
{
    connect(ui->num0, &QPushButton::clicked, this, &AppBasicCalculator::onN0Pressed);
    connect(ui->num1, &QPushButton::clicked, this, &AppBasicCalculator::onN1Pressed);
    connect(ui->num2, &QPushButton::clicked, this, &AppBasicCalculator::onN2Pressed);
    connect(ui->num3, &QPushButton::clicked, this, &AppBasicCalculator::onN3Pressed);
    connect(ui->num4, &QPushButton::clicked, this, &AppBasicCalculator::onN4Pressed);
    connect(ui->num5, &QPushButton::clicked, this, &AppBasicCalculator::onN5Pressed);
    connect(ui->num6, &QPushButton::clicked, this, &AppBasicCalculator::onN6Pressed);
    connect(ui->num7, &QPushButton::clicked, this, &AppBasicCalculator::onN7Pressed);
    connect(ui->num8, &QPushButton::clicked, this, &AppBasicCalculator::onN8Pressed);
    connect(ui->num9, &QPushButton::clicked, this, &AppBasicCalculator::onN9Pressed);
    connect(ui->numDecimalPoint, &QPushButton::clicked, this, &AppBasicCalculator::onDecimalPointPressed);
    connect(ui->numPrinciple, &QPushButton::clicked, this, &AppBasicCalculator::onPNStatusPressed);

    connect(ui->exDelete, &QPushButton::clicked, this, &AppBasicCalculator::onErasePressed);
    connect(ui->exAllClear, &QPushButton::clicked, this, &AppBasicCalculator::onEraseAllPressed);
    connect(ui->exBracket, &QPushButton::clicked, this, &AppBasicCalculator::onBracketPressed);
    connect(ui->exPercentage, &QPushButton::clicked, this, &AppBasicCalculator::onPercentagePressed);

    connect(ui->opAddition, &QPushButton::clicked, this, &AppBasicCalculator::onAdditionPressed);
    connect(ui->opSubstraction, &QPushButton::clicked, this, &AppBasicCalculator::onSubstractionPressed);
    connect(ui->opMultipulation, &QPushButton::clicked, this, &AppBasicCalculator::onMultipulationPressed);
    connect(ui->opDivision, &QPushButton::clicked, this, &AppBasicCalculator::onDivisionPressed);
    connect(ui->opReturn, &QPushButton::clicked, this, &AppBasicCalculator::onReturnsPressed);
}

void AppBasicCalculator::initialize(bool continuous)
{
    if (!continuous)
    {
        for (auto &iter : formula)
        {
            iter.release();
        }

        formula.clear();
    }

    isBracketOpened = false;
    openedIndex = 0;
    resetRequired = false;
    history.clear();
}

void AppBasicCalculator::print()
{
    QString output;

    if (!history.isEmpty())
    {
        output.append(history + '\n');
    }

    for (auto &iter : formula)
    {
        if (iter->isNegative_)
        {
            output.append('-');
        }

        output.append(iter->type_ == Element::Type::Operator? ' ' + iter->str_ + ' ' : iter->str_);
    }

    ui->display->setText(output);
}

bool AppBasicCalculator::wasLastType(Element::Type type)
{
    return (!formula.empty() && formula.back()->type_ == type);
}

bool AppBasicCalculator::isOperatorAvailable()
{
    if (formula.empty())
    {
        return false;
    }

    if (wasLastType(Element::Type::Bracket) && isBracketOpened)
    {
        return false;
    }

    return true;
}

void AppBasicCalculator::onNumericInput(QChar numChar)
{
    if (wasLastType(Element::Type::Bracket) && !isBracketOpened)
    {
        return;
    }

    if (resetRequired && formula.size() == 1)
    {
        initialize();
    }

    if (!wasLastType(Element::Type::Numeric))
    {
        formula.push_back(std::make_unique<Element>());
    }

    if (formula.back()->str_ == '0')
    {
        formula.back()->str_.clear();
    }

    formula.back()->str_.append(numChar);

    formula.back()->num_ = (formula.back()->str_).toDouble() * (formula.back()->isNegative_? -1 : 1);

    print();
}

void AppBasicCalculator::onDecimalPointPressed()
{
    if (wasLastType(Element::Type::Bracket))
    {
        return;
    }

    if (wasLastType(Element::Type::Numeric))
    {
        if (formula.back()->isRealNumber())
        {
            return;
        }
    }
    else
    {
        formula.push_back(std::make_unique<Element>());
    }

    if (resetRequired)
    {
        initialize(true);
    }

    formula.back()->str_.append(".");

    formula.back()->num_ = (formula.back()->str_).toDouble() * (formula.back()->isNegative_? -1 : 1);

    print();
}

void AppBasicCalculator::onOperatorPressed(QChar opChar)
{
    if (!isOperatorAvailable())
    {
        return;
    }

    if (resetRequired)
    {
        initialize(true);
    }

    if (wasLastType(Element::Type::Numeric) && formula.back()->str_.endsWith('.'))
    {
        formula.back()->str_.push_back('0');
    }

    if (!wasLastType(Element::Type::Operator))
    {
        formula.push_back(std::make_unique<Element>(Element::Type::Operator));
    }

    formula.back()->str_ = opChar;

    print();
}

void AppBasicCalculator::onErasePressed()
{
    if (!wasLastType(Element::Type::Numeric))
    {
        return;
    }

    formula.back()->str_.removeLast();

    formula.back()->num_ = (formula.back()->str_).toDouble() * (formula.back()->isNegative_? -1 : 1);

    if (formula.back()->str_.isEmpty())
    {
        formula.back().release();
        formula.pop_back();
    }

    print();
}

void AppBasicCalculator::onEraseAllPressed()
{
    initialize();

    print();
}

void AppBasicCalculator::onPNStatusPressed()
{
    if (resetRequired && formula.size() == 1)
    {
        initialize(true);
    }

    if (formula.empty() || formula.back()->type_ == Element::Type::Operator)
    {
        formula.push_back(std::make_unique<Element>());
    }

    auto* target = &formula.back();

    if (wasLastType(Element::Type::Bracket) && !isBracketOpened)
    {
        target = &(formula[openedIndex]);
    }

    (*target)->isNegative_ = !(*target)->isNegative_;

    if ((*target)->type_ == Element::Type::Numeric)
    {
        (*target)->num_ = ((*target)->str_).toDouble() * ((*target)->isNegative_? -1 : 1);
    }

    print();
}

void AppBasicCalculator::onBracketPressed()
{
    if (!isBracketOpened)
    {
        if (!formula.empty() && formula.back()->type_ != Element::Type::Operator)
        {
            return;
        }
    }
    else
    {
        if (wasLastType(Element::Type::Operator))
        {
            return;
        }

        if (wasLastType(Element::Type::Bracket))
        {
            formula.pop_back();
            isBracketOpened = false;
            print();

            return;
        }
    }

    formula.push_back(std::make_unique<Element>(Element::Type::Bracket));

    formula.back()->str_ = !isBracketOpened? '(' : ')';

    isBracketOpened = !isBracketOpened;

    openedIndex = isBracketOpened? formula.size() - 1 : openedIndex;

    print();
}

void AppBasicCalculator::onPercentagePressed()
{
    if (!wasLastType(Element::Type::Numeric))
    {
        return;
    }

    if (formula.back()->str_.endsWith('%'))
    {
        return;
    }

    formula.back()->str_.append('%');
    formula.back()->num_ /= 100;

    print();
}

void AppBasicCalculator::setArchive()
{
    history.clear();

    for (auto &iter : formula)
    {
        if (iter->isNegative_)
        {
            history.append('-');
        }

        history.append(iter->type_ == Element::Type::Operator? ' ' + iter->str_ + ' ' : iter->str_);

    }

    history.append(" = ");
}

void AppBasicCalculator::onReturnsPressed()
{
    if (formula.empty() || isBracketOpened || wasLastType(Element::Type::Operator))
    {
        return;
    }

    setArchive();

    if (!calculateFromula(formula.begin(), formula.end()))
    {
        ui->display->setText("ERROR");
        initialize();

        return;
    }

    resetRequired = true;

    print();
}

bool AppBasicCalculator::calculateFromula(FormulaElements::iterator sPos, FormulaElements::iterator ePos)
{
    // 1. Bracket
    auto it = std::find_if(sPos, ePos, [](auto& p){return p->type_ == Element::Type::Bracket;});
    if (it != ePos)
    {
        auto it2 = std::find_if(it+1, ePos, [](auto& p){return p->type_ == Element::Type::Bracket;});

        if (std::distance(it, it2) == 2)
        {
            if (it->get()->isNegative_)
            {
                (it+1)->get()->isNegative_ = true;
                (it+1)->get()->num_ *= -1;
            }

            it2 = formula.erase(it2);
            it = formula.erase(it);

            return calculateFromula(formula.begin(), formula.end());
        }

        return calculateFromula(it+1, it2);
    }

    it = std::find_if(sPos, ePos, [](auto& p){return p->type_ == Element::Type::Operator && (p->str_ == '*' || p->str_ == '/');});
    if (it != ePos)
    {
        auto opA = (it-1)->get()->num_;
        auto opB = (it+1)->get()->num_;

        if (it->get()->str_ == '*')
        {
            // 2. Multipulation
            it = formula.insert(it-1, std::make_unique<Element>(opA * opB));
        }
        else
        {
            if (opB == 0)
                return false;

            // 2. Division
            it = formula.insert(it-1, std::make_unique<Element>(opA / opB));
        }

        it+1 = formula.erase(it+1, it+4);

        return calculateFromula(formula.begin(), formula.end());
    }

    it = std::find_if(sPos, ePos, [](auto& p){return p->type_ == Element::Type::Operator;});
    if (it != ePos)
    {
        auto opA = (it-1)->get()->num_;
        auto opB = (it+1)->get()->num_;

        if (it->get()->str_ == '+')
        {
            // 3. Addition
            it = formula.insert(it-1, std::make_unique<Element>(opA + opB));
        }
        else
        {
            // 3. Substraction
            it = formula.insert(it-1, std::make_unique<Element>(opA - opB));
        }

        it+1 = formula.erase(it+1, it+4);

        return calculateFromula(formula.begin(), formula.end());
    }

    return (formula.size() == 1);
}
