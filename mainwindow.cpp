#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _chemcontroller = new ChemController;
    _reacSettings = new FormReacSettings;
    _tStatConfig = new FormTStatConfig;
    lblStatus = new QLabel(this);
    StatusTStat = new QLabel(this);
    StatusReac = new QLabel(this);
    statusBar() -> addWidget(lblStatus);
    statusBar() -> addWidget(StatusTStat);
    statusBar() -> addWidget(StatusReac);
    //проверка соединения

    if (!_chemcontroller->isConnected()){
        StatusDisconnected();
    }

    connect(ui->MenuConnect, &QAction::triggered, this, &MainWindow:: Connect); // Подключить устройство

    connect(ui->MenuDisconnect, &QAction::triggered, this, &MainWindow:: Disconnect);  // Отключить устройство


    connect(ui->buttonTStatOn, &QPushButton::clicked, this, &MainWindow::TurnOnTemp);

    connect(ui->buttonTStatOff, &QPushButton::clicked, this, &MainWindow::TurnOffTemp);


    connect(ui->MenuReacSet,&QAction::triggered, this, &MainWindow::ReacSettings);

     connect(ui->MenuTStatConfig,&QAction::triggered, this, &MainWindow::TStatConfig);


}




MainWindow::~MainWindow()
{
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

    _reacSettings -> show();
}

void MainWindow::TStatConfig(){
    _tStatConfig -> show();
}

// Displays ************************************************************************/

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
        ui -> MenuReacStop -> setEnabled(true);
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


void MainWindow:: Connect(){
    /*  При включенном приборе нужно обязательно, хотя бы раз в секунду,
                посылать команду на запрос статуса, чтобы подтвердить, что соединение не разорвано.
                Иначе прибор будет выключен. Для этого создаем таймер с интервалом в 1 секунду. */

    _pTimerCheckConnection = new QTimer(this);  // Создаем таймер
    _pTimerCheckConnection->setInterval(1000);

    /* По истечении времени 1 с вызывается команда запроса статуса.
             */

    connect(_pTimerCheckConnection, &QTimer::timeout, [this](){
        try {
            _chemcontroller -> OpenPort();
            _chemcontroller -> CheckConnect();
            StatusConnected();
        }  catch (...) {
            if (_chemcontroller -> IsOpen()){
                _chemcontroller -> ClosePort();
            }
            _pTimerCheckConnection->stop();
            ErrorMessage("Ошибка подключения");
            StatusDisconnected();
        }
    });
    _pTimerCheckConnection->start();  // запускаем таймер
}



void MainWindow:: Disconnect(){

    _chemcontroller-> ClosePort();
    StatusDisconnected();
    _pTimerCheckConnection->stop();
}

void MainWindow:: UploadConfig(){


}


void MainWindow::TurnOnTemp(){
    try {
        _chemcontroller->TStatEnable();
    }  catch (...) {
        ErrorMessage("Ошибка. Неудалось включить термостат!");
    }

}

void MainWindow::TurnOffTemp(){
    try {
        _chemcontroller-> TStatDisable();
    }  catch (...) {
        ErrorMessage("Ошибка. Неудалось выключить термостат!");
    }

}


void MainWindow::on_set_tempbox_valueChanged(double arg1)
{
    qDebug() << arg1;
}

