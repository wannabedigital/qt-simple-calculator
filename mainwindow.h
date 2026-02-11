#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onDigitButtonClicked();

    void mathOperation();

    void on_actionButton_clear_clicked();

    void on_actionButton_allClear_clicked();

    void on_actionButton_comma_clicked();

    void on_actionButton_equal_clicked();

    double calculate(double leftOperand, const QString &pendingOperator, double rightOperand);

    // double findNumber(QString& expression, int& pos);

private:
    Ui::MainWindow *ui;

    double currentResult = 0;

    QString pendingOperator = "";

    bool isWaitingOperand = true;
    bool needClear = false;
};
#endif // MAINWINDOW_H
