#ifndef CHEMCONTROLLER_H
#define CHEMCONTROLLER_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
class ChemController
{
    Q_OBJECT
public:
    ChemController();
    ~ChemController();
private:
    quint16* Crc16Table;
    quint16 Crc16(quint16 pcBlock[], int len);
};

#endif // CHEMCONTROLLER_H
