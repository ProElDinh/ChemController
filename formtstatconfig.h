#ifndef FORMTSTATCONFIG_H
#define FORMTSTATCONFIG_H

#include <QWidget>

namespace Ui {
class FormTStatConfig;
}

class FormTStatConfig : public QWidget
{
    Q_OBJECT

public:
    explicit FormTStatConfig(QWidget *parent = nullptr);
    ~FormTStatConfig();

private:
    Ui::FormTStatConfig *ui;
};

#endif // FORMTSTATCONFIG_H
