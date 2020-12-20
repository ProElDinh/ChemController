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

qreal ChemController::BitConvent(QByteArray data, int startoffset){
    quint8 a1 =data[startoffset];
    quint8 a2 = data[startoffset +1];
    qint16 a = (a1) | (a2 << 8);
    return (qreal) qRound((qreal) a *10/ 32) / 10;

}

void ChemController:: OpenPort(){
    _SerialPort = new QSerialPort(this);
    _SerialPort -> setPortName("com4"); // указываем параметры порта (далее)
    _SerialPort -> setBaudRate(QSerialPort::Baud57600);
    _SerialPort -> setDataBits(QSerialPort::Data8);
    _SerialPort -> setParity(QSerialPort::NoParity);
    _SerialPort -> setStopBits(QSerialPort :: OneStop);
    _SerialPort -> setFlowControl(QSerialPort:: NoFlowControl);
        /*  При включенном приборе нужно обязательно, хотя бы раз в секунду,
            посылать команду на запрос статуса, чтобы подтвердить, что соединение не разорвано.
            Иначе прибор будет выключен. Для этого создаем таймер с интервалом в 1 секунду. */
    _pTimerCheckConnection = new QTimer(this);
    _pTimerCheckConnection->setInterval(1000);
        // соединяем чтение - прием данных
        /* По истечении времени 1 с вызывается команда запроса статуса.
         *
        Здесь используется именно лямбда-функция, чтобы не создавать слот. */

    try {
        connect(_pTimerCheckConnection, &QTimer::timeout, [this](){
            try {
                Checkconnect();
            }  catch (...) {
                ClosePort();
                _isConnected = false;
                error_("Потеряна связь");
            };});
        connectToPort();  // Подключаем порт
        _pTimerCheckConnection->start();
    }  catch (...) {
        ClosePort();
        throw ChemException("error connect");
    }

}



bool ChemController:: Checkconnect(){  // Запрос статуса и проверка соединения
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
        //_pTimerCheckConnection->start();
        return true;
    } else {
        qDebug() << "Ошибка при подключении к устройству!";
        throw ChemException("Ошибка при подключении к устройству!");
    }
    return false;
}


void ChemController :: connectToPort(){
    if (_SerialPort->open(QSerialPort::ReadWrite))
    {
        // Убеждаемся, что в последовательный порт подключен именно в нужное устройство.
        try {
             _isConnected = Checkconnect();
             qDebug() << "Проверка соединения";
        }  catch (...) {
            qDebug() << "Нет Соединения";
            throw ChemException("Нет соединения");
        }
        if (_isConnected)
        {
            qDebug() << "Устройство подключено.";
        }
        else
        {
            qDebug() << "В последовательный порт подключено другое устройство";
            throw ChemException("В последовательный порт подключено другое устройство");
        }
    }
    else
    {
        qDebug() << "Последовательный порт не подключен.";
        _isConnected = false;
        throw ChemException("Последовательный порт не подключен.");
    }
}




