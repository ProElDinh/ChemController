#include "chemcontroller.h"
#include <QtMath>
// ДОБАВИТЬ ИСКЛЮЧЕНИЯ
ChemController::ChemController(QObject *parent) : QObject(parent)
{
    /*
        Name  : CRC-16
        Poly  : 0x8005    x^16 + x^15 + x^2 + 1
        Init  : 0xFFFF
        Revert: true
        XorOut: 0x0000
        Check : 0x4B37 ("123456789")
        MaxLen: 4095 bytes
    */

    Crc16Table = new quint16 [256]{
            0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
            0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
            0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
            0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
            0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
            0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
            0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
            0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
            0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
            0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
            0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
            0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
            0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
            0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
            0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
            0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
            0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
            0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
            0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
            0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
            0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
            0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
            0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
            0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
            0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
            0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
            0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
            0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
            0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
            0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
            0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
            0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
    };
    _SerialPort = new QSerialPort(this);
}

ChemController::~ChemController(){
    delete[] Crc16Table; // Удаление при завершении
    _SerialPort -> close(); // закрываем соединение при выходе
    delete _SerialPort;
}

quint16 ChemController::Crc16(quint8 pcBlock[], int len) {
    quint16 crc = 0xFFFF;
    for (int i = 0; i < len; i++) {
        crc = (quint16)((crc >> 8) ^ Crc16Table[((crc & 0xFF) ^ pcBlock[i])]);
    }
    return crc;
}


qint16 ChemController::BitConvent16(QByteArray data, int startoffset){
    int first = data[startoffset];
    int second = data[startoffset +1];
    return (qint16) (first | (second << 8));

}

qint32 ChemController::BitConvent32(QByteArray data, int startoffset){
    int first = (data[startoffset]) | (data[startoffset + 1] << 8);
    int second = (data[startoffset + 2]) | (data[startoffset + 3] << 8);
    return (quint32) ((quint16) first | (quint32) (second << 16));
}

void ChemController:: OpenPort(){
    if (_SerialPort->isOpen()){
         _SerialPort ->close();
    }
    _SerialPort -> setPortName("com4"); // указываем параметры порта (далее)
    _SerialPort -> setBaudRate(QSerialPort::Baud57600);
    _SerialPort -> setDataBits(QSerialPort::Data8);
    _SerialPort -> setParity(QSerialPort::NoParity);
    _SerialPort -> setStopBits(QSerialPort :: OneStop);
    _SerialPort -> setFlowControl(QSerialPort:: NoFlowControl);
    _SerialPort -> open(QSerialPort::ReadWrite);

}



void ChemController:: CheckConnect(){  // Запрос статуса и проверка соединения
    QByteArray receivedData = "";
    quint8 data[1];
    data[0] = CMD_NOP;
    quint8 status;
    try{
        qDebug() << "Checkconnect";
        receivedData  = writeAndRead(data,1);
        status = receivedData[0];
    }
    catch (ChemException &err){
        qDebug() << "Ответ не соответствует ожиданию.";
        throw ChemException("Ответ не соответствует ожиданию.");
    }
    if (status == RESP_OK && receivedData.size() == 1){
        _isConnected = true;
    } else {
        _isConnected = false;
        qDebug() << "Ошибка при подключении к устройству!";
        throw ChemException("Ошибка при подключении к устройству!");
    }
}

void ChemController :: ClosePort(){
     qDebug() << "Устройство отключено.";
     _SerialPort -> close();
}

bool ChemController :: isConnected() const{
    return _isConnected;
}


