#ifndef CHEMCONTROLLER_H
#define CHEMCONTROLLER_H
#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QSerialPortInfo>
#include <QDebug>
#include <QThread>
#include <QTimer>
#include <exception>
#include <string>
class ChemException: public std::runtime_error{
public:
    ChemException(const std::string error): std::runtime_error(error){


    }

};


class ChemController : public QObject
{
    Q_OBJECT

public:
    explicit ChemController(QObject *parent = 0);
    ~ChemController();
    bool isConnected() const; // Команда для определения состояния порта


public slots:
    void OpenPort();

    void ClosePort();
    void CheckConnect();   // Проверка соединения с устройством
    void TStatEnable(); // Команда включить установку температуры
    void TStatDisable(); // Команда отключить установку температуры
    quint16 GetADCTemper(int id);  // Команда для запроса АЦП термометра
    quint16 GetADCPress(int id);  // Команда для запроса АЦП пресса
    qreal GetTemper(int id);  // Получить температуру данного термометра
    qreal GetSuppTemper();
    void TStatSetTCoeffs(int id, qreal k, qreal b);  // Установить коэффициенты для преобразования кода АЦП в температуру
    int TStatGetPower();  // Команда для запроса мощности устройства
    bool TStatGetStatus();  // Запрос состояния термостата
    void TStatSetPWM(qint16 val);
    void TStatSetPID(qreal kP, qreal kI, qreal kD, qreal A);
    void TStatSetTemper(qreal temper);
    void ReacSetPCoeffs(int ch, qreal k, qreal b);
    bool ReacIsEnable();
    void ReacEnable(bool val);
    void ReacCalibration();
    qreal ReacGetCurrPress(int ch);
    int ReacGetSyrVol(int ch);
    void ReacSetMotorVel(int ch, int val);
    void ReacSyrSetMode(int ch, int mode);
    int ReacSyrGetMode(int ch);
    void ReacSetPID(qreal kP, qreal kI, qreal kD, qreal A);
    void ReacSetPress(int ch, qreal val);
    void ReacSetMaxSteps(int ch, int val);
    int ReacGetMaxSteps(int ch);
    void ReacSetPsc(int ch, int val);
    int ReacGetPsc(int ch);
    int ReacGetMotorVel(int ch);
    qreal ReacGetSuppPress(int ch);
    bool ReacGetCalibrFlag();
private:
    QSerialPort *_SerialPort;

    bool _isConnected = false;  // Команда проверка подключения устройства

    // Функция подключения.
    QByteArray writeAndRead(quint8 _SendData[], int len = 1);  // реализация протокола чтения с устройством
    qint16 BitConvent16(QByteArray data, int startoffset);  // Команда для расшифровки данных из устройства
    qint32 BitConvent32(QByteArray data, int startoffset);
    quint16* Crc16Table;  // Таблица контрольной суммы
    quint16 Crc16(quint8 pcBlock[], int len);  // функция для перевода данных




    const quint8 RESP_OK = 0x01; // Все ок.
    const quint8 RESP_ERROR = 0xFF; // Ошибка подключения

    const quint8 CMD_NOP = 0x00; // Команда запрос статуса

    const quint8 CMD_READ_TADC_VAL = 0x50;  // Получить значение АЦП термометра
    const quint8 CMD_READ_ADCPRESS_VAL = 0x51;  //

    const quint8 CMD_READ_TEMPER = 0x20;  // Прочитать температуру данного термостата
    const quint8 CMD_TSTAT_ENABLE = 0x25;
    const quint8 CMD_TSTAT_DISABLE = 0x26;
    const quint8 CMD_TSTAT_SET_PWM = 0x27;  // Получить мощность
    const quint8 CMD_TSTAT_SET_PID = 0x28;
    const quint8 CMD_TSTAT_SET_TEMPER = 0x29;  // Установить температуту
    const quint8 CMD_SET_TCOEFFS = 0x30;  // // Установить коэффициенты для преобразования кода АЦП в температуру
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
