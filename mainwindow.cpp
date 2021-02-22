#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    _chemcontroller = new ChemController;
    _sysconfig = new SysConfig;
    _devsettings = new DevSettings;

    _reacSettings = new FormReacSettings(_sysconfig, _devsettings);
    _tStatConfig = new FormTStatConfig(_sysconfig, _devsettings);

    lblStatus = new QLabel(this);
    StatusTStat = new QLabel(this);
    StatusReac = new QLabel(this);
    statusBar() -> addWidget(lblStatus);
    statusBar() -> addWidget(StatusTStat);
    statusBar() -> addWidget(StatusReac);
    //проверка соединения

    PSC_CHx = new int [4];
    for (int i = 0; i < 4; i++){
        PSC_CHx[i] = 1;
    }

    LoadConfig();  // Загрузить данные из конфига

    if (!_chemcontroller->isConnected()){
        StatusDisconnected();
    }

    // Привязка кнопок меню к функциями

    connect(ui->MenuConnect, &QAction::triggered, this, &MainWindow:: Connect); // Подключить устройство
    connect(ui->MenuDisconnect, &QAction::triggered, this, &MainWindow:: Disconnect);  // Отключить устройство


    connect(ui->buttonTStatOn, &QPushButton::clicked, this, &MainWindow::TurnOnTemp);
    connect(ui->buttonTStatOff, &QPushButton::clicked, this, &MainWindow::TurnOffTemp);


    connect(ui->MenuReacCalibr,  &QAction::triggered, this, &MainWindow::ReacRecalibration);
    connect(ui->MenuReacStart, &QAction::triggered, this, &MainWindow::ReacStart);
    connect(ui->MenuReacStop, &QAction::triggered, this, &MainWindow::ReacStop);


    connect(ui->MenuReacSet,&QAction::triggered, this, &MainWindow::ReacSettings);
    connect(ui->MenuTStatConfig,&QAction::triggered, this, &MainWindow::TStatConfig);

    connect(_reacSettings,&FormReacSettings::SaveConfig,this, &MainWindow::SafeConfig);
    connect(_tStatConfig,&FormTStatConfig::SaveConfig,this, &MainWindow::SafeConfig);
}




MainWindow::~MainWindow()
{
    delete [] PSC_CHx;
    delete lblStatus;
    delete StatusTStat;
    delete StatusReac;
    delete _chemcontroller;
    delete _reacSettings;
    delete _tStatConfig;
    delete ui;

}

void MainWindow::ErrorMessage(QString err){
    QMessageBox::critical(this,
                        "Ошибка подключения", err, QMessageBox::Ok);
}

void MainWindow::ReacSettings(){
    _reacSettings -> LoadConfig();
    _reacSettings -> setWindowTitle("Настройка реактора");
    _reacSettings -> show();

}

void MainWindow::TStatConfig(){
    _tStatConfig -> LoadConfig();
    _tStatConfig -> setWindowTitle("Настройка термостата");
    _tStatConfig -> show();

}

void MainWindow::TurnOnTemp(){
    try {
        _chemcontroller->TStatEnable();
        StatusTStatEnable();
    }  catch (...) {
        ErrorMessage("Ошибка при запуске термостата!");
    }

}

void MainWindow::TurnOffTemp(){
    try {
        _chemcontroller-> TStatDisable();
        StatusDisconnected();
    }  catch (...) {
        ErrorMessage("Ошибка при отключении термостата!");
    }

}
/***********************************************************************************/

//Displays

void MainWindow::ChXModeAuto(QComboBox *cmbRange, QSpinBox *VelCode, QDoubleSpinBox *Set){
    cmbRange -> setEnabled(false);
    VelCode -> setEnabled(false);
    Set -> setEnabled(true);
}


void MainWindow::ChXModeManual(QComboBox *cmbRange, QSpinBox *VelCode, QDoubleSpinBox *Set){
    cmbRange -> setEnabled(true);
    VelCode -> setEnabled(true);
    Set -> setEnabled(false);
}


void MainWindow::PressDisp(int ch, QLabel* label){
    label -> setText(QString::number(_chemcontroller -> ReacGetCurrPress(ch)) + " кПа") ;

}

