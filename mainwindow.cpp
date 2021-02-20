#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    _chemconroller = new ChemController;
    _reacSettings = new FormReacSettings;
    _tStatConfig = new FormTStatConfig;

    //проверка соединения

    if (!_chemconroller->isConnected()){
        StatusDisconnected();
    }

    connect(ui->connect, &QAction::triggered, this, &MainWindow:: Connect); // Подключить устройство

    connect(ui->disconnect, &QAction::triggered, this, &MainWindow:: Disconnect);  // Отключить устройство


    connect(ui->OnTemp, &QPushButton::clicked, this, &MainWindow::TurnOnTemp);

    connect(ui->OffTemp, &QPushButton::clicked, this, &MainWindow::TurnOffTemp);

    connect(ui->OpenScript, &QAction::triggered, _chemconroller, &ChemController::OpenPython);

    connect(ui->reacSetButton,&QAction::triggered, this, &MainWindow::ReacSettings);

     connect(ui->TStatConfigButton,&QAction::triggered, this, &MainWindow::TStatConfig);
    // При нажатии на кнопку "Задать", передаются все соответствующие параметры выставленные в set_tempbox.


    // Подключение сигналов

    connect(_chemconroller,&ChemController::error_, this, &MainWindow::ErrorMessage); // Подключение сигнала

}
MainWindow::~MainWindow()
{

    delete _chemconroller;
    delete _reacSettings;
    delete _tStatConfig;
    delete ui;

}

void MainWindow::StatusBar(QString status)
{
    _status -> setText("Состояние : " + status);
    //ui->statusBar->setStyleSheet("color: red");
    ui->statusBar->addPermanentWidget(_status);
}


void MainWindow::ReacSettings(){

    _reacSettings -> show();

}

void MainWindow::TStatConfig(){
    _tStatConfig -> show();
}

void MainWindow::StatusConnected(){
        ui->  connect->setEnabled(false);
        ui->  disconnect->setEnabled(true);
        ui -> set_temp -> setEnabled(false);
        ui -> OffTemp -> setEnabled(true);
        ui -> OnTemp -> setEnabled(true);
        ui -> set_tempbox -> setEnabled(true);
        ui -> setport -> setEnabled(false);
        StatusBar("Подключено");
}

void MainWindow::StatusDisconnected(){
    ui->  connect->setEnabled(true);
    ui->  disconnect->setEnabled(false);
    ui -> set_temp -> setEnabled(false);
    ui -> OffTemp -> setEnabled(false);
    ui -> OnTemp -> setEnabled(false);
    ui -> set_tempbox -> setEnabled(false);
    ui -> setport -> setEnabled(true);
    ui -> tab_4 -> setEnabled(false);
    StatusBar("Отключено");
}


void MainWindow:: Connect(){
    try {
        _chemconroller -> OpenPort();
        StatusConnected();
    }  catch (...) {
        ErrorMessage("Ошибка подключения");
    }
}


void MainWindow:: Disconnect(){
    _chemconroller-> ClosePort();
    StatusDisconnected();
}

void MainWindow::TurnOnTemp(){
    try {
        _chemconroller->TStatEnable();
        ui->set_temp -> setEnabled(true);
    }  catch (...) {
        ErrorMessage("Ошибка. Неудалось включить термостат!");
    }

}

void MainWindow::TurnOffTemp(){
    try {
        _chemconroller-> TStatDisable();
        ui->set_temp -> setEnabled(false);
    }  catch (...) {
        ErrorMessage("Ошибка. Неудалось включить термостат!");
    }

}

void MainWindow::ErrorMessage(QString err){
    QMessageBox::critical(this,
                        "Ошибка подключения", err, QMessageBox::Ok);
}

void MainWindow::on_set_tempbox_valueChanged(double arg1)
{
    qDebug() << arg1;
}