QByteArray ChemController::writeAndRead(quint8 *Data, int len){  // Функция отправка и чтения данных
    QByteArray SentData = "";  // Данные, посылаемые в порт
    quint16 crc = 0;
    crc = Crc16(Data, len);  // высчитывается контрольную сумму CRC16
    for (int i = 0; i< len; i++){
        SentData.append((quint8)(Data[i]));
    }
    SentData.append((quint8)(crc & 0xff));
    SentData.append((quint8)(crc>>8));

    qDebug() << "writeAndRead";
    qDebug() << SentData.toHex().toUpper();

    SentData = SentData.toHex().toUpper();  // Переводим данные в шестанцатеричный код и отправляем его



    _SerialPort->write(":");

    _SerialPort->write(SentData);

    _SerialPort-> write("=");

    _SerialPort->waitForReadyRead(1000);  // ждем 1с, пока устройство обработает данные и ответит

    QString data = _SerialPort->readAll();
    data = data.left(data.indexOf('='));  // читаем данные до символа "="
    if (data.indexOf(":")== (-1)){  // проверка данных
        qDebug() << "Package start not found";
        throw ChemException("Package start not found");
    }
    if (data.length() < 5){
        qDebug() << "Package too short";
        throw ChemException("Package too short");
    }
    if (data.length() % 2 == 0){
        qDebug() << "Incorrect packet length";
        throw ChemException("Incorrect packet length");
    }

    data = data.remove(QChar(':')); // удаляем символ ":"
    //qDebug() << data;

    QByteArray b_data;  // Создаем массив в котором будет храниться переведенная контрольная сумма
    b_data.resize(data.length() / 2);
    for (int i = 0; i < data.length() / 2; i ++ ){
        b_data[i] =(data.mid(i*2, 2)).toInt(nullptr, 16);
    }
    b_data.resize(b_data.length() - 2);
    qDebug() << b_data;


    return b_data;

}

bool ChemController::IsOpen(){
    return _SerialPort->isOpen();

}

// Получить Получить значение АЦП термометра
// <param name="id">Номер термометра, 1 или 2</param>
// <returns>Значение АЦП</returns>

quint16 ChemController::GetADCTemper(int id){
    quint8 data[2];
    data[0] = CMD_READ_TADC_VAL;
    data[1] = (quint8) id;
    QByteArray receivedData = "";
    try {
        qDebug() << "Запрос значения АЦП" +  QString::number(id) +"го Термометра";
        receivedData = writeAndRead(data, 2);
    }  catch (...) {
        qDebug() << "Произошла ошибка при запросе значения АЦП" +  QString::number(id) +"го Термометра";
        throw ChemException("Ошибка. Ошибка подключения.");
    }
    if (receivedData.size() == 5){
        quint16 num = BitConvent16(receivedData, 3);
        return num;
    } else {
        qDebug() << "Ошибка. Ответ не соответствует ожиданиям.";
        throw ChemException("Ошибка. Ответ не соответствует ожиданиям.");
    }
}

quint16 ChemController:: GetADCPress(int id){
    quint8 data[2];
    data[0] = CMD_READ_ADCPRESS_VAL;
    data[1] = (quint8) id;
    QByteArray receivedData = "";
    try {
        qDebug() << "Запрос значения АЦП" +  QString::number(id) +"го Пресса";
        receivedData = writeAndRead(data, 2);
    }  catch (...) {
        qDebug() << "Произошла ошибка при запросе значения АЦП" +  QString::number(id) +"го Пресса";
        throw ChemException("Ошибка. Ошибка подключения.");
    }
    if (receivedData.size() == 5){
        quint16 num = BitConvent16(receivedData,3);
        return num;
    } else {
        qDebug() << "Ошибка. Ответ не соответствует ожиданиям.";
        throw ChemException("Ошибка. Ответ не соответствует ожиданиям.");
    }

}

// <summary>
// Получить температуру данного термометра
// <param name="id">Номер термометра, 1 или 2</param>
// <returns>Температура</returns>