void MainWindow::TemperDisplay(int ch, QLabel *label){
    qreal T = _chemcontroller -> GetTemper(ch);
    label -> setText(QString::number(qRound(T * 10.0) / 10.0) +  " °C");

}

void MainWindow::PowerDisp(QLabel *label){
    qreal power = _chemcontroller -> TStatGetPower();
    power = qRound((power * 1000.0 / 255.0)) / 10.0;
    label -> setText(QString::number(power) + "%");

}

/***********************************************************************************/


// Statuses
void MainWindow::StatusConnected(){
        ui -> MenuConnect->setEnabled(false);
        ui -> MenuDisconnect->setEnabled(true);
        ui -> MenuSettings -> setEnabled(false);

        ui -> MenuReacStart -> setEnabled(true);
        ui -> MenuReacStop -> setEnabled(false);
        ui -> MenuReacCalibr -> setEnabled(true);
        ui -> MenuOpenScript -> setEnabled(true);


        ui -> groupTStat -> setEnabled(true);
        ui -> groupCH1 -> setEnabled(true);
        ui -> groupCH2 -> setEnabled(true);
        ui -> groupCH3 -> setEnabled(true);
        ui -> groupCH4 -> setEnabled(true);
        lblStatus->setText("Подключено\t");

}

void MainWindow::StatusDisconnected(){
        ui -> MenuConnect ->setEnabled(true);
        ui -> MenuDisconnect ->setEnabled(false);
        ui -> MenuSettings -> setEnabled(true);

        ui -> MenuReacStart -> setEnabled(false);
        ui -> MenuReacStop -> setEnabled(false);
        ui -> MenuReacCalibr -> setEnabled(false);
        ui -> MenuOpenScript -> setEnabled(false);

        ui -> MenuReacSet -> setEnabled(true);
        ui -> MenuTStatConfig -> setEnabled(true);

        ui -> groupTStat -> setEnabled(false);
        ui -> groupCH1 -> setEnabled(false);
        ui -> groupCH2 -> setEnabled(false);
        ui -> groupCH3 -> setEnabled(false);
        ui -> groupCH4 -> setEnabled(false);


        lblStatus->setText("Отключено\t");
        StatusTStat->setText("Термостат: ---\t");
        StatusReac->setText("Реактор: ---");


}

void MainWindow::StatusReacEnable(){
        ui -> MenuReacStart -> setEnabled(false);
        ui -> MenuReacStop -> setEnabled(true);
        ui -> MenuReacCalibr -> setEnabled(false);
        ui -> MenuReacSet -> setEnabled(false);

        StatusReac->setText("Реактор: ON");
}

void MainWindow::StatusReacDisable(){
        ui -> MenuReacStart -> setEnabled(true);
        ui -> MenuReacStop -> setEnabled(false);
        ui -> MenuReacCalibr -> setEnabled(true);
        ui -> MenuReacSet -> setEnabled(true);

        StatusReac->setText("Реактор: OFF");
}

void MainWindow::StatusTStatEnable(){
        ui -> buttonTStatOn -> setEnabled(false);
        ui -> buttonTStatOff -> setEnabled(true);
        ui -> set_tempbox -> setEnabled(true);

        StatusTStat -> setText("Термостат: ON");
}



void MainWindow::StatusTStatDisable(){
        ui -> buttonTStatOn -> setEnabled(true);
        ui -> buttonTStatOff -> setEnabled(false);
        ui -> set_tempbox -> setEnabled(false);

        StatusTStat -> setText("Термостат: OFF");

}


/***********************************************************************************/

//Functions for buttons

void MainWindow:: Connect(){


    _TimerRefreshInfo = new QTimer(this);  // Создаем таймер
    _TimerRefreshInfo->setInterval(1000);

    /* По истечении времени 1 с вызывается команда запроса статуса.
             */
    try {
        _chemcontroller -> OpenPort();
        //_chemcontroller -> CheckConnect();
        StatusConnected();
        UploadConfig();
        connect(_TimerRefreshInfo, &QTimer::timeout, this, &MainWindow::Refresh);
        _TimerRefreshInfo->start();  // запускаем таймер
    }  catch (...) {
        if (_chemcontroller -> IsOpen()){
            _chemcontroller -> ClosePort();
        }
        _TimerRefreshInfo->stop();
        ErrorMessage("Ошибка подключения");
        StatusDisconnected();

    }


}

