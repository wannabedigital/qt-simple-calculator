#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cmath>

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
    ui->displayExpressionLine->clear();
    pendingOperator = "";
    isWaitingOperand = true;
    needClear = false;
    isEnteringRootDegree = false;
    isEnteringRate = false;
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
    QString prettyOperator = clickedOperator;
    prettyOperator.replace("/","÷").replace("*","×");


    if (!pendingOperator.isEmpty()) {
        currentResult = calculate(currentResult, pendingOperator, operand);
        QString resultStr;
        resultStr.setNum(currentResult);
        ui->calculatorLine->setText(resultStr);
        ui->displayExpressionLine->setText(resultStr + " " + prettyOperator);

    } else {
        currentResult = operand;
        QString resultStr;
        resultStr.setNum(currentResult);
        ui->displayExpressionLine->setText(resultStr + " " + prettyOperator);
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

        QString currentExpressionLine = ui->displayExpressionLine->text();
        ui->displayExpressionLine->setText(
            currentExpressionLine + " " + currentCalculatorLineText + " ="
        );

        pendingOperator = "";
        isWaitingOperand = true;
        needClear = true;
    }
}


void MainWindow::on_actionButton_negate_clicked()
{
    QString currentCalculatorLineText = ui->calculatorLine->text();
    if (currentCalculatorLineText.isEmpty()) {
        ui->calculatorLine->setText("Сначала введите число");
        return;
    }
    double negateNum = currentCalculatorLineText.toDouble() * (-1);

    QString strNegateNum;
    strNegateNum.setNum(negateNum);

    ui->calculatorLine->setText(strNegateNum);
}


void MainWindow::on_actionButton_percent_clicked()
{
    QString currentCalculatorLineText = ui->calculatorLine->text();
    if (currentCalculatorLineText.isEmpty() || currentCalculatorLineText == "0") return;

    double percentNum = currentCalculatorLineText.toDouble() * 0.01;

    QString strPercentNum;
    strPercentNum.setNum(percentNum);

    ui->calculatorLine->setText(strPercentNum);
}


void MainWindow::on_actionButton_cos_clicked()
{
    QString currentCalculatorLineText = ui->calculatorLine->text();
    if (!currentCalculatorLineText.isEmpty()) {
        double currentNum = currentCalculatorLineText.toDouble();
        double cosNum = std::cos(currentNum);
        QString strCosNum;
        strCosNum.setNum(cosNum);
        ui->displayExpressionLine->setText("cos(" + currentCalculatorLineText + ") =");
        ui->calculatorLine->setText(strCosNum);
    } else {
        ui->calculatorLine->setText("Сначала введите число в радианах");
    }
}


void MainWindow::on_actionButton_nRoot_clicked()
{
    if (isEnteringRootDegree) {
        QString currentText = ui->calculatorLine->text();
        if (currentText.isEmpty()) return;

        double radicand = currentText.toDouble();
        if (rootDegree <= 0) {
            ui->calculatorLine->setText("Неправильная степень корня");
            isEnteringRootDegree = false;
            return;
        }

        double result = std::pow(radicand, 1.0 / rootDegree);
        QString resultStr;
        resultStr.setNum(result);

        QString degreeStr;
        degreeStr.setNum(rootDegree);
        QString prettyDegree = degreeStr;
        prettyDegree
            .replace("0","⁰")
            .replace("1","¹")
            .replace("2", "²")
            .replace("3", "³")
            .replace("4", "⁴")
            .replace("5", "⁵")
            .replace("6", "⁶")
            .replace("7", "⁷")
            .replace("8", "⁸")
            .replace("9", "⁹");

        ui->displayExpressionLine->setText(prettyDegree + "√" + currentText + " =");
        ui->calculatorLine->setText(resultStr);

        isEnteringRootDegree = false;
        isWaitingOperand = true;
        needClear = true;

    } else {
        QString currentText = ui->calculatorLine->text();
        if (currentText.isEmpty()) {
            ui->calculatorLine->setText("Введите степень корня");
            return;
        }

        rootDegree = currentText.toDouble();
        if (rootDegree <= 0) {
            ui->calculatorLine->setText("Неправильная степень корня");
            return;
        }

        QString degreeStr;
        degreeStr.setNum(rootDegree);

        QString prettyDegree = degreeStr;
        prettyDegree
            .replace("0","⁰")
            .replace("1","¹")
            .replace("2", "²")
            .replace("3", "³")
            .replace("4", "⁴")
            .replace("5", "⁵")
            .replace("6", "⁶")
            .replace("7", "⁷")
            .replace("8", "⁸")
            .replace("9", "⁹");

        ui->displayExpressionLine->setText(prettyDegree + "√" + ". Введите число:");
        ui->calculatorLine->clear();
        isEnteringRootDegree = true;
        isWaitingOperand = false;
    }
}


void MainWindow::on_actionButton_np_clicked()
{
    if (isEnteringRate) {
        QString currentText = ui->calculatorLine->text();
        if (currentText.isEmpty()) return;

        double base = currentText.toDouble();
        if (rate < 0) {
            ui->calculatorLine->setText("Направильный процент от числа");
            isEnteringRate = false;
            return;
        }

        double result = (base / 100) * rate;
        QString resultStr;
        resultStr.setNum(result);

        QString rateStr;
        rateStr.setNum(rate);

        ui->displayExpressionLine->setText(rateStr + "% от " + currentText + " =");
        ui->calculatorLine->setText(resultStr);

        isEnteringRate = false;
        isWaitingOperand = true;
        needClear = true;

    } else {
        QString currentText = ui->calculatorLine->text();
        if (currentText.isEmpty()) {
            ui->calculatorLine->setText("Введите процент от числа");
            return;
        }

        rate = currentText.toDouble();
        if (rate < 0) {
            ui->calculatorLine->setText("Направильный процент от числа");
            return;
        }

        ui->displayExpressionLine->setText(currentText + "%" + ". Введите число:");
        ui->calculatorLine->clear();
        isEnteringRate = true;
        isWaitingOperand = false;
    }
}