qreal ChemController:: GetTemper(int id){
    quint8 data[2];
    data[0] = CMD_READ_TEMPER;
    data[1] = (quint8) id;
    QByteArray receivedData = "";
    try {
        qDebug() << "Запрос температуры " +  QString::number(id) + " Термометра";
        receivedData = writeAndRead(data, 2);
    }  catch (...) {
        qDebug() << "Произошла ошибка при запросе температуры " +  QString::number(id) + " Термометра" ;
        throw ChemException("Ошибка. Ошибка подключения.");
    }
    if (receivedData.size() == 5){
        qint16 num =BitConvent16(receivedData,3);
        return (qreal) qRound((num *10) / 32.0) / 10.0;
    } else {
        qDebug() << "Ошибка. Ответ не соответствует ожиданиям.";
        throw ChemException("Ошибка. Ответ не соответствует ожиданиям.");
    }
}

qreal ChemController:: GetSuppTemper(){
    quint8 data[1];
    data[0] = CMD_TSTAT_GET_SUPP_TPR;
    QByteArray receivedData = "";
    try {
        qDebug() << "Запрос SuppTemper";
        receivedData = writeAndRead(data, 1);
    }  catch (...) {
        qDebug() << "Произошла ошибка при запросе SuppTemper" ;
        throw ChemException("Ошибка. Ошибка подключения.");
    }
    if (receivedData.size() == 4){
        qint16 num = BitConvent16(receivedData,2);
        return (qreal) qRound((num *10) / 32.0) / 10.0;
    } else {
        qDebug() << "Ошибка. Ответ не соответствует ожиданиям.";
        throw ChemException("Ошибка. Ответ не соответствует ожиданиям.");
    }
}

// Установить коэффициенты для преобразования кода АЦП в температуру (T = k*code + b)
// <param name="id">Номер термометра, 1 или 2</param>
// <param name="k">Коэффициент k</param>
// <param name="b">Коэффициент b</param>


void ChemController::TStatSetTCoeffs(int id, qreal k, qreal b){
    quint8 data[18];
    data[0] = CMD_SET_TCOEFFS;
    data[1] = (quint8) id;
    QByteArray receivedData = "";
    qint64 ck = (qint64) (k *qPow(2,29));
    qint64 cb = (qint64) (b *qPow(2,29));
    for (int i = 0; i < 8; i++)
    {
        data[i + 2] = (quint8)(( ck >> (8 * i)) & 0xFF);
        data[i + 10] = (quint8)(( cb >> (8 * i)) & 0xFF);
    }

    try {
        qDebug() << "Установка коэффициенты для преобразования кода АЦП в температуру в " +   QString::number(id) + "й Термометр";
        receivedData = writeAndRead(data, 18);
    }  catch (...) {
        qDebug() << "Произошла ошибка при установка коэффициенты для преобразования кода АЦП в " +   QString::number(id) + "ом Термометре";
        throw ChemException("Ошибка. Ошибка подключения.");
    }

    if (receivedData.size() != 3){
        qDebug() << "Ошибка. Ответ не соответствует ожиданиям.";
        throw ChemException("Ошибка. Ответ не соответствует ожиданиям.");
    }
}
// Команда для получения мощности устройства

int ChemController:: TStatGetPower(){
    quint8 data[1];
    data[0] = CMD_TSTAT_GET_PWR;
    QByteArray receivedData = "";
    try {
        qDebug() << "Запрос мощности устройства";
        receivedData = writeAndRead(data, 1);
    }  catch (...) {
        qDebug() << "Произошла ошибка при запросе мощности устройства";
        throw ChemException("Ошибка. Ошибка подключения.");
    }
    if (receivedData.size() != 4) {
        qDebug() << "Ошибка. Ответ не соответствует ожиданиям.";
        throw ChemException("Ошибка. Ответ не соответствует ожиданиям.");
    } else {
        return BitConvent16(receivedData,2);
    }


}

bool ChemController:: TStatGetStatus(){
    quint8 data[1];
    QByteArray receiveData = "";
    try {
        qDebug() << "Запрос статуса термостата";
        receiveData = writeAndRead(data, 1);
    }  catch (...) {
        qDebug() << "Произошла ошибка при запросе состояния термостата";
        throw ChemException("Ошибка. Ошибка подключения.");
    }

    if (receiveData.size() != 3){
        qDebug() << "Ошибка. Ответ не соответствует ожиданиям.";
        throw ChemException("Ошибка. Ответ не соответствует ожиданиям.");
    }
    quint8 status = receiveData[2];
    if (status != 0)
        return true;
    else
        return false;

}


