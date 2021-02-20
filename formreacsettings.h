#ifndef FORMREACSETTINGS_H
#define FORMREACSETTINGS_H

#include <QWidget>

namespace Ui {
class FormReacSettings;
}

class FormReacSettings : public QWidget
{
    Q_OBJECT

public:
    explicit FormReacSettings(QWidget *parent = nullptr);
    ~FormReacSettings();


private:
    Ui::FormReacSettings *ui;
};

#endif // FORMREACSETTINGS_H
