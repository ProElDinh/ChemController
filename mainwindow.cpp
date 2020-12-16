#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{


    ui->setupUi(this);


    //_Thread = new QThread(this);
    // Указывать родителя нет необходимости. Родителем станет поток, когда переместим в него объект прибора.
    _chemconroller = new ChemController;
        /* Перемещаем объект прибора в отдельный поток, чтобы синхронные ожидающие операции не блокировали
    основной GUI-поток. Создаем соединение: Удаляем объект прибора при окончании работы потока.*/

    //_chemconroller->moveToThread(_Thread);

    //connect(_Thread, SIGNAL(finished()), _chemconroller, SLOT(deleteLater()));

    //_Thread->start();  // Запускаем поток

    //проверка соединения

    if (!_chemconroller->isConnected()){
        StatusDisconnected();
    }

    connect(ui->connect, &QAction::triggered, this, &MainWindow:: Connect); // Подключить устройство


    connect(ui->disconnect, &QAction::triggered, _chemconroller, &ChemController:: ClosePort);  // Отключить устройство
    connect(ui->disconnect, &QAction::triggered, [this](){
        StatusDisconnected();
    });

    connect(ui->OnTemp, &QPushButton::clicked, _chemconroller, &ChemController::turnOnTemp);
    connect(ui->OnTemp, &QPushButton::clicked, [this](){
        ui->set_temp -> setEnabled(true);
    });

    connect(ui->OffTemp, &QPushButton::clicked, _chemconroller, &ChemController::turnOffTemp);
    connect(ui->OffTemp, &QPushButton::clicked, [this](){
        ui->set_temp -> setEnabled(false);
    });
    connect(ui->OpenScript, &QAction::triggered, _chemconroller, &ChemController::OpenPython);

    // При нажатии на кнопку "Задать", передаются все соответствующие параметры выставленные в set_tempbox.

    connect(ui->set_temp, &QPushButton::clicked, [this](){
            emit SetTempRequest(ui->set_tempbox->value());
    });

    // Подключение сигналов

    connect(this, &MainWindow::SetTempRequest, _chemconroller, &ChemController::setTemp);

    connect(_chemconroller,&ChemController::error_, this, &MainWindow::ErrorMessage); // Подключение сигнала

}
MainWindow::~MainWindow()
{
    //_Thread->quit();
    //_Thread->wait(1000);
    delete ui;

}

void MainWindow::StatusBar(QString status)
{
    _status -> setText("Состояние : " + status);
    //ui->statusBar->setStyleSheet("color: red");
    ui->statusBar->addPermanentWidget(_status);
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
    StatusDisconnected();
    qDebug() << "Устройство отключено";
}

void MainWindow::ErrorMessage(QString err){
    QMessageBox::critical(this,
                        "Ошибка подключения", err, QMessageBox::Ok);
}
