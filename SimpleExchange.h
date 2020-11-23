#ifndef SIMPLEEXCHANGE_H
#define SIMPLEEXCHANGE_H
#include "mainwindow.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <string>

class SimpleExchange {
private:
    QSerialPort *_serialPort;
    quint16* Crc16Table; // Создание динамического массива
    QByteArray *ba; // Массив данных от контроллера

public:
    SimpleExchange();
    ~SimpleExchange();
    quint16 Crc16(quint16 pcBlock[], int len);
    void getPortName(); // Получение информации об доступных портах
    bool IsOpen();
    void Open(QString port);
    void Write(); // отправить массив данных в контроллер
    QByteArray Read(); // получить массив данных от контроллера
};

#endif // SIMPLEEXCHANGE_H
