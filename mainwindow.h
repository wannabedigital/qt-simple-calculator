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
    void on_actionButton_clear_clicked();

    void on_actionButton_allClear_clicked();

    void onDigitButtonClicked();

    void on_actionButton_comma_clicked();

    void on_actionButton_div_clicked();

    void on_actionButton_mult_clicked();

    void on_actionButton_substraction_clicked();

    void on_actionButton_addition_clicked();

    void on_actionButton_equal_clicked();

private:
    Ui::MainWindow *ui;

    bool needClear = false;
};
#endif // MAINWINDOW_H
