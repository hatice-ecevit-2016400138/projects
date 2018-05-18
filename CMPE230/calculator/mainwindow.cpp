#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include<iostream>
#include<string.h>
#include <string>
#include <iostream>
#include <sstream>
using namespace std;
//this method sets up the buttons and links them into needed methods
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

       sumSoFar = 0.0;
       factorSoFar = 0.0;
       waitingForOperand = true;
        ishexa=false;

    connect(ui->pushButton_0,SIGNAL(released()),this,SLOT(digit_pressed()));
    connect(ui->pushButton_1,SIGNAL(released()),this,SLOT(digit_pressed()));
    connect(ui->pushButton_2,SIGNAL(released()),this,SLOT(digit_pressed()));
    connect(ui->pushButton_3,SIGNAL(released()),this,SLOT(digit_pressed()));
    connect(ui->pushButton_4,SIGNAL(released()),this,SLOT(digit_pressed()));
    connect(ui->pushButton_5,SIGNAL(released()),this,SLOT(digit_pressed()));
    connect(ui->pushButton_6,SIGNAL(released()),this,SLOT(digit_pressed()));
    connect(ui->pushButton_7,SIGNAL(released()),this,SLOT(digit_pressed()));
    connect(ui->pushButton_8,SIGNAL(released()),this,SLOT(digit_pressed()));
    connect(ui->pushButton_9,SIGNAL(released()),this,SLOT(digit_pressed()));
    connect(ui->pushButton_A,SIGNAL(released()),this,SLOT(hexadigit_pressed()));
    connect(ui->pushButton_B,SIGNAL(released()),this,SLOT(hexadigit_pressed()));
    connect(ui->pushButton_C,SIGNAL(released()),this,SLOT(hexadigit_pressed()));
    connect(ui->pushButton_D,SIGNAL(released()),this,SLOT(hexadigit_pressed()));
    connect(ui->pushButton_E,SIGNAL(released()),this,SLOT(hexadigit_pressed()));
    connect(ui->pushButton_F,SIGNAL(released()),this,SLOT(hexadigit_pressed()));
    connect(ui->pushButton_clr,SIGNAL(released()),this,SLOT(clr_pressed()));
    connect(ui->pushButton_plus,SIGNAL(released()),this,SLOT(addOperatorPressed()));
    connect(ui->pushButton_minus,SIGNAL(released()),this,SLOT(addOperatorPressed()));
    connect(ui->pushButton_mult,SIGNAL(released()),this,SLOT(multOperatorPressed()));
    connect(ui->pushButton_divide,SIGNAL(released()),this,SLOT(multOperatorPressed()));
    connect(ui->pushButton_dec,SIGNAL(released()),this,SLOT(on_pushButton_dec_released()));
    connect(ui->pushButton_equal,SIGNAL(released()),this,SLOT(equalClicked()));

}

MainWindow::~MainWindow()
{
    delete ui;
}
//this method converts the given hexadecimal number into a decimal number
int hexadecimalToDecimal(char hexVal[])
{

    int len = strlen(hexVal);

    // Initializing base value to 1, i.e 16^0
    int base = 1;

    int dec_val = 0;

    // Extracting characters as digits from last character
    for (int i=len-1; i>=0; i--)
    {
        // if character lies in '0'-'9', converting
        // it to integral 0-9 by subtracting 48 from
        // ASCII value.
        if (hexVal[i]>='0' && hexVal[i]<='9')
        {
            dec_val += (hexVal[i] - 48)*base;

            // incrementing base by power
            base = base * 16;
        }

        // if character lies in 'A'-'F' , converting
        // it to integral 10 - 15 by subtracting 55
        // from ASCII value
        else if ((hexVal[i]>='A' && hexVal[i]<='F'))
        {
            dec_val += (hexVal[i] - 55)*base;

            // incrementing base by power
            base = base*16;
        }
        else if ((hexVal[i]>='a' && hexVal[i]<='f'))
        {
            dec_val += (hexVal[i] - 87)*base;

            // incrementing base by power
            base = base*16;
        }
    }

    return dec_val;
}
//this method handles pressed digits

void MainWindow::digit_pressed()
{


    QPushButton *clickedButton = (QPushButton*)(sender());
        int digitValue = clickedButton->text().toInt();
        if (ui->label->text() == "0" && digitValue == 0.0)
            return;

        if (waitingForOperand) {
            ui->label->clear();
            waitingForOperand = false;
        }
        ui->label->setText(ui->label->text() + QString::number(digitValue));

}
//these two methods sets the mode of the calculator/hexa or decimal

void MainWindow::on_pushButton_dec_released()
{
    this->ishexa=false;
    clearAll();
}