void MainWindow:: Disconnect(){

    _chemcontroller-> ClosePort();
    StatusDisconnected();
    _TimerRefreshInfo->stop();
}

/***********************************************************************************/

//Reac buttons

void MainWindow::ReacRecalibration(){
    try {
        if (_chemcontroller ->IsOpen()){
            _chemcontroller ->ReacEnable(false);
            UploadReacConfig();
            //выполняем перекалибровку
            _chemcontroller ->ReacCalibration();
        }

    }  catch (...) {
        ErrorMessage("Ошибка при выполнении перекалибровки!");
    }

}

void MainWindow::ReacStart(){
    try {
        _chemcontroller->ReacEnable(true);
        StatusReacEnable();
    }  catch (...) {
        ErrorMessage("Ошибка выполнения операции!");
    }

}

void MainWindow::ReacStop(){
    try {
        _chemcontroller->ReacEnable(false);
        StatusReacDisable();
    }  catch (...) {
        ErrorMessage("Ошибка выполнения операции!");
    }

}

/***********************************************************************************/

void MainWindow::SettingsDefault(){
    _devsettings -> COM_PORT = "";
    _devsettings -> Reac_Kp = 30;
    _devsettings -> Reac_Ki = 0;
    _devsettings -> Reac_Kd = 0;
    _devsettings -> Reac_A = 20;

    _devsettings -> TStat_Kp = 10;
    _devsettings -> TStat_Ki = 0.09;
    _devsettings -> TStat_Kd = 0;
    _devsettings -> TStat_A = 10;

}

void MainWindow::ConfigDefault(){
    _sysconfig -> TStat_K1 = 0.01326435;
    _sysconfig -> TStat_B1 = -698.647035;

    _sysconfig -> TStat_K2 = 0.01326435;
    _sysconfig -> TStat_B2 = -698.647035;

    _sysconfig -> ReacCH1_Len = 3000;
    _sysconfig -> ReacCH1_MicroStep = 1;
    _sysconfig -> ReacCH1_Psc1 = 26;
    _sysconfig -> ReacCH1_Psc2 = 1000;
    _sysconfig -> ReacCH1_Psc3 = 32767;
    _sysconfig -> ReacCH1_Vol = 10000;
    _sysconfig -> ReacCH1_X = 0.00966538;
    _sysconfig -> ReacCH1_Y = -30.43621681;

    _sysconfig ->ReacCH2_Len = 3000;
    _sysconfig ->ReacCH2_MicroStep = 1;
    _sysconfig ->ReacCH2_Psc1 = 26;
    _sysconfig ->ReacCH2_Psc2 = 1000;
    _sysconfig ->ReacCH2_Psc3 = 32767;
    _sysconfig ->ReacCH2_Vol = 10000;
    _sysconfig ->ReacCH2_X = 0.00966538;
    _sysconfig ->ReacCH2_Y = -30.43621681;

    _sysconfig ->ReacCH3_Len = 3000;
    _sysconfig ->ReacCH3_MicroStep = 1;
    _sysconfig ->ReacCH3_Psc1 = 26;
    _sysconfig ->ReacCH3_Psc2 = 1000;
    _sysconfig ->ReacCH3_Psc3 = 32767;
    _sysconfig ->ReacCH3_Vol = 10000;
    _sysconfig ->ReacCH3_X = 0.00966538;
    _sysconfig ->ReacCH3_Y = -30.43621681;

    _sysconfig ->ReacCH4_Len = 3000;
    _sysconfig ->ReacCH4_MicroStep = 1;
    _sysconfig ->ReacCH4_Psc1 = 26;
    _sysconfig ->ReacCH4_Psc2 = 1000;
    _sysconfig ->ReacCH4_Psc3 = 32767;
    _sysconfig ->ReacCH4_Vol = 10000;
    _sysconfig ->ReacCH4_X = 0.00966538;
    _sysconfig ->ReacCH4_Y = -30.43621681;

}

