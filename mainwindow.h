#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QLabel>
#include <QMessageBox>

#include <QThread>
#include "ui_mainwindow.h"
#include "chemcontroller.h"
#include <formreacsettings.h>
#include <formtstatconfig.h>
#include <config.h>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void Connect();
    void Disconnect();
    void TurnOnTemp();
    void TurnOffTemp();
    void ReacSettings();
    void TStatConfig();

public slots:

private slots:
    void on_set_tempbox_valueChanged(double arg1);

    void on_ModeCH1_currentIndexChanged(int index);

    void on_Range1_currentIndexChanged(int index);

    void on_SpeedCode1_valueChanged(int arg1);

    void on_Pressure1_valueChanged(double arg1);

    void on_ModeCH2_currentIndexChanged(int index);

    void on_Range2_currentIndexChanged(int index);

    void on_SpeedCode2_valueChanged(int arg1);

    void on_Pressure2_valueChanged(double arg1);

    void on_ModeCH3_currentIndexChanged(int index);

    void on_Range3_currentIndexChanged(int index);

    void on_SpeedCode3_valueChanged(int arg1);

    void on_Pressure3_valueChanged(double arg1);

    void on_ModeCH4_currentIndexChanged(int index);

    void on_Range4_currentIndexChanged(int index);

    void on_SpeedCode4_valueChanged(int arg1);

    void on_Pressure4_valueChanged(double arg1);

private:
    Ui::MainWindow *ui;

    // Статусы в StatusBar
    QLabel *lblStatus;
    QLabel *StatusTStat;
    QLabel *StatusReac;


    ChemController *_chemcontroller;
    SysConfig *_sysconfig;
    DevSettings *_devsettings;

    //  Экземпляры окон
    FormReacSettings *_reacSettings;
    FormTStatConfig * _tStatConfig;


    QString DevSettingsFile = "E:\\ChemControllerGUI\\ChemControllerGUI\\Settings.cnf";
    QString DevConfigFile = "E:\\ChemControllerGUI\\ChemControllerGUI\\Config.cnf";

    // Таймер для обновления данных
    QTimer *_TimerRefreshInfo;

    int *PSC_CHx;

    void ErrorMessage(QString err);
    void Refresh();

    //
    void PressDisp(int ch, QLabel *label);
    void TemperDisplay(int ch, QLabel *label);
    void PowerDisp(QLabel *label);
    void ReacDispMode(int ch, QComboBox *cmb);
    void ReacDispRange(int ch, QComboBox *cmbRange, int R1, int R2, int R3);
    void ReacDispVelCode(int ch, QSpinBox *numVelCode);
    void ReacDispPress(int ch, QDoubleSpinBox *numPress);
    void ChXModeAuto(QComboBox *cmbRange, QSpinBox *VelCode, QDoubleSpinBox *Set);
    void ChXModeManual(QComboBox *cmbRange, QSpinBox *VelCode, QDoubleSpinBox *Set);


    void SyrRangeDisp(QLabel *label, QComboBox *combo, int Psc, int ReacCHx_Len, qreal ReacCHx_Vol, int ReacCHx_MicroStep, int RefFreq);
    void SyrVelocityDisp(QLabel *label, QComboBox *combo, int VelCode, int Psc, int ReacCHx_Len, qreal ReacCHx_Vol, int ReacCHx_MicroStep, int RefFreq);
    void SyrVolDisp(int ch, QLabel *label, int ReacCHx_Len, qreal ReacCHx_Vol, int ReacCHx_MicroStep);

    //
    void StatusConnected();
    void StatusDisconnected();
    void StatusTStatEnable();
    void StatusTStatDisable();
    void StatusReacEnable();
    void StatusReacDisable();

    //
    void UploadConfig();
    void UploadReacConfig();
    void UploadTStatPID();
    void UploadTStatCoeff();
    void SafeConfig();


    void SettingsDefault();
    void ConfigDefault();
    void LoadConfig();

    void ReacRecalibration();
    void ReacStart();
    void ReacStop();
    void ReacSetSyrMode(int ch, int mode);
    void ReacSetRange(int ch, int Psc);
    void ReacSetVelCode(int ch, int val);
    void ReacSetPress_(int ch, qreal val);
    void ReacSetPID();

    qreal SyrCode2Vol(int c, int ReacCHx_Len, qreal ReacCHx_Vol, int ReacCHx_MicroStep);
    qreal SyrVelocity(int VelCode, int Psc, int ReacCHx_Len, qreal ReacCHx_Vol, int ReacCHx_MicroStep, int RefFreq);

    void TStatSetTemper(double val);
};

#endif // MAINWINDOW_H
