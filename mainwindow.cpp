#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPixmap pix("C:/Users/Yogi/Dropbox/TrafficSystem/TrafficSystem/BackgroundImage.png");
    ui->backgroundImage->setPixmap(pix);
}

MainWindow::~MainWindow()
{
    delete ui;
}