void ChemController ::TStatEnable(){
    quint8 data[1];
    data[0] = CMD_TSTAT_ENABLE;
    QByteArray receivedData;
    quint8 status;
    try {
        qDebug() << "Включение термостата";
        receivedData = writeAndRead(data, 1);
        status = receivedData[0];
    }  catch (ChemException &err) {
        qDebug() << "Включение термостата произошло неудачно";
        throw ChemException("Ошибка. Ошибка подключения.");

    }

    if (receivedData.size() == 2 && status == RESP_OK){ //  size = 2
        qDebug() << "Термостат включен";
    }
    else {
        qDebug() << "Ошибка. Ответ не соответствует ожиданиям.";
        throw ChemException("Ошибка. Ответ не соответствует ожиданиям.");
    }
}


void ChemController ::TStatDisable(){
    quint8 data[1];
    data[0] = CMD_TSTAT_DISABLE;
    QByteArray receivedData = "";
    quint8 status;
    try {
        qDebug() << "Отключение термастата";
        receivedData = writeAndRead(data, 1);
        status = receivedData[0];
    }  catch (ChemException &err) {
        qDebug() << "Отключение термостат произошло неудачно";
        throw ChemException("Ошибка. Ошибка подключения.");
    }


   if (receivedData.size() == 2 && status == RESP_OK){ //  size = 2
        qDebug() << "Термостат выключен";
   }
   else {
        qDebug() << "Ошибка. Ответ не соответствует ожиданиям.";
        throw ChemException("Ошибка. Ответ не соответствует ожиданиям.");
        //emit error_( "Ошибка. Ответ не соответствует ожиданиям.");
   }
}

void ChemController::TStatSetPWM(qint16 val){
    quint8 data[3];
    data[0] = CMD_TSTAT_SET_PWM;
    QByteArray receiveData = "";
    for (int i = 0; i < 2; i++)
    {
        data[i + 1] = (quint8)(( val >> (8 * i)) & 0xFF);
    }
    try {
        qDebug() << "Установка значение PWM";
        receiveData = writeAndRead(data, 3);
    }  catch (...) {
        qDebug() << "Произошла ошибка при установке установке значения PWM";
        throw ChemException("Ошибка. Ошибка подключения.");
    }
    if (receiveData.size() != 2){
        qDebug() << "Ошибка. Ответ не соответствует ожиданиям.";
        throw ChemException("Ошибка. Ответ не соответствует ожиданиям.");
    }
}

void ChemController::TStatSetPID(qreal kP, qreal kI, qreal kD, qreal A){
    quint8 data[9];
    QByteArray receiveData = "";
    data[0] = CMD_TSTAT_SET_PID;  // Установить PID
    for (int i = 0; i < 2; i++)
    {
        data[i + 1] = (quint8)(( (quint16) (kP * qPow(2,5)) >> (8 * i) ) & 0xFF);
        data[i + 3] = (quint8)(( (quint16) (kI * qPow(2,5)) >> (8 * i) ) & 0xFF);
        data[i + 5] = (quint8)(( (quint16) (kD * qPow(2,5)) >> (8 * i) ) & 0xFF);
        data[i + 7] = (quint8)(( (quint16) (A * qPow(2,5))  >> (8 * i) ) & 0xFF);
    }
    try {
        qDebug() << "Установка значение PID";
        receiveData = writeAndRead(data, 9);
    }  catch (...) {
        qDebug() << "Произошла ошибка при установке установке значения PID";
        throw ChemException("Ошибка. Ошибка подключения.");
    }
    if (receiveData.size() != 2){
        qDebug() << "Ошибка. Ответ не соответствует ожиданиям.";
        throw ChemException("Ошибка. Ответ не соответствует ожиданиям.");
    }
}

