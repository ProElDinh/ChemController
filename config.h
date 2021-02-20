#ifndef CONFIG_H
#define CONFIG_H
#include <QDebug>
#include <QString>
#include <QFile>
class DevSettings
{
private:
    void ParseLine(QString line);

public:
    DevSettings();
    ~DevSettings();
    void Load(QString path);
    void Save(QString path);
    QString COM_PORT;
    qreal TStat_Kp;
    qreal TStat_Ki;
    qreal TStat_Kd;
    qreal TStat_A;
    qreal Reac_Kp;
    qreal Reac_Ki;
    qreal Reac_Kd;
    qreal Reac_A;


};


class SysConfig {
private:
    void ParseLine(QString line);

public:
    SysConfig();
    ~SysConfig();
    void Save(QString path);
    void Load(QString path);
    qreal TStat_K1;
    qreal TStat_B1;
    qreal TStat_K2;
    qreal TStat_B2;

    int ReacCH1_Len;
    qreal ReacCH1_Vol;
    int ReacCH1_MicroStep;
    int ReacCH1_Psc1;
    int ReacCH1_Psc2;
    int ReacCH1_Psc3;
    qreal ReacCH1_X;
    qreal ReacCH1_Y;

    int ReacCH2_Len;
    qreal ReacCH2_Vol;
    int ReacCH2_MicroStep;
    int ReacCH2_Psc1;
    int ReacCH2_Psc2;
    int ReacCH2_Psc3;
    qreal ReacCH2_X;
    qreal ReacCH2_Y;

    int ReacCH3_Len;
    qreal ReacCH3_Vol;
    int ReacCH3_MicroStep;
    int ReacCH3_Psc1;
    int ReacCH3_Psc2;
    int ReacCH3_Psc3;
    qreal ReacCH3_X;
    qreal ReacCH3_Y;

    int ReacCH4_Len;
    qreal ReacCH4_Vol;
    int ReacCH4_MicroStep;
    int ReacCH4_Psc1;
    int ReacCH4_Psc2;
    int ReacCH4_Psc3;
    qreal ReacCH4_X;
    qreal ReacCH4_Y;


};


#endif // CONFIG_H
