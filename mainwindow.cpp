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

void MainWindow::updateDisplay()
{
    QString html = QString(
        "<html>"
            "<body style='text-align: right; font-family: JetBrains Mono;'>"
                "<p style='font-size: 24px; margin: 0; margin-bottom: 8px;'>%1</p>"
                "<p style='font-size: 32px; margin: 0;'>%2</p>"
            "</body>"
        "</html>"
    ).arg(currentExpression, currentInput);

    ui->displayTextEdit->setHtml(html);
}

void MainWindow::on_actionButton_clear_clicked()
{
    QString currentCalculatorLineText = currentInput;

    if (!currentCalculatorLineText.isEmpty()) {
        int textLength = currentCalculatorLineText.length();

        currentInput = (currentCalculatorLineText.left(textLength - 1));
        updateDisplay();
    }
}

void MainWindow::on_actionButton_allClear_clicked()
{
    currentInput = "";
    currentExpression = "";
    updateDisplay();
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
        currentInput = "";
        updateDisplay();
        needClear = false;
    }

    QPushButton *button = (QPushButton *)(sender());
    if (!button) return;

    if (isWaitingOperand && !isEnteringRootDegree && !isEnteringRate && !isEnteringCos) {
        currentInput = "";
        updateDisplay();
        isWaitingOperand = false;
    }

    QString digit = button->property("digit").toString();

    QString currentCalculatorLineText = currentInput;

    QString strNum = (currentCalculatorLineText + digit);

    currentInput = (strNum);
    updateDisplay();
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
            currentInput = ("Ошибка: не найден оператор");
            updateDisplay();
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

    QString currentCalculatorLineText = currentInput;
    double operand = currentCalculatorLineText.toDouble();
    QString clickedOperator = button->property("operation").toString();
    QString prettyOperator = clickedOperator;
    prettyOperator.replace("/","÷").replace("*","×");


    if (!pendingOperator.isEmpty()) {
        currentResult = calculate(currentResult, pendingOperator, operand);
        QString resultStr;
        resultStr.setNum(currentResult);
        currentInput = (resultStr);
        currentExpression = (resultStr + " " + prettyOperator);
        updateDisplay();

    } else {
        currentResult = operand;
        QString resultStr;
        resultStr.setNum(currentResult);
        currentExpression = (resultStr + " " + prettyOperator);
        updateDisplay();
    }

    pendingOperator = clickedOperator;
    isWaitingOperand = true;
}

void MainWindow::on_actionButton_comma_clicked()
{
    if (needClear) {
        currentInput = "";
        updateDisplay();
        needClear = false;
    }

    if (isWaitingOperand) {
        currentInput = ("0");
        updateDisplay();
        isWaitingOperand = false;
    }

    QString currentCalculatorLineText = currentInput;

    if (currentCalculatorLineText.contains('.')) return;

    currentInput = (currentCalculatorLineText + ".");
    updateDisplay();
}

void MainWindow::on_actionButton_equal_clicked()
{
    if (needClear) {
        currentInput = "";
        updateDisplay();
        needClear = false;
        return;
    }

    QString currentCalculatorLineText = currentInput;
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
        currentInput = (resultStr);
        updateDisplay();

        QString currentExpressionLine = currentExpression;
        currentExpression = (
            currentExpressionLine + " " + currentCalculatorLineText + " ="
        );
        updateDisplay();

        pendingOperator = "";
        isWaitingOperand = true;
        needClear = true;
    } else {
        currentInput = (currentCalculatorLineText);
        currentExpression = (currentCalculatorLineText + " =");
        updateDisplay();
    }
}


void MainWindow::on_actionButton_negate_clicked()
{
    QString currentCalculatorLineText = currentInput;
    if (currentCalculatorLineText.isEmpty()) {
        currentInput = ("Сначала введите число");
        updateDisplay();
        return;
    }

    double negateNum = currentCalculatorLineText.toDouble() * (-1);

    QString strNegateNum;
    strNegateNum.setNum(negateNum);

    currentInput = (strNegateNum);
    updateDisplay();
}


void MainWindow::on_actionButton_percent_clicked()
{
    QString currentCalculatorLineText = currentInput;
    if (currentCalculatorLineText.isEmpty() || currentCalculatorLineText == "0") return;

    double percentNum = currentCalculatorLineText.toDouble() * 0.01;

    QString strPercentNum;
    strPercentNum.setNum(percentNum);

    currentInput = (strPercentNum);
    updateDisplay();
}


void MainWindow::on_actionButton_cos_clicked()
{
    if (isEnteringCos) {
        QString currentCalculatorLineText = currentInput;
        if (currentCalculatorLineText.isEmpty()) return;

        cosAngle = currentCalculatorLineText.toDouble();
        double cosResult = cosMultiplier * std::cos(cosAngle);

        QString resultStr;
        resultStr.setNum(cosResult);

        QString cosExpression;
        if (cosMultiplier != 1.0) {
            QString strCosMultiplier;
            strCosMultiplier.setNum(cosMultiplier);
            cosExpression = strCosMultiplier + " × cos(" + currentCalculatorLineText + ")";
        } else {
            cosExpression = "cos(" + currentCalculatorLineText + ")";
        }

        if (!pendingOperator.isEmpty()) {
            double leftOperand = currentResult;
            currentResult = calculate(currentResult, pendingOperator, cosResult);

            QString resultStr;
            resultStr.setNum(currentResult);
            currentInput = (resultStr);
            updateDisplay();

            QString prettyOperator = pendingOperator;
            prettyOperator.replace("/","÷").replace("*","×");

            QString strLeftOperand;
            strLeftOperand.setNum(leftOperand);

            currentExpression = (
                strLeftOperand + " " + prettyOperator + " " + cosExpression + " ="
                );
            updateDisplay();

            pendingOperator = "";
        } else {
            currentInput = (resultStr);
            currentExpression = (cosExpression + " =");
            updateDisplay();
        }

        isEnteringCos = false;
        isWaitingOperand = true;
        needClear = true;
        cosMultiplier = 0;
        cosAngle = 0;
    } else {
        QString currentCalculatorLineText = currentInput;

        if (!pendingOperator.isEmpty()) {
            cosMultiplier = 1.0;
        } else if (!currentCalculatorLineText.isEmpty()) {
            cosMultiplier = currentCalculatorLineText.toDouble();
        } else {
            cosMultiplier = 1.0;
        }

        currentExpression = ("Введите угол (в радианах)");
        currentInput = "";
        updateDisplay();

        isEnteringCos = true;
        isWaitingOperand = false;
        needClear = false;
    }
}


