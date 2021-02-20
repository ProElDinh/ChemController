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

private:
    Ui::MainWindow *ui;
    QLabel *_status;
    QLabel *_thermostat;
    QLabel *_reactor;
    ChemController *_chemcontroller;
    FormReacSettings *_reacSettings;
    FormTStatConfig * _tStatConfig;
    SysConfig *_sysconfig;
    DevSettings *_devsettings;

    QString DevSettingsFile = "config\\Settings.cnf";
    QString DevConfigFile = "config\\Config.cnf";

    QLabel *lblStatus;
    QLabel *StatusTStat;
    QLabel *StatusReac;
    QTimer *_pTimerCheckConnection;

    void StatusConnected();
    void StatusDisconnected();
    void ErrorMessage(QString err);
    void PressDisp(int ch, QLabel *label);
    void TemperDisplay(int ch, QLabel *label);
    void PowerDisp(QLabel *label);
    void StatusTStatEnable();
    void StatusTStatDisable();
    void StatusReacEnable();
    void StatusReacDisable();
    void UploadConfig();

};

#endif // MAINWINDOW_H
