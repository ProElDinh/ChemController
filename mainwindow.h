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
    void Read(); // Получение данных
    void Open(); // Открытие порта
    void WriteData(); // Отправка данных
    void Close();
    void on_action_triggered();

    void on_action_2_triggered();

private:
    Ui::MainWindow *ui;
    QLabel *status;
    QLabel *thermostat;
    QLabel *reactor;
    QSerialPort *serialPort;
    QByteArray serialData;
    QString serialBuffer;

};

#endif // MAINWINDOW_H