void MainWindow::SafeConfig(){
    try {
        _devsettings -> Save(DevSettingsFile);
    }
    catch(...) {
        ErrorMessage("Ошибка сохранения настроек!\r\nВозможно, файл " + DevSettingsFile + " защищен от записи,\r\n либо папка назначения не сущестует!");
    }

    try {
        _sysconfig -> Save(DevConfigFile);
    }
    catch(...) {
        ErrorMessage("Ошибка сохранения конфигурации!\r\nВозможно, файл " + DevConfigFile + " защищен от записи,\r\n либо папка назначения не сущестует!");
    }

    try {
        if (_chemcontroller -> IsOpen())
        {
            UploadTStatCoeff();
            UploadTStatPID();
        }
    }
    catch(...) {
        ErrorMessage("Ошибка загрузки конфигурации в устройство!");
    }

}

void MainWindow::LoadConfig(){
    SettingsDefault();
    ConfigDefault();
    try {
        _devsettings->Load(DevSettingsFile);
    }  catch (...) {
        SettingsDefault();
        // закончить загрузку файла
    }

    try {
        _sysconfig -> Load(DevConfigFile);
    }  catch (...) {
        ConfigDefault();
        // закончить загрузку файла
    }

}

void MainWindow::UploadReacConfig(){
    //Коэффициенты преобразования давления
    _chemcontroller -> ReacSetPCoeffs(1, _sysconfig -> ReacCH1_X, _sysconfig -> ReacCH1_Y);
    _chemcontroller -> ReacSetPCoeffs(2, _sysconfig -> ReacCH2_X, _sysconfig -> ReacCH2_Y);
    _chemcontroller -> ReacSetPCoeffs(3, _sysconfig -> ReacCH3_X, _sysconfig -> ReacCH3_Y);
    _chemcontroller -> ReacSetPCoeffs(4, _sysconfig -> ReacCH4_X, _sysconfig -> ReacCH4_Y);


    //Максимальное количество шагов мотора
    _chemcontroller -> ReacSetMaxSteps(1, _sysconfig -> ReacCH1_Len * _sysconfig -> ReacCH1_MicroStep);
    _chemcontroller -> ReacSetMaxSteps(2, _sysconfig -> ReacCH2_Len * _sysconfig -> ReacCH2_MicroStep);
    _chemcontroller -> ReacSetMaxSteps(3, _sysconfig -> ReacCH3_Len * _sysconfig -> ReacCH3_MicroStep);
    _chemcontroller -> ReacSetMaxSteps(4, _sysconfig -> ReacCH4_Len * _sysconfig -> ReacCH4_MicroStep);
}

void MainWindow::UploadTStatPID(){

    //Устанавливаем коэфф. PID термостата
    _chemcontroller -> TStatSetPID(_devsettings -> TStat_Kp,
                                   _devsettings -> TStat_Ki,
                                   _devsettings -> TStat_Kd,
                                   _devsettings -> TStat_A);
}

void MainWindow::UploadTStatCoeff(){
    //Загружаем коэфф. термостата
    _chemcontroller -> TStatSetTCoeffs(1, _sysconfig -> TStat_K1, _sysconfig -> TStat_B1);
    _chemcontroller -> TStatSetTCoeffs(2, _sysconfig -> TStat_K2, _sysconfig -> TStat_B2);
}

