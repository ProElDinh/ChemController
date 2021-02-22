#ifndef FORMREACSETTINGS_H
#define FORMREACSETTINGS_H
#include "config.h"
#include <QWidget>

namespace Ui {
class FormReacSettings;
}

class FormReacSettings : public QWidget
{
    Q_OBJECT
signals:
    void SaveConfig();
public:
    FormReacSettings(SysConfig *config, DevSettings *devSettings, QWidget *parent = nullptr);
    ~FormReacSettings();

    void LoadConfig();
    SysConfig *config;
    DevSettings *settings;


private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::FormReacSettings *ui;
};

#endif // FORMREACSETTINGS_H