void MainWindow::on_pushButton_hex_released()
{
    this->ishexa=true;
    clearAll();
}
//thhis method handles pressed hexa digits
void MainWindow::hexadigit_pressed()
{

    QPushButton *clickedButton = (QPushButton*)(sender());
    string str=clickedButton->text().toStdString();
    int n=str.length();
    char charArray[n+1];
    strcpy(charArray,str.c_str());

        int digitValue = hexadecimalToDecimal( charArray);
        if (ui->label->text() == "0" && digitValue == 0.0)
            return;

        if (waitingForOperand) {
            ui->label->clear();
            waitingForOperand = false;
        }
        ui->label->setText(ui->label->text() + clickedButton->text());


}
//this method clr's the screen
void MainWindow::clr_pressed(){
    if (waitingForOperand){

            return;}

        ui->label->setText("0");
        waitingForOperand = true;
}
//this method handle the operation + and -
void MainWindow::addOperatorPressed(){
     QPushButton * button=(QPushButton*)sender();
     QString clickedOperator = button->text();
    int operand;
    if(ishexa){

        string str=ui->label->text().toStdString();
        int n=str.length();
        char charArray[n+1];
        strcpy(charArray,str.c_str());
        operand = hexadecimalToDecimal(charArray);
        qDebug()<<operand;
    }
    else{
        operand = ui->label->text().toInt();
        qDebug()<<"decimal";
        qDebug()<<operand;
    }

    if (!pendingMultiplicativeOperator.isEmpty()) {
            if (!calculate(operand, pendingMultiplicativeOperator)) {
                abortOperation();
                return;
            }
            if(ishexa){
                std::stringstream ss;
                ss<< std::hex << factorSoFar; // int decimal_value
                std::string res ( ss.str() );

                ui->label->setText(QString::fromStdString(res));
                qDebug()<<"res";
            }
            else{
                ui->label->setText(QString::number(factorSoFar));
            }

            operand = factorSoFar;
            factorSoFar = 0;
            pendingMultiplicativeOperator.clear();
        }

        if (!pendingAdditiveOperator.isEmpty()) {
            if (!calculate(operand, pendingAdditiveOperator)) {
                abortOperation();
                return;
            }
            if(ishexa){
                std::stringstream ss;
                ss<< std::hex << sumSoFar; // int decimal_value
                std::string res ( ss.str() );

                ui->label->setText(QString::fromStdString(res));
                qDebug()<<"res";
            }
            else{
                ui->label->setText(QString::number(sumSoFar));
            }

        } else {
            sumSoFar = operand;
        }

        pendingAdditiveOperator = clickedOperator;
        waitingForOperand = true;
}
//this method handles the operations * and /
void MainWindow::multOperatorPressed(){
    QPushButton * button=(QPushButton*)sender();
       QString clickedOperator = button->text();
       int operand;
       if(ishexa){
           string str=ui->label->text().toStdString();
           qDebug()<<QString::fromStdString(str);
           int n=str.length();
           char charArray[n+1];
           strcpy(charArray,str.c_str());
           operand = hexadecimalToDecimal(charArray);
           qDebug()<<operand;
       }
       else{
       operand = ui->label->text().toInt();}

       if (!pendingMultiplicativeOperator.isEmpty()) {
           if (!calculate(operand, pendingMultiplicativeOperator)) {
               abortOperation();
               return;
           }
           if(ishexa){
               std::stringstream ss;
               ss<< std::hex << factorSoFar; // int decimal_value
               std::string res ( ss.str() );

               ui->label->setText(QString::fromStdString(res));

           }
           else{
               ui->label->setText(QString::number(factorSoFar));
           }

       } else {
           factorSoFar = operand;
       }

       pendingMultiplicativeOperator = clickedOperator;
       waitingForOperand = true;
}
//this method does all of the calculations
bool MainWindow::calculate(int rightOperand, const QString &pendingOperator)
{

    qDebug()<<pendingOperator;
    string str=pendingOperator.toStdString();
    if (pendingOperator.compare("+") == 0) {

        sumSoFar += rightOperand;
    } else if (pendingOperator.compare("-") == 0 ) {
        sumSoFar -= rightOperand;
    } else if (pendingOperator.compare("*")== 0) {
        factorSoFar *= rightOperand;
    } else if (pendingOperator.compare("/")==0) {
        if (rightOperand == 0)
            return false;
        factorSoFar /= rightOperand;
    }
    return true;
}
//this method is called when there is an error with the operation
void MainWindow::abortOperation()
{
    clearAll();
    ui->label->setText(tr("####"));
}
//this method clears all of the variables and resets the caalculator
void MainWindow::clearAll()
{
    sumSoFar = 0;
    factorSoFar = 0;
    pendingAdditiveOperator.clear();
    pendingMultiplicativeOperator.clear();
    ui->label->setText("0");
    waitingForOperand = true;
}
//this method handles the '=' click
void MainWindow::equalClicked()
{
    int operand;
    if(ishexa){
        string str=ui->label->text().toStdString();
        int n=str.length();
        char charArray[n+1];
        strcpy(charArray,str.c_str());
        operand = hexadecimalToDecimal(charArray);
    qDebug()<<operand;
    }
    else{
            operand=ui->label->text().toInt();}

    if (!pendingMultiplicativeOperator.isEmpty()) {
        if (!calculate(operand, pendingMultiplicativeOperator)) {
            abortOperation();
            return;
        }
        operand = factorSoFar;
        factorSoFar = 0;
        pendingMultiplicativeOperator.clear();
    }
    if (!pendingAdditiveOperator.isEmpty()) {
        if (!calculate(operand, pendingAdditiveOperator)) {
            abortOperation();
            return;
        }
        pendingAdditiveOperator.clear();
    } else {
        sumSoFar = operand;
    }
    if(ishexa){
        std::stringstream ss;
        ss<< std::hex << sumSoFar; // int decimal_value
        std::string res ( ss.str() );

        ui->label->setText(QString::fromStdString(res));

    }
    else{
        ui->label->setText(QString::number(sumSoFar));
    }

    sumSoFar = 0;
    waitingForOperand = true;
}




