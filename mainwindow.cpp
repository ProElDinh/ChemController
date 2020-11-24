#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "SimpleExchange.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    StatusBar();
    serialPort = new QSerialPort(this);// новый экзампляр класса AbstractSerial
    serialBuffer = "";
    QObject::connect(serialPort, SIGNAL(readyRead()), this, SLOT(Read()));
    QObject::connect(serialPort, SIGNAL(writeData(QByteArray)), this, SLOT(WriteData()));

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

void MainWindow:: Read(){ // получаем данные
    serialData = serialPort -> readAll(); // читаем все
    serialBuffer = QString::fromStdString(serialData.toStdString());
    qDebug() << serialData;

}

void MainWindow:: Open(){
    if (serialPort->isOpen()){
        serialPort->close();
    }
    serialPort -> setPortName("com4"); // указываем параметры порта (далее)
    serialPort -> setBaudRate(QSerialPort::Baud9600);
    serialPort -> setDataBits(QSerialPort::Data8);
    serialPort -> setParity(QSerialPort::NoParity);
    serialPort -> setStopBits(QSerialPort :: OneStop);
    serialPort -> setFlowControl(QSerialPort:: NoFlowControl);
    serialPort -> open((QSerialPort:: ReadWrite)); // открыли порт
 // соединяем чтение - прием данных
    serialPort -> write("hello");
    QObject::connect(serialPort, SIGNAL(readyRead()), this, SLOT(Read()));
    foreach (const QSerialPortInfo &info, QSerialPortInfo :: availablePorts()){
        QSerialPort port;
        port.setPort(info);
        if (port.open(QIODevice::ReadWrite)){
            qDebug() << "Название: " + info.portName() + " " + info.description() + info.manufacturer();
        }

    }

    // получаем список доступных в системе com портов при помощи QSerialPort

}
void MainWindow:: Close(){
    serialPort->close();
}

void MainWindow::WritetoData(){
    if (serialPort->isOpen()){
     serialPort->write(":");



     serialPort  -> write("=");
}
}

void MainWindow::on_action_triggered()
{
    Open();
}

void MainWindow::on_action_2_triggered()
{
    Close();
}
