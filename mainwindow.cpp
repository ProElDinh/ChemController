#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "SimpleExchange.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    StatusBar();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::StatusBar()
{
    status = new QLabel(this);
    status -> setText("Состояние : Отключено");
    ui->statusBar->setStyleSheet("color: red");
    ui->statusBar->addPermanentWidget(status);
}
