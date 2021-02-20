#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QLabel>
#include <QMessageBox>

#include <QThread>

#include "chemcontroller.h"
#include <formreacsettings.h>
#include <formtstatconfig.h>
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
    QLabel *_status = new QLabel(this);
    QLabel *_thermostat = new QLabel(this) ;
    QLabel *_reactor = new QLabel(this);
    ChemController *_chemconroller;
    FormReacSettings *_reacSettings;
    FormTStatConfig * _tStatConfig;
    void StatusConnected();
    void StatusDisconnected();
    void StatusBar(QString status);
    void ErrorMessage(QString err);

};

#endif // MAINWINDOW_H
