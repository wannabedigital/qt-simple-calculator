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



void MainWindow::on_actionButton_equal_clicked()
{
    QString expression = ui->calculatorLine->text();
    expression.replace("÷", "/");
    expression.replace("×", "*");
    expression.replace(",", ".");
    expression.replace(" ", "");


}

