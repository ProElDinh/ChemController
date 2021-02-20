#include "formtstatconfig.h"
#include "ui_formtstatconfig.h"

FormTStatConfig::FormTStatConfig(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormTStatConfig)
{
    ui->setupUi(this);
}

FormTStatConfig::~FormTStatConfig()
{
    delete ui;
}
