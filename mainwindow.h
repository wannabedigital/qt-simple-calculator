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

    double calculate(double leftOperand, const QString &pendingOperator, double rightOperand);

    void on_actionButton_clear_clicked();

    void on_actionButton_allClear_clicked();

    void on_actionButton_comma_clicked();

    void on_actionButton_equal_clicked();

    void on_actionButton_negate_clicked();

    void on_actionButton_percent_clicked();

    void on_actionButton_cos_clicked();

    void on_actionButton_nRoot_clicked();

    void on_actionButton_np_clicked();

private:
    Ui::MainWindow *ui;

    double currentResult = 0;

    QString pendingOperator = "";
    bool isWaitingOperand = true;
    bool needClear = false;

    double rootDegree = 0;
    double rootMultiplier = 0;
    bool isEnteringRootDegree = false;
    bool isRootDegreeConfirmed = false;

    double rate = 0;
    double percentMultiplier = 0;
    bool isEnteringRate = false;
    bool isPercentRateConfirmed = false;

    double cosAngle = 0;
    double cosMultiplier = 0;
    bool isEnteringCos = false;
};
#endif // MAINWINDOW_H