void MainWindow::on_actionButton_nRoot_clicked()
{
    if (isEnteringRootDegree && isRootDegreeConfirmed) {
        QString currentCalculatorLineText = currentInput;
        if (currentCalculatorLineText.isEmpty()) return;

        double radicand = currentCalculatorLineText.toDouble();
        if (rootDegree <= 0) {
            currentInput = ("Неправильная степень корня");
            updateDisplay();
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
            currentInput = (resultStr);

            QString prettyOperator = pendingOperator;
            prettyOperator.replace("/","÷").replace("*","×");

            QString strLeftOperand;
            strLeftOperand.setNum(leftOperand);

            currentExpression = (
                strLeftOperand + " " + prettyOperator + " " + rootExpression + " ="
                );

            updateDisplay();

            pendingOperator = "";
        } else {
            currentInput = (QString::number(rootResult));
            currentExpression = (rootExpression + " =");
            updateDisplay();
        }

        isEnteringRootDegree = false;
        isRootDegreeConfirmed = false;
        isWaitingOperand = true;
        needClear = true;
        rootMultiplier = 0;
        rootDegree = 0;
    } else if (isEnteringRootDegree && !isRootDegreeConfirmed) {
        QString currentCalculatorLineText = currentInput;
        if (currentCalculatorLineText.isEmpty()) {
            currentInput = ("Введите степень корня");
            updateDisplay();
            return;
        }

        rootDegree = currentCalculatorLineText.toDouble();
        if (rootDegree <= 0) {
            currentInput = ("Неправильная степень корня");
            updateDisplay();
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

        currentExpression = (prettyDegree + "√ (введите число под корнем)");

        currentInput = "";
        updateDisplay();

        isRootDegreeConfirmed = true;
        isWaitingOperand = false;
    } else {
        QString currentCalculatorLineText = currentInput;

        if (!pendingOperator.isEmpty()) {
            rootMultiplier = 1.0;
        }
        else if (!currentCalculatorLineText.isEmpty()) {
            rootMultiplier = currentCalculatorLineText.toDouble();
        }
        else {
            rootMultiplier = 1.0;
        }

        currentExpression = ("Введите степень корня");

        currentInput = "";
        updateDisplay();

        isEnteringRootDegree = true;
        isRootDegreeConfirmed = false;
        isWaitingOperand = false;
        needClear = false;
    }
}


void MainWindow::on_actionButton_np_clicked()
{
    if (isEnteringRate && isPercentRateConfirmed) {
        QString currentCalculatorLineText = currentInput;
        if (currentCalculatorLineText.isEmpty()) return;

        double base = currentCalculatorLineText.toDouble();
        if (rate < 0) {
            currentInput = ("Неправильный процент");
            updateDisplay();
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
            currentInput = (resultStr);

            QString prettyOperator = pendingOperator;
            prettyOperator.replace("/","÷").replace("*","×");

            QString strLeftOperand;
            strLeftOperand.setNum(leftOperand);

            currentExpression = (
                strLeftOperand + " " + prettyOperator + " " + percentExpression + " ="
                );

            updateDisplay();

            pendingOperator = "";
        } else {
            currentInput = (QString::number(percentResult));

            currentExpression = (percentExpression + " =");
            updateDisplay();
        }

        isEnteringRate = false;
        isPercentRateConfirmed = false;
        isWaitingOperand = true;
        needClear = true;
        percentMultiplier = 0;
        rate = 0;
    } else if (isEnteringRate && !isPercentRateConfirmed) {
        QString currentCalculatorLineText = currentInput;
        if (currentCalculatorLineText.isEmpty()) {
            currentInput = ("Введите процент");
            updateDisplay();
            return;
        }

        rate = currentCalculatorLineText.toDouble();
        if (rate < 0) {
            currentInput = ("Неправильный процент");
            updateDisplay();
            return;
        }

        QString strRate;
        strRate.setNum(rate);

        currentExpression = (strRate + "% (введите число)");

        currentInput = "";
        updateDisplay();

        isPercentRateConfirmed = true;
        isWaitingOperand = false;
    } else {
        QString currentCalculatorLineText = currentInput;

        if (!pendingOperator.isEmpty()) {
            percentMultiplier = 1.0;
        }
        else if (!currentCalculatorLineText.isEmpty()) {
            percentMultiplier = currentCalculatorLineText.toDouble();
        }
        else {
            percentMultiplier = 1.0;
        }

        currentExpression = ("Введите процент");

        currentInput = "";
        updateDisplay();

        isEnteringRate = true;
        isPercentRateConfirmed = false;
        isWaitingOperand = false;
        needClear = false;
    }
}
