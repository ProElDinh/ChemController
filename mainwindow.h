#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QLabel>
#include <QSerialPort>
#include <QSerialPortInfo>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void StatusBar();
private slots:
    void serialRecieve(); // Получаем данные
    void serialcheck();


private:
    Ui::MainWindow *ui;
    QLabel *status;
    QLabel *thermostat;
    QLabel *reactor;
    QSerialPort *serialPort;

};

#endif // MAINWINDOW_H