void MainWindow:: UploadConfig(){
    try {
        // Состояние термостата
        bool TStatIsEnable = _chemcontroller -> TStatGetStatus();

        if (TStatIsEnable){  // Уcтанавливаем значение термостата
            ui -> set_tempbox ->setValue((qreal) (_chemcontroller-> GetSuppTemper()));
            StatusTStatEnable();
        } else{
            StatusTStatDisable();
            UploadTStatCoeff();
            UploadTStatPID();

            //Если термостат не запущен
            //то устанавливаем температуру по-умолчанию
            ui -> set_tempbox -> setValue(30);
            _chemcontroller -> TStatSetTemper(30);
        }

        //Реактор
        ReacDispMode(1, ui -> ModeCH1);
        ReacDispMode(2, ui -> ModeCH2);
        ReacDispMode(3, ui -> ModeCH3);
        ReacDispMode(4, ui -> ModeCH4);

        ReacDispRange(1, ui -> Range1,
           _sysconfig -> ReacCH1_Psc1,
           _sysconfig -> ReacCH1_Psc2,
           _sysconfig -> ReacCH1_Psc3);

        ReacDispRange(2, ui -> Range2,
           _sysconfig -> ReacCH2_Psc1,
           _sysconfig -> ReacCH2_Psc2,
           _sysconfig -> ReacCH2_Psc3);

        ReacDispRange(3, ui -> Range3,
           _sysconfig -> ReacCH3_Psc1,
           _sysconfig -> ReacCH3_Psc2,
           _sysconfig -> ReacCH3_Psc3);

        ReacDispRange(4, ui -> Range4,
           _sysconfig -> ReacCH4_Psc1,
           _sysconfig -> ReacCH4_Psc2,
           _sysconfig -> ReacCH4_Psc3);

        ReacDispVelCode(1, ui -> SpeedCode1);
        ReacDispVelCode(2, ui -> SpeedCode2);
        ReacDispVelCode(3, ui -> SpeedCode3);
        ReacDispVelCode(4, ui -> SpeedCode4);

        ReacDispPress(1, ui -> Pressure1);
        ReacDispPress(2, ui -> Pressure2);
        ReacDispPress(3, ui -> Pressure3);
        ReacDispPress(4, ui -> Pressure4);


        //Включен ли реактор?
        bool ReacIsEnable = _chemcontroller -> ReacIsEnable();

        if (ReacIsEnable){
            StatusReacEnable();
        } else {
            StatusReacDisable();

            ReacSetPID();
            UploadReacConfig();

            //выполняем перекалибровку
            _chemcontroller -> ReacCalibration();
        }

    }  catch (...) {
        ErrorMessage("Ошибка загрузки текущих констант системы!");
    }

}






void MainWindow::ReacDispMode(int ch, QComboBox *cmbMode){
    int mode = _chemcontroller->ReacSyrGetMode(ch);
    cmbMode->setCurrentIndex(mode);
}


void MainWindow::ReacSetSyrMode(int ch, int mode){
    try {
        if (_chemcontroller->IsOpen()){
            _chemcontroller->ReacSyrSetMode(ch, mode);
        }
    }  catch (...) {
            ErrorMessage("Ошибка выполнения операции!");
    }
}

void MainWindow::ReacSetRange(int ch, int Psc){
    try {
        if (_chemcontroller->IsOpen()){
            _chemcontroller->ReacSetPsc(ch, Psc);
            PSC_CHx[ch-1] = Psc;
        }
    }  catch (...) {
            ErrorMessage("Ошибка выполнения операции!");
    }

}

void MainWindow::ReacSetPress_(int ch, qreal val){
    try {
        if (_chemcontroller->IsOpen()){
            _chemcontroller->ReacSetPress(ch, val);
        }
    }  catch (...) {
            ErrorMessage("Ошибка выполнения операции!");
    }
}

void MainWindow::ReacSetVelCode(int ch, int val){
    try {
        if (_chemcontroller -> IsOpen()){
            _chemcontroller -> ReacSetMotorVel(ch, val);
        }
    }  catch (...) {
             ErrorMessage("Ошибка выполнения операции!");
    }
}

void MainWindow::ReacDispRange(int ch, QComboBox *cmbRange, int R1, int R2, int R3){
    int psc = _chemcontroller -> ReacGetPsc(ch);
    if (psc == R1){
        cmbRange->setCurrentIndex(0);
    } else if(psc == R2) {
        cmbRange->setCurrentIndex(1);
    } else if(psc == R3) {
        cmbRange->setCurrentIndex(2);
    } else {
        cmbRange->setCurrentIndex(-1);
    }
    PSC_CHx[ch - 1] = psc;
}

void MainWindow::ReacDispVelCode(int ch, QSpinBox *numVelCode){
    int vel = _chemcontroller -> ReacGetMotorVel(ch);
    numVelCode ->setValue(vel);
}

void MainWindow::ReacDispPress(int ch, QDoubleSpinBox *numPress){
    qreal press = _chemcontroller -> ReacGetSuppPress(ch);
    numPress -> setValue((qreal) press);
}

void MainWindow::ReacSetPID(){
    _chemcontroller->ReacSetPID((qreal)(_devsettings ->Reac_Kp),
                                (qreal)(_devsettings ->Reac_Ki),
                                (qreal)(_devsettings ->Reac_Kd),
                                (qreal)(_devsettings ->Reac_A));
}

