#include "formreacsettings.h"
#include "ui_formreacsettings.h"

FormReacSettings::FormReacSettings(SysConfig *sysconfig, DevSettings *devsettings, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormReacSettings)
{
    config = sysconfig;
    settings = devsettings;
    setWindowTitle("Настройка реактора");
    ui->setupUi(this);
}

FormReacSettings::~FormReacSettings()
{
    delete ui;
}

void FormReacSettings::on_buttonBox_accepted()
{
    settings -> Reac_Kp = ui -> numKp -> value();
    settings -> Reac_Ki = ui -> numKi -> value();
    settings -> Reac_Kd = ui -> numKd -> value();
    settings -> Reac_A  = ui -> numKa -> value();

    config -> ReacCH1_Len =         ui -> numLenCH1 -> value();
    config -> ReacCH1_Vol =         ui -> numVolCH1 -> value();
    config -> ReacCH1_MicroStep =   ui -> numMicroStepCH1 -> value();
    config -> ReacCH1_Psc1 =        ui -> numPsc1CH1 -> value();
    config -> ReacCH1_Psc2 =        ui -> numPsc2CH1 -> value();
    config -> ReacCH1_Psc3 =        ui -> numPsc3CH1 -> value();
    config -> ReacCH1_X =           ui -> numXCH1 -> value();
    config -> ReacCH1_Y =           ui -> numYCH1 -> value();

    config -> ReacCH2_Len =         ui -> numLenCH2 -> value();
    config -> ReacCH2_Vol =         ui -> numVolCH2 -> value();
    config -> ReacCH2_MicroStep =   ui -> numMicroStepCH2 -> value();
    config -> ReacCH2_Psc1 =        ui -> numPsc1CH2 -> value();
    config -> ReacCH2_Psc2 =        ui -> numPsc2CH2 -> value();
    config -> ReacCH2_Psc3 =        ui -> numPsc3CH2 -> value();
    config -> ReacCH2_X =           ui -> numXCH2 -> value();
    config -> ReacCH2_Y =           ui -> numYCH2 -> value();


    config -> ReacCH3_Len =         ui -> numLenCH3 -> value();
    config -> ReacCH3_Vol =         ui -> numVolCH3 -> value();
    config -> ReacCH3_MicroStep =   ui -> numMicroStepCH3 -> value();
    config -> ReacCH3_Psc1 =        ui -> numPsc1CH3 -> value();
    config -> ReacCH3_Psc2 =        ui -> numPsc2CH3 -> value();
    config -> ReacCH3_Psc3 =        ui -> numPsc3CH3 -> value();
    config -> ReacCH3_X =           ui -> numXCH3 -> value();
    config -> ReacCH3_Y =           ui -> numYCH3 -> value();

    config -> ReacCH4_Len =         ui -> numLenCH4 -> value();
    config -> ReacCH4_Vol =         ui -> numVolCH4 -> value();
    config -> ReacCH4_MicroStep =   ui -> numMicroStepCH4 -> value();
    config -> ReacCH4_Psc1 =        ui -> numPsc1CH4 -> value();
    config -> ReacCH4_Psc2 =        ui -> numPsc2CH4 -> value();
    config -> ReacCH4_Psc3 =        ui -> numPsc3CH4 -> value();
    config -> ReacCH4_X =           ui -> numXCH4 -> value();
    config -> ReacCH4_Y =           ui -> numYCH4 -> value();

    emit SaveConfig();
    close();
}

void FormReacSettings::LoadConfig(){

    ui -> numKp -> setValue(settings -> Reac_Kp);
    ui -> numKi -> setValue(settings -> Reac_Ki);
    ui -> numKd -> setValue(settings -> Reac_Kd);
    ui -> numKa -> setValue(settings -> Reac_A);

    ui -> numLenCH1 -> setValue(config -> ReacCH1_Len);
    ui -> numVolCH1 -> setValue(config -> ReacCH1_Vol);
    ui -> numMicroStepCH1 -> setValue(config -> ReacCH1_MicroStep);
    ui -> numPsc1CH1 -> setValue(config -> ReacCH1_Psc1);
    ui -> numPsc2CH1 -> setValue(config -> ReacCH1_Psc2);
    ui -> numPsc3CH1 -> setValue(config -> ReacCH1_Psc3);
    ui -> numXCH1 -> setValue(config -> ReacCH1_X);
    ui -> numYCH1 -> setValue(config -> ReacCH1_Y);

    ui -> numLenCH2 -> setValue(config -> ReacCH2_Len);
    ui -> numVolCH2 -> setValue(config -> ReacCH2_Vol);
    ui -> numMicroStepCH2 -> setValue(config -> ReacCH2_MicroStep);
    ui -> numPsc1CH2 -> setValue(config -> ReacCH2_Psc1);
    ui -> numPsc2CH2 -> setValue(config -> ReacCH2_Psc2);
    ui -> numPsc3CH2 -> setValue(config -> ReacCH2_Psc3);
    ui -> numXCH2 -> setValue(config -> ReacCH2_X);
    ui -> numYCH2 -> setValue(config -> ReacCH2_Y);

    ui -> numLenCH3 -> setValue(config -> ReacCH3_Len);
    ui -> numVolCH3 -> setValue(config -> ReacCH3_Vol);
    ui -> numMicroStepCH3 -> setValue(config -> ReacCH3_MicroStep);
    ui -> numPsc1CH3 -> setValue(config -> ReacCH3_Psc1);
    ui -> numPsc2CH3 -> setValue(config -> ReacCH3_Psc2);
    ui -> numPsc3CH3 -> setValue(config -> ReacCH3_Psc3);
    ui -> numXCH3 -> setValue(config -> ReacCH3_X);
    ui -> numYCH3 -> setValue(config -> ReacCH3_Y);

    ui -> numLenCH4 -> setValue(config -> ReacCH4_Len);
    ui -> numVolCH4 -> setValue(config -> ReacCH4_Vol);
    ui -> numMicroStepCH4 -> setValue(config -> ReacCH4_MicroStep);
    ui -> numPsc1CH4 -> setValue(config -> ReacCH4_Psc1);
    ui -> numPsc2CH4 -> setValue(config -> ReacCH4_Psc2);
    ui -> numPsc3CH4 -> setValue(config -> ReacCH4_Psc3);
    ui -> numXCH4 -> setValue(config -> ReacCH4_X);
    ui -> numYCH4 -> setValue(config -> ReacCH4_Y);
}

void FormReacSettings::on_buttonBox_rejected()
{
    close();
}


