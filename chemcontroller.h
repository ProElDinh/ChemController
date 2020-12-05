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
    bool isConnected() const; // Команда для определения состояния порта
signals:
    void error_(QString err);//Сигнал ошибок порта


public slots:
    void OpenPort();
    void ClosePort();
    void turnOnTemp(); // Команда включить установку температуры
    void turnOffTemp(); // Команда отключить установку температуры
    void setTemp(double temp); // Команда чтобы задать температуру
    void printHello();

private:
    bool Checkconnect();
    void connectToPort(); //  Команда подключение к порту
    bool _isConnected;  //  Команда проверка подключения устройства
    void commandSetTemp(double temp); //Установка температуры

    // Функция подключения.
    QByteArray writeAndRead(quint8 _SendData[], int len = 1);  // реализация протокола чтения с устройством


    quint16* Crc16Table;
    quint16 Crc16(quint8 pcBlock[], int len);
    QSerialPort *_SerialPort;
    QTimer *_pTimerCheckConnection;



    const quint8 RESP_OK = 0x01; // Все ок.
    const quint8 RESP_ERROR = 0xFF; // Ошибка подключения

    const quint8 CMD_NOP = 0x00; // Команда запрос статуса

    const quint8 CMD_READ_TADC_VAL = 0x50;
    const quint8 CMD_READ_ADCPRESS_VAL = 0x51;

    const quint8 CMD_READ_TEMPER = 0x20;
    const quint8 CMD_TSTAT_ENABLE = 0x25;
    const quint8 CMD_TSTAT_DISABLE = 0x26;
    const quint8 CMD_TSTAT_SET_PWM = 0x27;
    const quint8 CMD_TSTAT_SET_PID = 0x28;
    const quint8 CMD_TSTAT_SET_TEMPER = 0x29;
    const quint8 CMD_SET_TCOEFFS = 0x30;
    const quint8 CMD_TSTAT_GET_PWR = 0x31;
    const quint8 CMD_TSTAT_GET_STATE = 0x32;      //получить состояние термостата (On/Off)
    const quint8 CMD_TSTAT_GET_SUPP_TPR = 0x33;   //получить поддерживаемую температуру


    /*********** Команды Хим. реактора ***********/
    const quint8 CMD_REAC_SET_PID = 0x3A;         //установить коэфф. ПИД
    const quint8 CMD_REAC_SET_PRESS = 0x3B;       //установить давление шприца
    const quint8 CMD_REAC_SET_FREQ = 0x3C;        //установить частоту вращения шаговика
    const quint8 CMD_REAC_SET_STATE = 0x3D;       //установить состояние реактора
    const quint8 CMD_REAC_SET_COEFF = 0x3E;       //установить коэфф. преобразования кода АЦП в давление
    const quint8 CMD_REAC_CALIBR = 0x3F;          //выполнить калибровку системы

    const quint8 CMD_REAC_GET_STATE = 0x40;       //получить состояние реактора
    const quint8 CMD_REAC_GET_CURR_PRESS = 0x41;  //получить текущее давление шприца
    const quint8 CMD_REAC_GET_VOL = 0x42;         //получить текущий объем шприца

    const quint8 CMD_REAC_SYR_SET_MODE = 0x43;    //Установить режим работы шприца
    const quint8 CMD_REAC_SYR_GET_MODE = 0x44;    //Получить режим работы шприца

    const quint8 CMD_REAC_SET_MAX_STEPS = 0x45;   //Установить максимальное значение длины шприца
    const quint8 CMD_REAC_GET_MAX_STEPS = 0x46;   //Получить максимальное значение длины шприца

    const quint8 CMD_REAC_SET_PSC = 0x47;         //Установить значение предделителя таймера шаговика
    const quint8 CMD_REAC_GET_PSC = 0x48;         //Получить значение предделителя таймера шаговика
    const quint8 CMD_REAC_GET_FREQ = 0x49;        //получить код скорости шаговика
    const quint8 CMD_REAC_GET_SUPP_PRESS = 0x4A;  //Получить поддерживаемое давление шприца
    const quint8 CMD_REAC_GET_CALIB_FLAG = 0x4B;  //Получить статус выполнения калибровки

};

#endif  // CHEMCONTROLLER_H