qreal MainWindow::SyrCode2Vol(int c, int ReacCHx_Len, qreal ReacCHx_Vol, int ReacCHx_MicroStep){
    qreal k = (qreal) ReacCHx_Vol / (ReacCHx_Len * ReacCHx_MicroStep);
    return k * c;

}

qreal MainWindow::SyrVelocity(int VelCode, int Psc, int ReacCHx_Len, double ReacCHx_Vol, int ReacCHx_MicroStep, int RefFreq)
        {
            qreal k = (qreal) ReacCHx_Vol / ReacCHx_Len;
            qreal Div = (65535.0 / VelCode);
            qreal Freq = (qreal) RefFreq / (Psc * Div * 2);
            qreal D = (qreal) (k * Freq) / ReacCHx_MicroStep;
            return D;
        }

void MainWindow::SyrRangeDisp(QLabel *label, QComboBox *combo, int Psc, int ReacCHx_Len, qreal ReacCHx_Vol, int ReacCHx_MicroStep, int RefFreq){
    if (combo->currentIndex() == 0 || combo->currentIndex() == 1){  // ручной режим
        qreal val1 = SyrVelocity(1, Psc, ReacCHx_Len, ReacCHx_Vol, ReacCHx_MicroStep, RefFreq);
        qreal val2 = SyrVelocity(255, Psc, ReacCHx_Len, ReacCHx_Vol, ReacCHx_MicroStep, RefFreq);
        QString S = " мкЛ/с";

        if (qFabs(val2) < 1 && qFabs(val2) > 0.001){
                val1 *= 1000;
                val2 *= 1000;
                S = " нЛ/с";
            } else if (qFabs(val2) <= 0.001){
                val1 *= 1000000;
                val2 *= 1000000;
                S = " пЛ/с";
            }
        label ->setText(QString::number(qRound(val1 * 10.0) / 10.0) + "..." + QString::number(qRound(val2 * 10.0) / 10.0)  + S);
    } else {  // автоматический режим
        label -> setText("---");
    }
}

void MainWindow::SyrVelocityDisp(QLabel *label, QComboBox *combo, int VelCode, int Psc, int ReacCHx_Len, qreal ReacCHx_Vol, int ReacCHx_MicroStep, int RefFreq){
    if (combo->currentIndex() == 0 || combo->currentIndex() == 1){  // ручной режим
        qreal val = SyrVelocity(VelCode, Psc, ReacCHx_Len, ReacCHx_Vol, ReacCHx_MicroStep, RefFreq);
        QString S = " мкЛ/с";
        if (qFabs(val) < 1 && qFabs(val) > 0.001) {
            val *= 1000;
            S = " нЛ/с";
        } else if (qFabs(val) <= 0.001) {
            val *= 1000000;
            S = " пЛ/с";
        }

        if (qFabs(val) > 100){
            label -> setText(QString::number(qRound(val * 100.0) / 100.0) + S);
        } else {
            label -> setText(QString::number(qRound(val * 1000.0) / 1000.0) + S);
        }

    } else {  // автоматический режим
        label -> setText("---");
    }
}

void MainWindow::SyrVolDisp(int ch, QLabel *label, int ReacCHx_Len, qreal ReacCHx_Vol, int ReacCHx_MicroStep){
    int code;
    code = _chemcontroller -> ReacGetSyrVol(ch);
    qreal val = SyrCode2Vol(code, ReacCHx_Len, ReacCHx_Vol, ReacCHx_MicroStep);
    label -> setText(QString::number(qRound(val* 100) / 100.0) + " мкЛ");
}

void MainWindow::TStatSetTemper(double val){
    try {
        _chemcontroller -> TStatSetTemper(val);
    }  catch (...) {
        ErrorMessage("Ошибка выполнения операции!");
    }
}

