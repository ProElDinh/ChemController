#include "formtstatconfig.h"
#include "ui_formtstatconfig.h"
#include <QDebug>
FormTStatConfig::FormTStatConfig(SysConfig *sysconfig, DevSettings *devsettings, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormTStatConfig)
{
    config = sysconfig;
    settings = devsettings;
    ui->setupUi(this);


}

FormTStatConfig::~FormTStatConfig()
{
    delete ui;
}

void FormTStatConfig::on_buttonBox_accepted()
{
    settings -> TStat_Kp = ui -> numKp ->value();
    settings -> TStat_Ki = ui -> numKi ->value();
    settings -> TStat_Kd = ui -> numKd ->value();
    settings -> TStat_A  = ui -> numa -> value();

    config -> TStat_K1 = ui -> numK1 -> value();
    config -> TStat_B1 = ui -> numB1 -> value();
    config -> TStat_K2 = ui -> numK2 -> value();
    config -> TStat_B2 = ui -> numB2 -> value();
    emit SaveConfig();
    close();
}

void FormTStatConfig::LoadConfig(){
    ui -> numKp -> setValue(settings ->TStat_Kp);
    ui -> numKi -> setValue(settings ->TStat_Ki);
    ui -> numKd -> setValue(settings ->TStat_Kd);
    ui -> numa  -> setValue(settings ->TStat_A);

    ui -> numK1 -> setValue(config -> TStat_K1);
    ui -> numB1 -> setValue(config -> TStat_B1);
    ui -> numK2 -> setValue(config -> TStat_K2);
    ui -> numB2 -> setValue(config -> TStat_B2);
}

void FormTStatConfig::on_buttonBox_rejected()
{
    close();
}
