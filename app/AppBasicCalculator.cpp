#include "AppBasicCalculator.h"
#include "ui_AppBasicCalculator.h"

Element::Element(char c)
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

Element::Element(float f) : type_(Type::Numeric), num_(f)
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
    connect(ui->numPrinciple, &QPushButton::clicked, this, &AppBasicCalculator::onPNStatusPressed);

    connect(ui->exDelete, &QPushButton::clicked, this, &AppBasicCalculator::onErasePressed);
    connect(ui->exAllClear, &QPushButton::clicked, this, &AppBasicCalculator::onEraseAllPressed);
    connect(ui->exBracket, &QPushButton::clicked, this, &AppBasicCalculator::onBracketPressed);

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

        formula.push_back(std::make_unique<Element>());
    }

    isBracketOpened = false;
    openedIndex = 0;
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

bool AppBasicCalculator::isOperatorAvailable()
{
    if (formula.empty())
    {
        return false;
    }

    if (formula.back()->type_ == Element::Type::Numeric && formula.back()->str_.isEmpty())
    {
        return false;
    }

    if (formula.back()->type_ == Element::Type::Bracket && isBracketOpened)
    {
        return false;
    }

    return true;
}

void AppBasicCalculator::onNumberInput(QChar numChar)
{
    if (!history.isEmpty() && formula.size() == 1)
    {
        initialize();
    }

    if (formula.empty() || formula.back()->type_ != Element::Type::Numeric)
    {
        formula.push_back(std::make_unique<Element>());
    }

    if (formula.back()->str_ == '0')
    {
        formula.back()->str_.clear();
    }

    formula.back()->str_.append(numChar);

    formula.back()->num_ = (formula.back()->str_).toFloat() * (formula.back()->isNegative_? -1 : 1);

    print();
}

void AppBasicCalculator::onOperatorPressed(QChar opChar)
{
    if (!isOperatorAvailable())
    {
        return;
    }

    if (!history.isEmpty())
    {
        initialize(true);
    }

    if (formula.back()->type_ != Element::Type::Operator)
    {
        formula.push_back(std::make_unique<Element>(Element::Type::Operator));
    }

    formula.back()->str_ = opChar;

    print();
}

void AppBasicCalculator::onErasePressed()
{
    if (formula.empty() || formula.back()->type_ != Element::Type::Numeric)
    {
        return;
    }

    formula.back()->str_.removeLast();

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
    if (formula.empty() || formula.back()->type_ == Element::Type::Operator)
    {
        formula.push_back(std::make_unique<Element>());
    }

    auto* target = &formula.back();

    if (formula.back()->type_ == Element::Type::Bracket && !isBracketOpened)
    {
        target = &(formula[openedIndex]);
    }

    (*target)->isNegative_ = !(*target)->isNegative_;

    if ((*target)->type_ == Element::Type::Numeric)
    {
        (*target)->num_ = ((*target)->str_).toFloat() * ((*target)->isNegative_? -1 : 1);
    }

    print();
}

void AppBasicCalculator::onBracketPressed()
{
    if (isBracketOpened)
    {
        if (formula.back()->type_ == Element::Type::Operator)
        {
            return;
        }

        if (formula.back()->type_ == Element::Type::Bracket)
        {
            formula.pop_back();
        }
    }

    if (formula.back()->type_ == Element::Type::Numeric && formula.back()->str_.isEmpty())
    {
        formula.back()->type_ = Element::Type::Bracket;
    }
    else
    {
        formula.push_back(std::make_unique<Element>(Element::Type::Bracket));
    }

    formula.back()->str_ = !isBracketOpened? '(' : ')';

    isBracketOpened = !isBracketOpened;

    openedIndex = isBracketOpened? formula.size() - 1 : openedIndex;

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

    setArchive();

    if (!calcualteFromula(formula.begin(), formula.end()))
    {
        ui->display->setText("ERROR");
        initialize();

        return;
    }

    print();
}

bool AppBasicCalculator::calcualteFromula(FormulaElements::iterator sPos, FormulaElements::iterator ePos)
{
    if (formula.size() == 1)
    {
        return true;
    }

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

            return calcualteFromula(formula.begin(), formula.end());
        }

        return calcualteFromula(it+1, it2);
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

        return calcualteFromula(formula.begin(), formula.end());
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

        return calcualteFromula(formula.begin(), formula.end());
    }

    return false;
}
