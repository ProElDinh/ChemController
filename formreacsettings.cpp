#include "formreacsettings.h"
#include "ui_formreacsettings.h"

FormReacSettings::FormReacSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormReacSettings)
{
    ui->setupUi(this);
}

FormReacSettings::~FormReacSettings()
{
    delete ui;
}
