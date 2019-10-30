#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QString"
#include "QMessageBox"
#include <iostream>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)


{
    ui->setupUi(this);

    QPixmap pix("C:/Users/Yogi/Dropbox/TrafficSystem/TrafficSystem/BackgroundImage.png"); //Change location to where image is saved.
    ui->backgroundImage->setPixmap(pix);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_GenButton_clicked()
{
    //Variables from spinboxes
    QString spin1=ui->CarSpinBox1->text();
    QString spin2=ui->SecSpinBox1->text();
    QString spin3=ui->CarSpinBox2->text();
    QString spin4=ui->SecSpinBox2->text();
    QString spin5=ui->CarSpinBox3->text();
    QString spin6=ui->SecSpinBox3->text();
    QString spin7=ui->CarSpinBox4->text();
    QString spin8=ui->SecSpinBox4->text();
    //Spinbox values to integers
    int ispin1=spin1.toInt();
    int ispin2=spin2.toInt();
    int ispin3=spin3.toInt();
    int ispin4=spin4.toInt();
    int ispin5=spin5.toInt();
    int ispin6=spin6.toInt();
    int ispin7=spin7.toInt();
    int ispin8=spin8.toInt();

    //Transfer spinbox values into textbox for copying purposes.
    ui->lineEdit->setText("Lane 1 Cars=" + spin1 + ", Time Spent=" + spin2 + "\n" + "Lane 2 Cars=" + spin3 + ", Time Spent=" + spin4 + "Lane 3 Cars=" + spin5 + ", Time Spent=" + spin6 + "\n" + "Lane 4 Cars=" + spin7 + ", Time Spent=" + spin8);

    if(ispin2 == 60 | ispin4 == 60 | ispin6 == 60 | ispin8 == 60) { //Priority1: any cars waiting longer 60 seconds go first
        if (ispin2 == 60 | ispin6 == 60) {
            QMessageBox::information(this,tr("Lane Order"),tr("Lane 1 and 3 should go first."));
        } else {
            QMessageBox::information(this,tr("Lane Order"),tr("Lane 2 and 4 should go first."));
        }
    } else {
        int max(std::max({ispin1,ispin3,ispin5,ispin7}));
        if (max == ispin1 |max == ispin5){
            QMessageBox::information(this,tr("Lane Order"),tr("Lane 1 and 3 should go first."));
        } else {
            QMessageBox::information(this,tr("Lane Order"),tr("Lane 2 and 4 should go first."));
        }
    }



}