void MainWindow::Refresh(){
    try {
        //Термостат
        TemperDisplay(1, ui-> t1Label);
        TemperDisplay(2, ui-> t2Label);
        PowerDisp(ui -> powerLabel);

        //Реактор
        //Отображение текущего давления
        PressDisp(1, ui -> press1Label);
        PressDisp(2, ui -> press2Label);
        PressDisp(3, ui -> press3Label);
        PressDisp(4, ui -> press4Label);

        //Отображение текущего объема шприца
        SyrVolDisp(1, ui -> volume1Label, _sysconfig -> ReacCH1_Len, _sysconfig -> ReacCH1_Vol, _sysconfig -> ReacCH1_MicroStep);
        SyrVolDisp(2, ui -> volume2Label, _sysconfig -> ReacCH2_Len, _sysconfig -> ReacCH2_Vol, _sysconfig -> ReacCH2_MicroStep);
        SyrVolDisp(3, ui -> volume3Label, _sysconfig -> ReacCH3_Len, _sysconfig -> ReacCH3_Vol, _sysconfig -> ReacCH3_MicroStep);
        SyrVolDisp(4, ui -> volume4Label, _sysconfig -> ReacCH4_Len, _sysconfig -> ReacCH4_Vol, _sysconfig -> ReacCH4_MicroStep);

        //Отображение текущей скрости шприца (мкЛ/сек) [для ручных режимов]
        SyrVelocityDisp(ui -> speed1Label, ui -> ModeCH1, ui -> SpeedCode1 -> value(), PSC_CHx[0], _sysconfig -> ReacCH1_Len, _sysconfig -> ReacCH1_Vol, _sysconfig -> ReacCH1_MicroStep, 84000000);
        SyrVelocityDisp(ui -> speed2Label, ui -> ModeCH2, ui -> SpeedCode2 -> value(), PSC_CHx[1], _sysconfig -> ReacCH2_Len, _sysconfig -> ReacCH2_Vol, _sysconfig -> ReacCH2_MicroStep, 84000000);
        SyrVelocityDisp(ui -> speed3Label, ui -> ModeCH3, ui -> SpeedCode3 -> value(), PSC_CHx[2], _sysconfig -> ReacCH3_Len, _sysconfig -> ReacCH3_Vol, _sysconfig -> ReacCH3_MicroStep, 84000000);
        SyrVelocityDisp(ui -> speed4Label, ui -> ModeCH4, ui -> SpeedCode4 -> value(), PSC_CHx[3], _sysconfig -> ReacCH4_Len, _sysconfig -> ReacCH4_Vol, _sysconfig -> ReacCH4_MicroStep, 84000000);


        //Отображение диапазона скоростей [для ручных режимов]
        SyrRangeDisp(ui -> range1Label, ui -> ModeCH1, PSC_CHx[0], _sysconfig -> ReacCH1_Len, _sysconfig -> ReacCH1_Vol, _sysconfig -> ReacCH1_MicroStep, 84000000);
        SyrRangeDisp(ui -> range2Label, ui -> ModeCH2, PSC_CHx[1], _sysconfig -> ReacCH2_Len, _sysconfig -> ReacCH2_Vol, _sysconfig -> ReacCH2_MicroStep, 84000000);
        SyrRangeDisp(ui -> range3Label, ui -> ModeCH3, PSC_CHx[2], _sysconfig -> ReacCH3_Len, _sysconfig -> ReacCH3_Vol, _sysconfig -> ReacCH3_MicroStep, 84000000);
        SyrRangeDisp(ui -> range4Label, ui -> ModeCH4, PSC_CHx[3], _sysconfig -> ReacCH4_Len, _sysconfig -> ReacCH4_Vol, _sysconfig -> ReacCH4_MicroStep, 84000000);

        if (_chemcontroller -> ReacGetCalibrFlag()){
            lblStatus -> setText("Калибровка");
        } else {
            lblStatus -> setText("Подключено");
        }

    }  catch (...) {
        _TimerRefreshInfo->stop();
        ErrorMessage("Ошибка чтения статуса устройства!");
    }
}
/***********************************************************************************/

// Menu and buttons

void MainWindow::on_set_tempbox_valueChanged(double arg1)
{
    TStatSetTemper(arg1);
}


void MainWindow::on_ModeCH1_currentIndexChanged(int index)
{
    if(index == 0 || index == 1) { //Ручной или Ручной без возврата
        ChXModeManual(ui -> Range1,
                     ui -> SpeedCode1,
                     ui -> Pressure1);
    } else if(index == 2) {
        ChXModeAuto(ui -> Range1,
                    ui -> SpeedCode1,
                    ui -> Pressure1);

        ReacSetRange(1, _sysconfig -> ReacCH1_Psc1);
     }

     ReacSetSyrMode(1, index);
}



