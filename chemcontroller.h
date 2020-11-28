#ifndef CHEMCONTROLLER_H
#define CHEMCONTROLLER_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QSerialPortInfo>
#include <QDebug>
#include <QThread>
#include <QTimer>

class ChemController : public QObject
{
    Q_OBJECT
public:
    explicit ChemController(QObject *parent = 0);
    ~ChemController();
    bool isConnected() const;
public slots:
    void OpenPort();
    void ClosePort();
    //void turnOn();
    //void turnoff();
private:
    bool Checkconnect();
    void connectToPort();  // Функция подключения.
    QByteArray writeAndRead(quint8 _SendData[]);  // реализация протокола чтения с устройством
    quint16* Crc16Table;
    quint16 Crc16(quint8 pcBlock[], int len);
    QSerialPort *_SerialPort;
    QTimer *_pTimerCheckConnection;
    bool _isConnected;  // Проверка подключения устройства

    quint8 CMD_NOP = 0x00;

};

#endif  // CHEMCONTROLLER_H
