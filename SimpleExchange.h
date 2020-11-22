#ifndef SIMPLEEXCHANGE_H
#define SIMPLEEXCHANGE_H
#include <QSerialPort>
#include <QSerialPortInfo>

class SimpleExchange {
private:

    unsigned short int* Crc16Table; // Создание динамического массива

public:
    SimpleExchange();
    ~SimpleExchange();
    unsigned short int Crc16(unsigned short int pcBlock[], int len);
    QSerialPort _serialPort;

    const int BUFFER_LEN = 32;
    const int BAUD_RATE = 57600;
};

#endif // SIMPLEEXCHANGE_H
