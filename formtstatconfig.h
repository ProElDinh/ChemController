#ifndef FORMTSTATCONFIG_H
#define FORMTSTATCONFIG_H
#include "config.h"
#include <QWidget>

namespace Ui {
class FormTStatConfig;
}

class FormTStatConfig : public QWidget
{
    Q_OBJECT

signals:
    void SaveConfig();

public:
    FormTStatConfig(SysConfig *config, DevSettings *devSettings, QWidget *parent = nullptr);
    ~FormTStatConfig();
    void LoadConfig();

    SysConfig *config;
    DevSettings *settings;


private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::FormTStatConfig *ui;
};

#endif // FORMTSTATCONFIG_H
