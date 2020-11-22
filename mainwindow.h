#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QLabel>


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


private:
    Ui::MainWindow *ui;
    QLabel *status;
    QLabel *thermostat;
    QLabel *reactor;


};

#endif // MAINWINDOW_H