void ChemController::TStatSetTemper(qreal temper){
    quint8 data[3];
    QByteArray receiveData = "";
    data[0] = CMD_TSTAT_SET_TEMPER;
    for (int i = 0; i < 2; i++){
       data[i + 1] = (quint8)(( (qint16) (temper * qPow(2,5)) >> (8 * i) ) & 0xFF);
    }
    try {
        qDebug() << "Установка TStatSetTemper";
        receiveData = writeAndRead(data, 3);
    }  catch (...) {
        qDebug() << "Произошла ошибка при установки TStatTemper";
        throw ChemException("Ошибка. Ошибка подключения.");
    }
    if (receiveData.size()!= 2){
        qDebug() << "Ошибка. Ответ не соответствует ожиданиям.";
        throw ChemException("Ошибка. Ответ не соответствует ожиданиям.");
    }
}

void ChemController::ReacSetPCoeffs(int ch, qreal k, qreal b){
     quint8 data[18];
     QByteArray receiveData = "";
     data[0] = CMD_REAC_SET_COEFF;
     qint64 ck = (qint64) (k * qPow(2,29));
     qint64 cb = (qint64) (b * qPow(2,29));
     data[1] =(quint8) (ch);
     for (int i = 0; i <8; i++){
        data[i + 2] = (quint8)(( ck >> (8 * i)) & 0xFF);
        data[i + 10] = (quint8)(( cb >> (8 * i)) & 0xFF);
     }

     try {
         qDebug() << "Установка ReacPCoeffs";
         receiveData = writeAndRead(data, 18);
     }  catch (...) {
        qDebug() << "Произошла ошибка при установки ReacPCoeffs";
         throw ChemException("Ошибка. Ошибка подключения.");
     }
     if (receiveData.size()!= 3){
         qDebug() << "Ошибка. Ответ не соответствует ожиданиям.";
         throw ChemException("Ошибка. Ответ не соответствует ожиданиям.");
     }


}

bool ChemController::ReacIsEnable(){
    quint8 data[1];
    data[0] = CMD_REAC_GET_STATE;
    QByteArray receiveData = "";
    quint8 status;
    try {
        qDebug() << "Проверка Состояния Реактора";
        receiveData = writeAndRead(data);
        status = receiveData[2];
    }  catch (...) {
        qDebug() << "Произошла ошибка при проверки состояния Реактора";
        throw ChemException("Ошибка. Ошибка подключения.");
    }
    if (receiveData.size()!= 3){
        qDebug() << "Ошибка. Ответ не соответствует ожиданиям.";
        throw ChemException("Ошибка. Ответ не соответствует ожиданиям.");
    }
    return status == 0 ? false : true;
}

void ChemController:: ReacEnable(bool val){
    quint8 data[2];
    QByteArray receiveData = "";
    data[0] = CMD_REAC_SET_STATE;
    data[1] = (quint8)(val ? 1 : 0);
    try {
        qDebug() << "Отправка Состояния Реактора";
        receiveData = writeAndRead(data, 2);
    }  catch (...) {
        qDebug() << "Произошла ошибка при отправки состояния Реактора";
        throw ChemException("Ошибка. Ошибка подключения.");
    }
    if (receiveData.size()!= 2){
        qDebug() << "Ошибка. Ответ не соответствует ожиданиям.";
        throw ChemException("Ошибка. Ответ не соответствует ожиданиям.");
    }

}

void ChemController::ReacCalibration(){
    quint8 data[1];
    QByteArray receiveData = "";
    data[0] = CMD_REAC_CALIBR;
    try {
        qDebug() << "Запуск калибровки реактора";
        receiveData = writeAndRead(data);
    }  catch (...) {
        qDebug() << "Произошла ошибка при запуске Калибровки Реактора";
        throw ChemException("Ошибка. Ошибка подключения.");

    }
    if (receiveData.size()!= 2){
        qDebug() << "Ошибка. Ответ не соответствует ожиданиям.";
        throw ChemException("Ошибка. Ответ не соответствует ожиданиям.");
    }
}

