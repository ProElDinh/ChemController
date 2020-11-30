#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QLabel>
#include <QMessageBox>

#include <QThread>

#include "chemcontroller.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void StatusBar(QString status);
    void Connect();
    void Disconnect();
signals:
    void SetTempRequest(double temp);

private slots:

private:
    Ui::MainWindow *ui;
    QLabel *_status = new QLabel(this);
    QLabel *_thermostat = new QLabel(this) ;
    QLabel *_reactor = new QLabel(this);
    // Устройтсво и отдельный поток, в котором он будет работать.
    QThread *_Thread;
    ChemController *_chemconroller;

};

#endif // MAINWINDOW_H