void MainWindow::on_Range1_currentIndexChanged(int index)
{
    int div;

    if (index == 0)
        div = _sysconfig -> ReacCH1_Psc1;
    else if (index == 1)
        div = _sysconfig -> ReacCH1_Psc2;
    else if (index == 2)
        div = _sysconfig -> ReacCH1_Psc3;
    else
        return;
    ReacSetRange(1, div);
}

void MainWindow::on_SpeedCode1_valueChanged(int arg1)
{
    ReacSetVelCode(1, arg1);
}

void MainWindow::on_Pressure1_valueChanged(double arg1)
{
    ReacSetPress_(1, arg1);
}


void MainWindow::on_ModeCH2_currentIndexChanged(int index)
{
    if(index == 0 || index == 1) { //Ручной или Ручной без возврата
        ChXModeManual(ui -> Range2,
                     ui -> SpeedCode2,
                     ui -> Pressure2);
    } else if(index == 2) {
        ChXModeAuto(ui -> Range2,
                    ui -> SpeedCode2,
                    ui -> Pressure2);

        ReacSetRange(2, _sysconfig -> ReacCH2_Psc1);
     }

     ReacSetSyrMode(2, index);
}

void MainWindow::on_Range2_currentIndexChanged(int index)
{
    int div;

    if (index == 0)
        div = _sysconfig -> ReacCH2_Psc1;
    else if (index == 1)
        div = _sysconfig -> ReacCH2_Psc2;
    else if (index == 2)
        div = _sysconfig -> ReacCH2_Psc3;
    else
        return;
    ReacSetRange(2, div);
}

void MainWindow::on_SpeedCode2_valueChanged(int arg1)
{
    ReacSetVelCode(2, arg1);
}

void MainWindow::on_Pressure2_valueChanged(double arg1)
{
    ReacSetPress_(2, arg1);
}




void MainWindow::on_ModeCH3_currentIndexChanged(int index)
{
    if(index == 0 || index == 1) { //Ручной или Ручной без возврата
        ChXModeManual(ui -> Range3,
                     ui -> SpeedCode3,
                     ui -> Pressure3);
    } else if(index == 2) {
        ChXModeAuto(ui -> Range3,
                    ui -> SpeedCode3,
                    ui -> Pressure3);

        ReacSetRange(3, _sysconfig -> ReacCH3_Psc1);
     }

     ReacSetSyrMode(3, index);
}

void MainWindow::on_Range3_currentIndexChanged(int index)
{
    int div;

    if (index == 0)
        div = _sysconfig -> ReacCH3_Psc1;
    else if (index == 1)
        div = _sysconfig -> ReacCH3_Psc2;
    else if (index == 2)
        div = _sysconfig -> ReacCH3_Psc3;
    else
        return;
    ReacSetRange(3, div);
}

void MainWindow::on_SpeedCode3_valueChanged(int arg1)
{
    ReacSetVelCode(3, arg1);
}

void MainWindow::on_Pressure3_valueChanged(double arg1)
{
    ReacSetPress_(3, arg1);
}

void MainWindow::on_ModeCH4_currentIndexChanged(int index)
{
    if(index == 0 || index == 1) { //Ручной или Ручной без возврата
        ChXModeManual(ui -> Range4,
                     ui -> SpeedCode4,
                     ui -> Pressure4);
    } else if(index == 2) {
        ChXModeAuto(ui -> Range4,
                    ui -> SpeedCode4,
                    ui -> Pressure4);

        ReacSetRange(4, _sysconfig -> ReacCH4_Psc1);
     }

     ReacSetSyrMode(4, index);
}

void MainWindow::on_Range4_currentIndexChanged(int index)
{
    int div;

    if (index == 0)
        div = _sysconfig -> ReacCH4_Psc1;
    else if (index == 1)
        div = _sysconfig -> ReacCH4_Psc2;
    else if (index == 2)
        div = _sysconfig -> ReacCH4_Psc3;
    else
        return;
    ReacSetRange(4, div);
}

void MainWindow::on_SpeedCode4_valueChanged(int arg1)
{
    ReacSetVelCode(4, arg1);
}

void MainWindow::on_Pressure4_valueChanged(double arg1)
{
    ReacSetPress_(4, arg1);
}
