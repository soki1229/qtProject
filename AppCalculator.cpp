#include "AppCalculator.h"
#include "ui_AppCalculator.h"

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
}

void AppCalculator::initialize()
{
    for (auto &iter : printable)
    {
        iter.release();
    }
    printable.clear();
    isBracketOpened = false;

    printable.push_back(std::make_unique<Element>());
}

void AppCalculator::print()
{
    QString output;
    for (auto &iter : printable)
    {
        if (iter->type_ == Type::Numeric && iter->isNegative_)
            output.push_back("-");

        output.push_back(iter->str_);
    }

    ui->textBrowser->setText(output);
}

void AppCalculator::onNumberInput(QString numString)
{
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

    printable.back()->isNegative_ = !printable.back()->isNegative_;

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
        printable.push_back(std::make_unique<Element>());
        printable.back()->type_ = Type::Bracket;

        printable.back()->str_ = !isBracketOpened? "(" : ")";
    }


    isBracketOpened = !isBracketOpened;
    print();
}

void AppCalculator::onOperatorPressed(QString opString)
{
    if (!printable.empty() && printable.back()->type_ == Type::Bracket && isBracketOpened)
    {
        return;
    }

    if (printable.back()->type_ != Type::Operator)
    {
        if (printable.back()->str_.isEmpty())
        {
            return;
        }

        printable.push_back(std::make_unique<Element>());

        printable.back()->type_ = Type::Operator;
    }

    printable.back()->str_ = opString;

    print();
}