void ChemController :: ClosePort(){
     //QByteArray receivedData = writeAndRead({});
     qDebug() << "Устройство отключено.";
     _pTimerCheckConnection ->stop();
     _SerialPort -> close();
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



void ChemController ::setTemp(double temp){
    if (isConnected())
        {
            commandSetTemp(temp);
        }
    else{
            qDebug() << "Устройство отключено.";
            //emit error_("\r\r\rУстройство отключено.\r\r\r");
            throw ChemException("Устройство отключено");
    }
}




bool ChemController :: isConnected() const{
    return _isConnected;
}

void ChemController ::commandSetTemp(double temp){ // Команда установить температуту
    quint8 data[3];
    data[0] = CMD_TSTAT_SET_TEMPER;
    data[1] = (quint8) (temp * 32) & 0xff;
    data[2] = ((quint8) temp * 32) >> 8;
    QByteArray receivedData = "";
    try {
        qDebug() << "Установка температуры";
        receivedData = writeAndRead(data, 3);
    }  catch (ChemException &err) {
        qDebug() << "Установка температуры произошло неудачно";
        throw ChemException("Ошибка. Ответ не соответствует ожиданиям.");

    }
    // Дописать обработку исключений
    if (receivedData.size() != 1){ // size = 2
        //emit error_( "Ошибка. Ответ не соответствует ожиданиям.");
        qDebug() << "Ошибка. Ответ не соответствует ожиданиям.";
        throw ChemException("Ошибка. Ответ не соответствует ожиданиям.");

    }
}

// Получить Получить значение АЦП термометра
// <param name="id">Номер термометра, 1 или 2</param>
// <returns>Значение АЦП</returns>

qreal ChemController::GetADCTemper(int id){
    quint8 data[2];
    data[0] = CMD_READ_TADC_VAL;
    data[1] = (quint8) id;
    QByteArray receivedData = "";
    try {
        qDebug() << "Запрос значения АЦП" +  QString::number(id) +"го Термометра";
        receivedData = writeAndRead(data, 2);
    }  catch (...) {
        qDebug() << "Произошла ошибка при запросе значения АЦП" +  QString::number(id) +"го Термометра";
        throw ChemException("Ошибка. Ответ не соответствует ожиданиям.");
    }
    if (receivedData.size() == 5){
        return BitConvent(receivedData, 3);
    } else {
        qDebug() << "Ошибка. Ответ не соответствует ожиданиям.";
        throw ChemException("Ошибка. Ответ не соответствует ожиданиям.");
    }
}

qreal ChemController:: GetADCPress(int id){
    quint8 data[2];
    data[0] = CMD_READ_ADCPRESS_VAL;
    data[1] = (quint8) id;
    QByteArray receivedData = "";
    try {
        qDebug() << "Запрос значения АЦП" +  QString::number(id) +"го Пресса";
        receivedData = writeAndRead(data, 2);
    }  catch (...) {
        qDebug() << "Произошла ошибка при запросе значения АЦП" +  QString::number(id) +"го Пресса";
        throw ChemException("Ошибка. Ответ не соответствует ожиданиям.");
    }
    if (receivedData.size() == 5){
        return BitConvent(receivedData, 3);
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
        throw ChemException("Ошибка. Ответ не соответствует ожиданиям.");
    }
    if (receivedData.size() == 5){
        return BitConvent(receivedData, 3);
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
        throw ChemException("Ошибка. Ответ не соответствует ожиданиям.");
    }
    if (receivedData.size() == 4){
        return BitConvent(receivedData, 2);
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
        throw ChemException("Ошибка. Ответ не соответствует ожиданиям.");
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
        throw ChemException("Ошибка. Ответ не соответствует ожиданиям.");
    }
    if (receivedData.size() != 4) {
        qDebug() << "Ошибка. Ответ не соответствует ожиданиям.";
        throw ChemException("Ошибка. Ответ не соответствует ожиданиям.");
    }
    return BitConvent(receivedData, 2);
}

bool ChemController:: TStatGetStatus(){
    quint8 data[1];
    QByteArray receiveData = "";
    try {
        qDebug() << "Запрос статуса термостата";
        receiveData = writeAndRead(data, 1);
    }  catch (...) {
        qDebug() << "Произошла ошибка при запросе состояния термостата";
        throw ChemException("Ошибка. Ответ не соответствует ожиданиям.");
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


void ChemController ::turnOnTemp(){
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
        throw ChemException("Ошибка. Ответ не соответствует ожиданиям.");

    }

    if (receivedData.size() == 2 && status == RESP_OK){ //  size = 2
        qDebug() << "Термостат включен";
    }
    else {
        qDebug() << "Ошибка. Ответ не соответствует ожиданиям.";
        throw ChemException("Ошибка. Ответ не соответствует ожиданиям.");
    }
}


void ChemController ::turnOffTemp(){
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
        throw ChemException("Ошибка. Ответ не соответствует ожиданиям.");
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
        throw ChemException("Ошибка. Ответ не соответствует ожиданиям.");
    }
    if (receiveData.size() != 2){
        qDebug() << "Ошибка. Ответ не соответствует ожиданиям.";
        throw ChemException("Ошибка. Ответ не соответствует ожиданиям.");
    }
}



void ChemController::OpenPython(){
    system("E:/ChemControllerGUI/test.py");
}
