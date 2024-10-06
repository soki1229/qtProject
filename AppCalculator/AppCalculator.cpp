#include "AppCalculator.h"
#include "ui_AppCalculator.h"
#include <iostream>

AppCalculator::AppCalculator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AppCalculator)
{
    ui->setupUi(this);

    initSignalSlots();

    initialize();

    print();
}


AppCalculator::~AppCalculator()
{
    delete ui;
}

void AppCalculator::initSignalSlots()
{
    connect(ui->n0, &QPushButton::clicked, this, &AppCalculator::onN0Pressed);
    connect(ui->n1, &QPushButton::clicked, this, &AppCalculator::onN1Pressed);
    connect(ui->n2, &QPushButton::clicked, this, &AppCalculator::onN2Pressed);
    connect(ui->n3, &QPushButton::clicked, this, &AppCalculator::onN3Pressed);
    connect(ui->n4, &QPushButton::clicked, this, &AppCalculator::onN4Pressed);
    connect(ui->n5, &QPushButton::clicked, this, &AppCalculator::onN5Pressed);
    connect(ui->n6, &QPushButton::clicked, this, &AppCalculator::onN6Pressed);
    connect(ui->n7, &QPushButton::clicked, this, &AppCalculator::onN7Pressed);
    connect(ui->n8, &QPushButton::clicked, this, &AppCalculator::onN8Pressed);
    connect(ui->n9, &QPushButton::clicked, this, &AppCalculator::onN9Pressed);

    connect(ui->erase, &QPushButton::clicked, this, &AppCalculator::onErasePressed);
    connect(ui->eraseAll, &QPushButton::clicked, this, &AppCalculator::onEraseAllPressed);
    connect(ui->pnStatus, &QPushButton::clicked, this, &AppCalculator::onPNStatusPressed);
    connect(ui->bracket, &QPushButton::clicked, this, &AppCalculator::onBracketPressed);

    connect(ui->sAddition, &QPushButton::clicked, this, &AppCalculator::onAdditionPressed);
    connect(ui->sSubstraction, &QPushButton::clicked, this, &AppCalculator::onSubstractionPressed);
    connect(ui->sMultipulation, &QPushButton::clicked, this, &AppCalculator::onMultipulationPressed);
    connect(ui->sDivision, &QPushButton::clicked, this, &AppCalculator::onDivisionPressed);

    connect(ui->returns, &QPushButton::clicked, this, &AppCalculator::onReturnsPressed);
}

void AppCalculator::initialize(bool continuous)
{
    if (!continuous)
    {
        for (auto &iter : printable)
        {
            iter.release();
        }

        printable.clear();

        printable.push_back(std::make_unique<Element>());
    }

    isBracketOpened = false;
    openedIndex = 0;
    formula.clear();
}

void AppCalculator::print()
{
    QString output;

    if (!formula.isEmpty())
    {
        output.push_back(formula + '\n');
    }

    for (auto &iter : printable)
    {
        if (iter->isNegative_)
        {
            output.push_back("-");
        }

        output.push_back(iter->str_);
    }

    ui->textBrowser->setText(output);
}

bool AppCalculator::isOperatorAvailable()
{
    if (printable.empty())
    {
        return false;
    }

    if (printable.back()->type_ == Type::Numeric && printable.back()->str_.isEmpty())
    {
        return false;
    }

    if (printable.back()->type_ == Type::Bracket && isBracketOpened)
    {
        return false;
    }

    return true;
}

void AppCalculator::onNumberInput(QString numString)
{
    if (!formula.isEmpty())
    {
        initialize();
    }

    if (printable.empty() || printable.back()->type_ != Type::Numeric)
    {
        printable.push_back(std::make_unique<Element>());
    }

    if (printable.back()->str_ == "0")
    {
        printable.back()->str_.clear();
    }

    printable.back()->str_.append(numString);

    print();
}

void AppCalculator::onOperatorPressed(QString opString)
{
    if (!isOperatorAvailable())
    {
        return;
    }

    if (!formula.isEmpty())
    {
        initialize(true);
    }

    if (printable.back()->type_ != Type::Operator)
    {
        printable.push_back(std::make_unique<Element>(Type::Operator));
    }

    printable.back()->str_ = opString;

    print();
}

void AppCalculator::onErasePressed()
{
    if (printable.empty() || printable.back()->type_ != Type::Numeric)
    {
        return;
    }

    printable.back()->str_.removeLast();

    if (printable.back()->str_.isEmpty())
    {
        printable.back().release();
        printable.pop_back();
    }

    print();
}

void AppCalculator::onEraseAllPressed()
{
    initialize();

    print();
}

void AppCalculator::onPNStatusPressed()
{
    if (printable.empty() || printable.back()->type_ == Type::Operator)
    {
        printable.push_back(std::make_unique<Element>());
    }

    auto* target = &printable.back();

    if (printable.back()->type_ == Type::Bracket && !isBracketOpened)
    {
        target = &(printable[openedIndex]);
    }

    (*target)->isNegative_ = !(*target)->isNegative_;

    print();
}

void AppCalculator::onBracketPressed()
{
    if (!printable.empty() && printable.back()->type_ == Type::Bracket && isBracketOpened)
    {
        printable.pop_back();
    }
    else
    {
        printable.push_back(std::make_unique<Element>(Type::Bracket));

        printable.back()->str_ = !isBracketOpened? "(" : ")";
    }

    isBracketOpened = !isBracketOpened;

    openedIndex = isBracketOpened? printable.size() - 1 : openedIndex;

    print();
}

void AppCalculator::setFormula()
{
    QString output;
    for (auto &iter : printable)
    {
        if (iter->type_ == Type::Numeric && iter->isNegative_)
            output.push_back("-");

        output.push_back(iter->str_);
    }

    output.push_back(" =");

    formula = output;
}

void AppCalculator::onReturnsPressed()
{
    setFormula();

    if (!calculateFormula())
    {
        ui->textBrowser->setText("ERR");
        initialize();
        return;
    }

    print();
}

bool AppCalculator::calculateFormula()
{
    for (auto it = printable.begin(); it != printable.end(); it++)
    {
        auto& curr = *it;
        if (curr->type_ == Type::Operator)
        {
            auto& prev = *(it-1);
            auto& next = *(it+1);

            if (curr->str_ == " * ")
            {
                prev->str_ = QString::number(prev->str_.toFloat() * next->str_.toFloat());

                printable.erase(it, it+2);
                it--;
            }
            else if (curr->str_ == " / ")
            {
                if (next->str_ == "0")
                {
                    return false;
                }

                prev->str_ = QString::number(prev->str_.toFloat() / next->str_.toFloat());

                printable.erase(it, it+2);
                it--;
            }
        }
    }

    return true;
}

/*
 *     12 / 9 * 3 / 2 should be converted into (12 * 3) / (9 * 2)
*/
