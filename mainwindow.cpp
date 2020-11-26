#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    _Thread = new QThread(this);
    // Указывать родителя нет необходимости. Родителем станет поток, когда переместим в него наш объект излучателя.
    _chemconroller = new ChemController;
    /* Перемещаем объект излучателя в отдельный поток, чтобы синхронные ожидающие операции не блокировали
    основной GUI-поток. Создаем соединение: Удаляем объект излучателя при окончании работы потока.
    Запускаем поток.*/
    _chemconroller->moveToThread(_Thread);
    connect(_Thread, SIGNAL(finished()), _chemconroller, SLOT(deleteLater()));
    _Thread->start();

    //проверка соединения
    if (!_chemconroller -> isConnected()){
        ui -> disconnect -> setEnabled(false);
    }


    connect(ui->connect, &QAction::triggered, _chemconroller, &ChemController::OpenPort);
    connect(ui->connect, &QAction::triggered, [this](){
            ui->connect->setEnabled(false);
            ui->disconnect->setEnabled(true);
});
    connect(ui->disconnect, &QAction::triggered, _chemconroller, &ChemController::ClosePort);
        connect(ui->disconnect, &QAction::triggered, [this](){
            ui->connect->setEnabled(true);
            ui->disconnect->setEnabled(false);
        });

}
MainWindow::~MainWindow()
{
    _Thread->quit();
    _Thread->wait(1000);
    delete ui;

}

void MainWindow::StatusBar()
{
    status = new QLabel(this);
    status -> setText("Состояние : Отключено");
    ui->statusBar->setStyleSheet("color: red");
    ui->statusBar->addPermanentWidget(status);
}