qreal ChemController::ReacGetCurrPress(int ch){
    quint8 data[2];
    QByteArray receiveData = "";
    data[0] = CMD_REAC_GET_CURR_PRESS;
    data[1] = (quint8) ch;
    try {
        qDebug() << "Запрос значения ReacCurrPress";
        receiveData = writeAndRead(data,2);
    }  catch (...) {
        qDebug() << "Произошла ошибка при запросе значения ReacCurrPress";
        throw ChemException("Ошибка. Ошибка подключения.");
    }

    if (receiveData.size() == 5){
        qint16 t = BitConvent16(receiveData, 3);
        return (qreal) (qRound(t * 10 / 32.0)) / 10.0;
    }
    else {
        qDebug() << "Ошибка. Ответ не соответствует ожиданиям.";
        throw ChemException("Ошибка. Ответ не соответствует ожиданиям.");
    }
}

int ChemController::ReacGetSyrVol(int ch){
    quint8 data[2];
    QByteArray receiveData = "";
    data[0] = CMD_REAC_GET_VOL;
    data[1] = (quint8) ch;
    try {
        qDebug() << "Запрос значения REAC VOl";
        receiveData = writeAndRead(data,2);
    }  catch (...) {
        qDebug() << "Произошла ошибка при запросе значения REAC VOl";
        throw ChemException("Ошибка. Ошибка подключения.");
    }
    if (receiveData.size() != 7){
        qDebug() << "Ошибка. Ответ не соответствует ожиданиям.";
        throw ChemException("Ошибка. Ответ не соответствует ожиданиям.");
    }
    qint32 num = BitConvent32(receiveData, 3);
    return num;
}

void ChemController::ReacSetMotorVel(int ch, int val){
    quint8 data[6];
    QByteArray receiveData = "";
    data[0] = CMD_REAC_SET_FREQ;
    data[1] = (quint8) ch;

    for (int i = 0; i < 4; i ++){
        data[i + 2] = (quint8)(( val >> (8 * i)) & 0xFF);
    }

    try {
        qDebug() << "Установка Частоты Реактора";
        receiveData = writeAndRead(data, 6);
    }  catch (...) {
        qDebug() << "Произошла ошибка при Установки Частоты Реактора";
        throw ChemException("Ошибка. Ошибка подключения.");
    }
    if (receiveData.size() != 2){
        qDebug() << "Ошибка. Ответ не соответствует ожиданиям.";
        throw ChemException("Ошибка. Ответ не соответствует ожиданиям.");
    }
}

void ChemController::ReacSyrSetMode(int ch, int mode){
    quint8 data[3];
    QByteArray receiveData = "";
    data[0] = CMD_REAC_SYR_SET_MODE;
    data[1] = (quint8) ch;
    data[2] = (quint8) mode;
    try {
        qDebug() << "Установка Режима Реактора";
        receiveData = writeAndRead(data, 3);
    }  catch (...) {
        qDebug() << "Произошла ошибка при Установки Режима Реактора";
        throw ChemException("Ошибка. Ошибка подключения.");
    }
    if (receiveData.size()!= 4){
        qDebug() << "Ошибка. Ответ не соответствует ожиданиям.";
        throw ChemException("Ошибка. Ответ не соответствует ожиданиям.");
    }
}

int ChemController::ReacSyrGetMode(int ch){
    QByteArray receiveData = "";
    quint8 data[2];
    data[0] = CMD_REAC_SYR_GET_MODE;
    data[1] = (quint8) ch;
    try {
        qDebug() << "Запрос Режима Реактора";
        receiveData = writeAndRead(data, 2);
    }  catch (...) {
        qDebug() << "Произошла ошибка при Запросе Режима Реактора";
        throw ChemException("Ошибка. Ошибка подключения.");
    }
    if (receiveData.size()!= 3){
        qDebug() << "Ошибка. Ответ не соответствует ожиданиям.";
        throw ChemException("Ошибка. Ответ не соответствует ожиданиям.");
    }
    return (quint8) receiveData[2];

}

