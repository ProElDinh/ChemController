#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "SimpleExchange.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    StatusBar();
    serialcheck();
    SimpleExchange ui;
    ui.getPortName();
}

MainWindow::~MainWindow()
{
    delete ui;
    // закрываем соединение при выходе
    serialPort -> close();
    delete serialPort;
}

void MainWindow::StatusBar()
{
    status = new QLabel(this);
    status -> setText("Состояние : Отключено");
    ui->statusBar->setStyleSheet("color: red");
    ui->statusBar->addPermanentWidget(status);
}

void MainWindow:: serialRecieve(){ // получаем данные
    QByteArray ba; // массив байтов
    ba = serialPort -> readAll(); // читаем все
    ui -> label -> setText(ba.toHex());// переводим в hex

}

void MainWindow:: serialcheck(){
    serialPort = new QSerialPort(this);// новый экзампляр класса AbstractSerial
    if (serialPort->isOpen()){
        serialPort->close();
    }
    serialPort -> setPortName("com4"); // указываем параметры порта (далее)
    serialPort -> setBaudRate(QSerialPort::Baud9600);
    serialPort -> setDataBits(QSerialPort::Data8);
    serialPort -> setParity(QSerialPort::NoParity);
    serialPort -> setStopBits(QSerialPort :: OneStop);
    serialPort -> setFlowControl(QSerialPort:: NoFlowControl);
    serialPort -> open((QIODevice:: ReadWrite)); // открыли порт
    serialPort -> write("Hello"); //записываем данные

    QObject::connect(serialPort, SIGNAL(readyRead()), this, SLOT(serialRecieve()));// соединяем чтение - прием данных

    // получаем список доступных в системе com портов при помощи QSerialPort

}
