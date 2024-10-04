#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initSignalSlots();

    initialize();

    print();
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initSignalSlots()
{
    connect(ui->n0, &QPushButton::clicked, this, &MainWindow::onN0Pressed);
    connect(ui->n1, &QPushButton::clicked, this, &MainWindow::onN1Pressed);
    connect(ui->n2, &QPushButton::clicked, this, &MainWindow::onN2Pressed);
    connect(ui->n3, &QPushButton::clicked, this, &MainWindow::onN3Pressed);
    connect(ui->n4, &QPushButton::clicked, this, &MainWindow::onN4Pressed);
    connect(ui->n5, &QPushButton::clicked, this, &MainWindow::onN5Pressed);
    connect(ui->n6, &QPushButton::clicked, this, &MainWindow::onN6Pressed);
    connect(ui->n7, &QPushButton::clicked, this, &MainWindow::onN7Pressed);
    connect(ui->n8, &QPushButton::clicked, this, &MainWindow::onN8Pressed);
    connect(ui->n9, &QPushButton::clicked, this, &MainWindow::onN9Pressed);

    connect(ui->erase, &QPushButton::clicked, this, &MainWindow::onErasePressed);
    connect(ui->eraseAll, &QPushButton::clicked, this, &MainWindow::onEraseAllPressed);
    connect(ui->pnStatus, &QPushButton::clicked, this, &MainWindow::onPNStatusPressed);
}

void MainWindow::initialize()
{
    for (auto &iter : printable)
    {
        iter.release();
    }
    printable.clear();

    printable.push_back(std::make_unique<Element>());
}

void MainWindow::print()
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

void MainWindow::onNumberInput(QString num)
{
    if (printable.back()->str_ == "0")
    {
        if (num == "0")
            return;

        printable.back()->str_.removeLast();
    }

    printable.back()->str_.append(num);

    print();
}

void MainWindow::onErasePressed()
{
    while (printable.back()->type_ == Type::Operator)
    {
        printable.back().release();
        printable.pop_back();
    }

    printable.back()->str_.removeLast();

    if (printable.back()->str_.isEmpty())
    {
        printable.back()->str_ = "0";
        printable.back()->isNegative_ = false;
    }

    print();
}

void MainWindow::onEraseAllPressed()
{
    initialize();

    print();
}

void MainWindow::onPNStatusPressed()
{
    if (printable.back()->type_ == Type::Operator)
    {
        printable.push_back(std::make_unique<Element>());
        printable.back()->isNegative_ = true;
    }
    else/* if (printable.back()->str_ != "0")*/
    {
        printable.back()->isNegative_ = !printable.back()->isNegative_;
    }

    print();
}
