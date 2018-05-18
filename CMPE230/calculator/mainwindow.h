#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool ishexa;
    int sumSoFar=0;
    int factorSoFar=0;
    QString pendingAdditiveOperator;
    QString pendingMultiplicativeOperator;
    bool waitingForOperand;

private:
    Ui::MainWindow *ui;
private slots:
    void digit_pressed();
    void on_pushButton_hex_released();
    void on_pushButton_dec_released();
    void hexadigit_pressed();
    void clr_pressed();
    void addOperatorPressed();
    void multOperatorPressed();
    bool calculate(int rightOperand, const QString &pendingOperator);
    void abortOperation();
    void clearAll();
    void equalClicked();
};

#endif // MAINWINDOW_H
