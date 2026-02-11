#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->numButton_0, &QPushButton::clicked, this, &MainWindow::onDigitButtonClicked);
    connect(ui->numButton_1, &QPushButton::clicked, this, &MainWindow::onDigitButtonClicked);
    connect(ui->numButton_2, &QPushButton::clicked, this, &MainWindow::onDigitButtonClicked);
    connect(ui->numButton_3, &QPushButton::clicked, this, &MainWindow::onDigitButtonClicked);
    connect(ui->numButton_4, &QPushButton::clicked, this, &MainWindow::onDigitButtonClicked);
    connect(ui->numButton_5, &QPushButton::clicked, this, &MainWindow::onDigitButtonClicked);
    connect(ui->numButton_6, &QPushButton::clicked, this, &MainWindow::onDigitButtonClicked);
    connect(ui->numButton_7, &QPushButton::clicked, this, &MainWindow::onDigitButtonClicked);
    connect(ui->numButton_8, &QPushButton::clicked, this, &MainWindow::onDigitButtonClicked);
    connect(ui->numButton_9, &QPushButton::clicked, this, &MainWindow::onDigitButtonClicked);

    connect(ui->actionButton_div, &QPushButton::clicked, this, &MainWindow::mathOperation);
    connect(ui->actionButton_mult, &QPushButton::clicked, this, &MainWindow::mathOperation);
    connect(ui->actionButton_addition, &QPushButton::clicked, this, &MainWindow::mathOperation);
    connect(ui->actionButton_substraction, &QPushButton::clicked, this, &MainWindow::mathOperation);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionButton_clear_clicked()
{
    QString currentCalculatorLineText = ui->calculatorLine->text();

    if (!currentCalculatorLineText.isEmpty()) {
        int textLength = currentCalculatorLineText.length();

        ui->calculatorLine->setText(currentCalculatorLineText.left(textLength - 1));
    }
}

void MainWindow::on_actionButton_allClear_clicked()
{
    ui->calculatorLine->clear();
    pendingOperator = "";
    isWaitingOperand = true;
}

void MainWindow::onDigitButtonClicked()
{
    if (needClear) {
        ui->calculatorLine->clear();
        needClear = false;
    }

    QPushButton *button = (QPushButton *)(sender());
    if (!button) return;

    if (isWaitingOperand) {
        ui->calculatorLine->clear();
        isWaitingOperand = false;
    }

    QString digit = button->property("digit").toString();

    QString currentCalculatorLineText = ui->calculatorLine->text();

    QString strNum = (currentCalculatorLineText + digit);

    ui->calculatorLine->setText(strNum);
}

double MainWindow::calculate(double leftOperand, const QString &pendingOperator, double rightOperand)
{
    if (pendingOperator == "+") {
        return leftOperand + rightOperand;
    } else if (pendingOperator == "-") {
        return leftOperand - rightOperand;
    } else if (pendingOperator == "*") {
        return leftOperand * rightOperand;
    } else if (pendingOperator == "/") {
        if (rightOperand != 0) {
            return leftOperand / rightOperand;
        } else {
            ui->calculatorLine->setText("Ошибка: не найден оператор");
            isWaitingOperand = true;
            return 0;
        }
    }
    return rightOperand;
}

void MainWindow::mathOperation()
{
    if (needClear) {
        needClear = false;
    }

    QPushButton *button = (QPushButton *)(sender());
    if (!button) return;

    QString currentCalculatorLineText = ui->calculatorLine->text();
    double operand = currentCalculatorLineText.toDouble();
    QString clickedOperator = button->property("operation").toString();

    if (!pendingOperator.isEmpty()) {
        currentResult = calculate(currentResult, pendingOperator, operand);
        QString resultStr;
        resultStr.setNum(currentResult);
        ui->calculatorLine->setText(resultStr);
    } else {
        currentResult = operand;
    }

    pendingOperator = clickedOperator;
    isWaitingOperand = true;
}

void MainWindow::on_actionButton_comma_clicked()
{
    if (needClear) {
        ui->calculatorLine->clear();
        needClear = false;
    }

    if (isWaitingOperand) {
        ui->calculatorLine->setText("0");
        isWaitingOperand = false;
    }

    QString currentCalculatorLineText = ui->calculatorLine->text();

    if (currentCalculatorLineText.contains('.')) return;

    ui->calculatorLine->setText(currentCalculatorLineText + ".");
}

void MainWindow::on_actionButton_equal_clicked()
{
    if (needClear) {
        ui->calculatorLine->clear();
        needClear = false;
        return;
    }

    QString currentCalculatorLineText = ui->calculatorLine->text();
    double operand = currentCalculatorLineText.toDouble();

    if (!pendingOperator.isEmpty()) {
        currentResult = calculate(currentResult, pendingOperator, operand);

        QString resultStr;
        resultStr.setNum(currentResult);
        ui->calculatorLine->setText(resultStr);

        pendingOperator = "";
        isWaitingOperand = true;
        needClear = true;
    }
}