void ChemController::ReacSetPID(qreal kP, qreal kI, qreal kD, qreal A){
    QByteArray receiveData = "";
    quint8 data[9];
    data[0] = CMD_REAC_SET_PID;
    for (int i = 0; i < 2; i++){
        data[i + 1] = (quint8)(( (quint16) (kP * qPow(2,5)) >> (8 * i) ) & 0xFF);
        data[i + 3] = (quint8)(( (quint16) (kI * qPow(2,5)) >> (8 * i) ) & 0xFF);
        data[i + 5] = (quint8)(( (quint16) (kD * qPow(2,5)) >> (8 * i) ) & 0xFF);
        data[i + 7] = (quint8)(( (quint16) (A * qPow(2,5))  >> (8 * i) ) & 0xFF);
    }
    try {
        qDebug() << "Установка PID";
        receiveData = writeAndRead(data, 9);
    }  catch (...) {
        qDebug() << "Произошла ошибка при Установке PID";
        throw ChemException("Ошибка. Ошибка подключения.");
    }
    if (receiveData.size() != 2){
        qDebug() << "Ошибка. Ответ не соответствует ожиданиям.";
        throw ChemException("Ошибка. Ответ не соответствует ожиданиям.");
    }
}

void ChemController::ReacSetPress(int ch, qreal val){
    QByteArray receiveData = "";
    quint8 data[4];
    data[0] = CMD_REAC_SET_PRESS;
    data[1] = (quint8) ch;
    for (int i = 0; i < 2; i++){
        data[i + 2] = (quint8)(( (quint16) (val * qPow(2,5)) >> (8 * i) ) & 0xFF);
    }
    try {
        qDebug() << "Установка React Press";
        receiveData = writeAndRead(data, 4);
    }  catch (...) {
        qDebug() << "Произошла ошибка при Установке React Press";
        throw ChemException("Ошибка. Ошибка подключения.");
    }
    if (receiveData.size() != 2){
        qDebug() << "Ошибка. Ответ не соответствует ожиданиям.";
        throw ChemException("Ошибка. Ответ не соответствует ожиданиям.");
    }
}

void ChemController::ReacSetMaxSteps(int ch, int val){
    QByteArray receiveData = "";
    quint8 data[6];
    data[0] = CMD_REAC_SET_MAX_STEPS;
    data[1] = (quint8) ch;
    for (int i = 0; i < 4;i ++){
      data[i + 2] = (quint8)(( val >> (8 * i)) & 0xFF);
    }

    try {
        qDebug() << "Установка React MaxSteps";
        receiveData = writeAndRead(data, 6);
    }  catch (...) {
        qDebug() << "Произошла ошибка при Установке React MaxSteps";
        throw ChemException("Ошибка. Ошибка подключения.");
    }
    if (receiveData.size() != 3){
        qDebug() << "Ошибка. Ответ не соответствует ожиданиям.";
        throw ChemException("Ошибка. Ответ не соответствует ожиданиям.");
    }

}

int ChemController::ReacGetMaxSteps(int ch){
    QByteArray receiveData = "";
    quint8 data[2];
    data[0] = CMD_REAC_GET_MAX_STEPS;
    data[1] = (quint8) ch;
    try {
        qDebug() << "Запрос React MaxSteps";
        receiveData = writeAndRead(data, 2);
    }  catch (...) {
        qDebug() << "Произошла ошибка при Запросе React MaxSteps";
        throw ChemException("Ошибка. Ошибка подключения.");
    }
    if (receiveData.size() != 7){
        qDebug() << "Ошибка. Ответ не соответствует ожиданиям.";
        throw ChemException("Ошибка. Ответ не соответствует ожиданиям.");
    }
    return BitConvent32(receiveData, 3);
}

