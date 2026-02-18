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
    isRootDegreeConfirmed = false;
    rootDegree = 0;
    rootMultiplier = 0;
    isEnteringRate = false;
    isPercentRateConfirmed = false;
    rate = 0;
    percentMultiplier = 0;
    isEnteringCos = false;
    cosAngle = 0;
    cosMultiplier = 0;
}

void MainWindow::onDigitButtonClicked()
{
    if (needClear) {
        ui->calculatorLine->clear();
        needClear = false;
    }

    QPushButton *button = (QPushButton *)(sender());
    if (!button) return;

    if (isWaitingOperand && !isEnteringRootDegree && !isEnteringRate && !isEnteringCos) {
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
    if (isEnteringRootDegree || isEnteringRate || isEnteringCos) {
        on_actionButton_allClear_clicked();
    }

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

    if (isEnteringRate) {
        on_actionButton_np_clicked();
        return;
    }

    if (isEnteringRootDegree) {
        on_actionButton_nRoot_clicked();
        return;
    }

    if (isEnteringCos) {
        on_actionButton_cos_clicked();
        return;
    }

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
    } else {
        ui->calculatorLine->setText(currentCalculatorLineText);
        ui->displayExpressionLine->setText(currentCalculatorLineText + " =");
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
    if (isEnteringCos) {
        QString currentCalculatorLineText = ui->calculatorLine->text();
        if (currentCalculatorLineText.isEmpty()) return;

        cosAngle = currentCalculatorLineText.toDouble();
        double cosResult = cosMultiplier * std::cos(cosAngle);

        QString resultStr;
        resultStr.setNum(cosResult);

        QString cosExpression;
        if (cosMultiplier != 1.0) {
            QString strCosMultiplier;
            strCosMultiplier.setNum(cosMultiplier);
            cosExpression = strCosMultiplier + " * cos(" + currentCalculatorLineText + ")";
        } else {
            cosExpression = "cos(" + currentCalculatorLineText + ")";
        }

        if (!pendingOperator.isEmpty()) {
            double leftOperand = currentResult;
            currentResult = calculate(currentResult, pendingOperator, cosResult);

            QString resultStr;
            resultStr.setNum(currentResult);
            ui->calculatorLine->setText(resultStr);

            QString prettyOperator = pendingOperator;
            prettyOperator.replace("/","÷").replace("*","×");

            QString strLeftOperand;
            strLeftOperand.setNum(leftOperand);

            ui->displayExpressionLine->setText(
                strLeftOperand + " " + prettyOperator + " " + cosExpression + " ="
                );

            pendingOperator = "";
        } else {
            ui->calculatorLine->setText(resultStr);
            ui->displayExpressionLine->setText(cosExpression + " =");
        }

        isEnteringCos = false;
        isWaitingOperand = true;
        needClear = true;
        cosMultiplier = 0;
        cosAngle = 0;
    } else {
        QString currentCalculatorLineText = ui->calculatorLine->text();

        if (!pendingOperator.isEmpty()) {
            cosMultiplier = 1.0;
        } else if (!currentCalculatorLineText.isEmpty()) {
            cosMultiplier = currentCalculatorLineText.toDouble();
        } else {
            cosMultiplier = 1.0;
        }

        ui->displayExpressionLine->setText("Введите угол (в радианах)");

        ui->calculatorLine->clear();
        isEnteringCos = true;
        isWaitingOperand = false;
        needClear = false;
    }
}


void MainWindow::on_actionButton_nRoot_clicked()
{
    if (isEnteringRootDegree && isRootDegreeConfirmed) {
        QString currentCalculatorLineText = ui->calculatorLine->text();
        if (currentCalculatorLineText.isEmpty()) return;

        double radicand = currentCalculatorLineText.toDouble();
        if (rootDegree <= 0) {
            ui->calculatorLine->setText("Неправильная степень корня");
            isEnteringRootDegree = false;
            isRootDegreeConfirmed = false;
            return;
        }

        double rootResult = rootMultiplier * std::pow(radicand, 1.0 / rootDegree);

        QString degreeStr;
        degreeStr.setNum(rootDegree);
        QString prettyDegree = degreeStr;

        prettyDegree
            .replace("0", "⁰")
            .replace("1", "¹")
            .replace("2", "²")
            .replace("3", "³")
            .replace("4", "⁴")
            .replace("5", "⁵")
            .replace("6", "⁶")
            .replace("7", "⁷")
            .replace("8", "⁸")
            .replace("9", "⁹");

        QString rootExpression;
        if (rootMultiplier != 1.0) {
            QString strRootMultiplier;
            strRootMultiplier.setNum(rootMultiplier);
            rootExpression = strRootMultiplier + " * " + prettyDegree + "√" + currentCalculatorLineText;
        } else {
            rootExpression = prettyDegree + "√" + currentCalculatorLineText;
        }

        if (!pendingOperator.isEmpty()) {
            double leftOperand = currentResult;
            currentResult = calculate(currentResult, pendingOperator, rootResult);

            QString resultStr;
            resultStr.setNum(currentResult);
            ui->calculatorLine->setText(resultStr);

            QString prettyOperator = pendingOperator;
            prettyOperator.replace("/","÷").replace("*","×");

            QString strLeftOperand;
            strLeftOperand.setNum(leftOperand);

            ui->displayExpressionLine->setText(
                strLeftOperand + " " + prettyOperator + " " + rootExpression + " ="
                );

            pendingOperator = "";
        } else {
            ui->calculatorLine->setText(QString::number(rootResult));
            ui->displayExpressionLine->setText(rootExpression + " =");
        }

        isEnteringRootDegree = false;
        isRootDegreeConfirmed = false;
        isWaitingOperand = true;
        needClear = true;
        rootMultiplier = 0;
        rootDegree = 0;
    } else if (isEnteringRootDegree && !isRootDegreeConfirmed) {
        QString currentCalculatorLineText = ui->calculatorLine->text();
        if (currentCalculatorLineText.isEmpty()) {
            ui->calculatorLine->setText("Введите степень корня");
            return;
        }

        rootDegree = currentCalculatorLineText.toDouble();
        if (rootDegree <= 0) {
            ui->calculatorLine->setText("Неправильная степень корня");
            return;
        }

        QString degreeStr;
        degreeStr.setNum(rootDegree);
        QString prettyDegree = degreeStr;
        prettyDegree
            .replace("0", "⁰")
            .replace("1", "¹")
            .replace("2", "²")
            .replace("3", "³")
            .replace("4", "⁴")
            .replace("5", "⁵")
            .replace("6", "⁶")
            .replace("7", "⁷")
            .replace("8", "⁸")
            .replace("9", "⁹");

        ui->displayExpressionLine->setText(prettyDegree + "√ (введите число под корнем)");

        ui->calculatorLine->clear();
        isRootDegreeConfirmed = true;
        isWaitingOperand = false;
    } else {
        QString currentCalculatorLineText = ui->calculatorLine->text();

        if (!pendingOperator.isEmpty()) {
            rootMultiplier = 1.0;
        }
        else if (!currentCalculatorLineText.isEmpty()) {
            rootMultiplier = currentCalculatorLineText.toDouble();
        }
        else {
            rootMultiplier = 1.0;
        }

        ui->displayExpressionLine->setText("Введите степень корня");

        ui->calculatorLine->clear();
        isEnteringRootDegree = true;
        isRootDegreeConfirmed = false;
        isWaitingOperand = false;
        needClear = false;
    }
}


void MainWindow::on_actionButton_np_clicked()
{
    if (isEnteringRate && isPercentRateConfirmed) {
        QString currentCalculatorLineText = ui->calculatorLine->text();
        if (currentCalculatorLineText.isEmpty()) return;

        double base = currentCalculatorLineText.toDouble();
        if (rate < 0) {
            ui->calculatorLine->setText("Неправильный процент");
            isEnteringRate = false;
            isPercentRateConfirmed = false;
            return;
        }

        double percentResult = percentMultiplier * (base * rate / 100.0);

        QString strRate;
        strRate.setNum(rate);

        QString percentExpression;
        if (percentMultiplier != 1.0) {
            QString strPercentMultiplier;
            strPercentMultiplier.setNum(percentMultiplier);
            percentExpression = strPercentMultiplier + " * " + strRate + "% от " + currentCalculatorLineText;
        } else {
            percentExpression = strRate + "% от " + currentCalculatorLineText;
        }

        if (!pendingOperator.isEmpty()) {
            double leftOperand = currentResult;
            currentResult = calculate(currentResult, pendingOperator, percentResult);

            QString resultStr;
            resultStr.setNum(currentResult);
            ui->calculatorLine->setText(resultStr);

            QString prettyOperator = pendingOperator;
            prettyOperator.replace("/","÷").replace("*","×");

            QString strLeftOperand;
            strLeftOperand.setNum(leftOperand);

            ui->displayExpressionLine->setText(
                strLeftOperand + " " + prettyOperator + " " + percentExpression + " ="
                );

            pendingOperator = "";
        } else {
            ui->calculatorLine->setText(QString::number(percentResult));
            ui->displayExpressionLine->setText(percentExpression + " =");
        }

        isEnteringRate = false;
        isPercentRateConfirmed = false;
        isWaitingOperand = true;
        needClear = true;
        percentMultiplier = 0;
        rate = 0;
    } else if (isEnteringRate && !isPercentRateConfirmed) {
        QString currentCalculatorLineText = ui->calculatorLine->text();
        if (currentCalculatorLineText.isEmpty()) {
            ui->calculatorLine->setText("Введите процент");
            return;
        }

        rate = currentCalculatorLineText.toDouble();
        if (rate < 0) {
            ui->calculatorLine->setText("Неправильный процент");
            return;
        }

        QString strRate;
        strRate.setNum(rate);

        ui->displayExpressionLine->setText(strRate + "% (введите число)");

        ui->calculatorLine->clear();
        isPercentRateConfirmed = true;
        isWaitingOperand = false;
    } else {
        QString currentCalculatorLineText = ui->calculatorLine->text();

        if (!pendingOperator.isEmpty()) {
            percentMultiplier = 1.0;
        }
        else if (!currentCalculatorLineText.isEmpty()) {
            percentMultiplier = currentCalculatorLineText.toDouble();
        }
        else {
            percentMultiplier = 1.0;
        }

        ui->displayExpressionLine->setText("Введите процент");

        ui->calculatorLine->clear();
        isEnteringRate = true;
        isPercentRateConfirmed = false;
        isWaitingOperand = false;
        needClear = false;
    }
}
