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
}

void MainWindow::onDigitButtonClicked()
{
    if (needClear) {
        ui->calculatorLine->clear();
        needClear = false;
    }

    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    QString digit = button->property("digit").toString();

    QString currentCalculatorLineText = ui->calculatorLine->text();

    ui->calculatorLine->setText(currentCalculatorLineText + digit);
}


void MainWindow::on_actionButton_comma_clicked()
{
    if (needClear) {
        ui->calculatorLine->clear();
        needClear = false;
    }

    QString currentCalculatorLineText = ui->calculatorLine->text();
    ui->calculatorLine->setText(currentCalculatorLineText + ",");
}




void MainWindow::on_actionButton_div_clicked()
{
    if (needClear) {
        ui->calculatorLine->clear();
        needClear = false;
    }

    QString currentCalculatorLineText = ui->calculatorLine->text();
    ui->calculatorLine->setText(currentCalculatorLineText + " ÷ ");
}




void MainWindow::on_actionButton_mult_clicked()
{
    if (needClear) {
        ui->calculatorLine->clear();
        needClear = false;
    }

    QString currentCalculatorLineText = ui->calculatorLine->text();
    ui->calculatorLine->setText(currentCalculatorLineText + " × ");
}

void MainWindow::on_actionButton_substraction_clicked()
{
    if (needClear) {
        ui->calculatorLine->clear();
        needClear = false;
    }

    QString currentCalculatorLineText = ui->calculatorLine->text();
    ui->calculatorLine->setText(currentCalculatorLineText + " - ");
}




void MainWindow::on_actionButton_addition_clicked()
{
    if (needClear) {
        ui->calculatorLine->clear();
        needClear = false;
    }

    QString currentCalculatorLineText = ui->calculatorLine->text();
    ui->calculatorLine->setText(currentCalculatorLineText + " + ");
}

double MainWindow::findNumber(QString& expression, int& pos)
{
    double value = 0, frac = 0, div = 1;
    bool isFrac = false;

    while (pos < expression.length() && (expression[pos].isDigit() || expression[pos] == '.')) {
        if (expression[pos] == '.'){
            isFrac = true;
        } else if (!isFrac) {
            value *= 10;
            value += expression[pos].digitValue();
        } else {
            div *= 10;
            frac += expression[pos].digitValue();
            frac /= div;
        }

        pos++;
    }

    return value+frac;
}

// TODO: Подписать ошибки

void MainWindow::on_actionButton_equal_clicked()
{
    if (needClear) {
        ui->calculatorLine->clear();
        needClear = false;
        return;
    }

    QString expression = ui->calculatorLine->text();
    expression.replace("÷", "/");
    expression.replace("×", "*");
    expression.replace(",", ".");
    expression.replace(" ", "");

    if (expression.isEmpty()) {
        ui->calculatorLine->setText("Ошибка");
        needClear = true;
        return;
    } else {
        int position = 0;

        double num1 = findNumber(expression, position);
        if (position >= expression.length()) {
            ui->calculatorLine->setText("Ошибка");
            needClear = true;
            return;
        }

        QChar op = expression[position++];
        if (position >= expression.length()) {
            ui->calculatorLine->setText("Ошибка");
            needClear = true;
            return;
        }

        double num2 = findNumber(expression, position);
        if (position < expression.length()) {
            ui->calculatorLine->setText("Ошибка");
            needClear = true;
            return;
        }

        double result = 0.0;

        if (op == '+') {
            result = num1 + num2;
        } else if (op == '-') {
            result = num1 - num2;
        } else if (op == '/') {
            if (num2 != 0) {
                result = num1 / num2;
            } else {
                ui->calculatorLine->setText("Ошибка");
                needClear = true;
                return;
            }
        } else if (op == '*') {
            result = num1 * num2;
        } else {
            ui->calculatorLine->setText("Ошибка");
            needClear = true;
            return;
        }

        QString strResult;
        strResult.setNum(result);

        ui->calculatorLine->setText(strResult.replace(".", ","));
    }

    needClear = true;
}