void ChemController::ReacSetPsc(int ch, int val){
    QByteArray receiveData = "";
    quint8 data[4];
    data[0] = CMD_REAC_SET_PSC;
    data[1] = (quint8) ch;
    for (int i = 0; i < 2;i ++){
      data[i + 2] = (quint8)(( (quint16) val >> (8 * i)) & 0xFF);
    }

    try {
        qDebug() << "Установка React PSC";
        receiveData = writeAndRead(data, 4);
    }  catch (...) {
        qDebug() << "Произошла ошибка при Установка React PSC";
        throw ChemException("Ошибка. Ошибка подключения.");
    }
    if (receiveData.size()!= 3){
        qDebug() << "Ошибка. Ответ не соответствует ожиданиям.";
        throw ChemException("Ошибка. Ответ не соответствует ожиданиям.");
    }
}

int ChemController::ReacGetPsc(int ch){
    QByteArray receiveData = "";
    quint8 data[2];
    data[0] = CMD_REAC_GET_PSC;
    data[1] = (quint8) ch;
    try {
        qDebug() << "Запрос React PSC";
        receiveData = writeAndRead(data, 2);
    }  catch (...) {
        qDebug() << "Произошла ошибка при Запросе React PSC";
        throw ChemException("Ошибка. Ошибка подключения.");
    }
    if (receiveData.size()!= 5){
        qDebug() << "Ошибка. Ответ не соответствует ожиданиям.";
        throw ChemException("Ошибка. Ответ не соответствует ожиданиям.");
    }
    return BitConvent16(receiveData, 3);
}

int ChemController::ReacGetMotorVel(int ch){
    QByteArray receiveData = "";
    quint8 data[2];
    data[0] = CMD_REAC_GET_FREQ;
    data[1] = (quint8) ch;
    try {
        qDebug() << "Запрос React MotorVel";
        receiveData = writeAndRead(data, 2);
    }  catch (...) {
        qDebug() << "Произошла ошибка при Запросе React MotorVel";
        throw ChemException("Ошибка. Ошибка подключения.");
    }
    if (receiveData.size() != 7){
        qDebug() << "Ошибка. Ответ не соответствует ожиданиям.";
        throw ChemException("Ошибка. Ответ не соответствует ожиданиям.");
    }
    return BitConvent32(receiveData, 3);
}

qreal ChemController::ReacGetSuppPress(int ch){
    QByteArray receiveData = "";
    quint8 data[2];
    data[0] = CMD_REAC_GET_SUPP_PRESS;
    data[1] = (quint8) ch;
    try {
        qDebug() << "Запрос React SuppPress";
        receiveData = writeAndRead(data, 2);
    }  catch (...) {
        qDebug() << "Произошла ошибка при Запросе React SuppPress";
        throw ChemException("Ошибка. Ошибка подключения.");
    }
    if (receiveData.size() == 5){
        qint16 t = BitConvent16(receiveData, 3);
        return (qreal) qRound(t * 10 / 32.0) / 10.0;
    } else {
        qDebug() << "Ошибка. Ответ не соответствует ожиданиям.";
        throw ChemException("Ошибка. Ответ не соответствует ожиданиям.");
    }
}

bool ChemController::ReacGetCalibrFlag(){
    QByteArray receiveData = "";
    quint8 data[1];
    data[0] = CMD_REAC_GET_CALIB_FLAG;
    try {
        qDebug() << "Запрос React CalibrFlag";
        receiveData = writeAndRead(data);
    }  catch (...) {
        qDebug() << "Произошла ошибка при Запросе React CalibrFlag";
        throw ChemException("Ошибка. Ошибка подключения.");
    }
    if (receiveData.size() != 3){
        qDebug() << "Ошибка. Ответ не соответствует ожиданиям.";
        throw ChemException("Ошибка. Ответ не соответствует ожиданиям.");
    }
    quint8 status = receiveData[2];
    return status == 0 ? false : true;
}

