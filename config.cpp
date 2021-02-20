#include "config.h"

DevSettings::DevSettings()
{

}

SysConfig::SysConfig(){


}
void DevSettings::Save(QString filePath){
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream out(&file); // поток записываемых данных направляем в файл
        out << "TStat_Kp = \""   << qSetRealNumberPrecision( 10 ) << TStat_Kp <<  "\"" << Qt::endl;
        out << "TStat_Ki = \""      << qSetRealNumberPrecision( 10 ) << TStat_Ki <<  "\"" << Qt::endl;
        out << "TStat_Kd = \""      << qSetRealNumberPrecision( 10 ) << TStat_Kd <<  "\"" << Qt::endl;
        out << "TStat_A = \""       << qSetRealNumberPrecision( 10 ) << TStat_A  <<  "\"" << Qt::endl;

        out << "Reac_Kp = \""       << qSetRealNumberPrecision( 10 ) << Reac_Kp  <<  "\"" << Qt::endl;
        out << "Reac_Ki = \""       << qSetRealNumberPrecision( 10 ) << Reac_Ki  <<  "\"" << Qt::endl;
        out << "Reac_Kd = \""       << qSetRealNumberPrecision( 10 ) << Reac_Kd  <<  "\"" << Qt::endl;
        out << "Reac_A = \""        << qSetRealNumberPrecision( 10 ) << Reac_A   <<  "\"" << Qt::endl;
     } else {

        qWarning("Could not open file");
        throw std::runtime_error("Could not open file");
      }
    file.close();
}

void DevSettings::Load(const QString filePath){

    QFile file(filePath);
   if(!file.open(QFile::ReadOnly | QFile::Text)){  // Проверяем есть ли файл и можно ли его открыть
       qDebug() << "Не получилось открыть или найти файл";
   }
   QTextStream stream(&file);  // Создаем потом, чтобы прочитать файл.
   QString buffer = "";
   while (!(buffer = stream.readLine()).isEmpty()){
        buffer = buffer.trimmed();
        if (buffer.length() > 1) //если не пустая строка
            {
                if (buffer[0] != ';') //если не комментарий
                {
                    ParseLine(buffer);
                }
        }
   }
   file.close();
}


void DevSettings::ParseLine(QString line){
    QStringList tp = line.split('=');
    if (tp.length()!= 2){
        qDebug() << "Неправильный формат \"" + line + "\"";
        throw std::runtime_error("Неправильный формат строк");
    }
    QString key = tp[0].trimmed();  // ключ
    QString value = tp[1].trimmed();  // значение
    if (value.length() >= 2){
        if (value[0] == '"' && value[value.length() - 1] == '"'){
            value = value.mid(1, value.length() - 2);
        }
        else {
            qDebug() << "Неправильный формат \"" + value + "\"";
            throw std::runtime_error("String parsing error: the value of the key must be in quotation marks");
                }
    }
    else {
        qDebug() << "Неправильный формат \"" + value + "\"";
        throw std::runtime_error("String parsing error: the value of the key must be in quotation marks");
        }


      if (key == "TStat_Kp") {
        TStat_Kp = value.toDouble();
    } else if (key == "TStat_Ki") {
        TStat_Ki = value.toDouble();

    } else if (key == "TStat_Kd") {
        TStat_Kd = value.toDouble();

    } else if (key == "TStat_A") {
        TStat_A = value.toDouble();

    } else if (key == "Reac_Kp") {
        Reac_Kp = value.toInt();

    } else if (key == "Reac_Ki") {
        Reac_Ki = value.toDouble();

    } else if (key == "Reac_Kd") {
        Reac_Kd = value.toInt();

    } else if (key == "Reac_A") {
        Reac_A = value.toInt();

    } else {
        qDebug() << "Неправильный формат: \"" + key + "\"";
        throw std::runtime_error("Неправильный формат строк");

    }

}






void SysConfig::Save(const QString filePath){
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream out(&file); // поток записываемых данных направляем в файл

        // Для записи данных в файл используем оператор <<
        out << "TStat_K1 = \"" << qSetRealNumberPrecision( 10 ) <<    TStat_K1 << "\"" << Qt::endl;
        out << "TStat_B1 = \"" << qSetRealNumberPrecision( 10 ) <<    TStat_B1 <<  "\"" << Qt::endl;
        out << "TStat_K2 = \"" << qSetRealNumberPrecision( 10 ) <<    TStat_K2 <<  "\"" << Qt::endl;
        out << "TStat_B2 = \"" << qSetRealNumberPrecision( 10 ) <<    TStat_B2 <<  "\"" << Qt::endl;

        out << "ReacCH1_Len = \"" << qSetRealNumberPrecision( 10 ) << ReacCH1_Len <<  "\"" << Qt::endl;
        out << "ReacCH1_Vol = \"" <<                                  ReacCH1_Vol << "\"" << Qt::endl;
        out << "ReacCH1_MicroStep = \"" <<                            ReacCH1_MicroStep << "\"" << Qt::endl;
        out << "ReacCH1_Psc1 = \"" <<                                 ReacCH1_Psc1 << "\"" << Qt::endl;
        out << "ReacCH1_Psc2 = \"" <<                                 ReacCH1_Psc2 <<  "\"" << Qt::endl;
        out << "ReacCH1_Psc3 = \"" <<                                 ReacCH1_Psc3 <<  "\"" << Qt::endl;
        out << "ReacCH1_X = \""  <<  qSetRealNumberPrecision( 10 ) << ReacCH1_X <<  "\"" << Qt::endl;
        out << "ReacCH1_Y = \""  <<  qSetRealNumberPrecision( 10 ) << ReacCH1_Y <<  "\"" << Qt::endl;

        out << "ReacCH2_Len = \"" << qSetRealNumberPrecision( 10 ) << ReacCH2_Len <<  "\"" << Qt::endl;
        out << "ReacCH2_Vol = \"" <<                                  ReacCH2_Vol << "\"" << Qt::endl;
        out << "ReacCH2_MicroStep = \"" <<                            ReacCH2_MicroStep << "\"" << Qt::endl;
        out << "ReacCH2_Psc1 = \"" <<                                 ReacCH2_Psc1 << "\"" << Qt::endl;
        out << "ReacCH2_Psc2 = \"" <<                                 ReacCH2_Psc2 <<  "\"" << Qt::endl;
        out << "ReacCH2_Psc3 = \"" <<                                 ReacCH2_Psc3 <<  "\"" << Qt::endl;
        out << "ReacCH2_X = \""  <<  qSetRealNumberPrecision( 10 ) << ReacCH2_X <<  "\"" << Qt::endl;
        out << "ReacCH2_Y = \""  <<  qSetRealNumberPrecision( 10 ) << ReacCH2_Y <<  "\"" << Qt::endl;

        out << "ReacCH3_Len = \"" << qSetRealNumberPrecision( 10 ) << ReacCH3_Len <<  "\"" << Qt::endl;
        out << "ReacCH3_Vol = \"" <<                                  ReacCH3_Vol  << "\"" << Qt::endl;
        out << "ReacCH3_MicroStep = \"" <<                            ReacCH3_MicroStep << "\"" << Qt::endl;
        out << "ReacCH3_Psc1 = \"" <<                                 ReacCH3_Psc1 << "\"" << Qt::endl;
        out << "ReacCH3_Psc2 = \"" <<                                 ReacCH3_Psc2 <<  "\"" << Qt::endl;
        out << "ReacCH3_Psc3 = \"" <<                                 ReacCH3_Psc3 <<  "\"" << Qt::endl;
        out << "ReacCH3_X = \""  <<  qSetRealNumberPrecision( 10 ) << ReacCH3_X <<  "\"" << Qt::endl;
        out << "ReacCH3_Y = \""  <<  qSetRealNumberPrecision( 10 ) << ReacCH3_Y <<  "\"" << Qt::endl;

        out << "ReacCH4_Len = \"" << qSetRealNumberPrecision( 10 ) << ReacCH4_Len << "\"" << Qt::endl;
        out << "ReacCH4_Vol = \"" <<                                  ReacCH4_Vol  << "\"" << Qt::endl;
        out << "ReacCH4_MicroStep = \"" <<                            ReacCH4_MicroStep << "\"" << Qt::endl;
        out << "ReacCH4_Psc1 = \"" <<                                 ReacCH4_Psc1 << "\"" << Qt::endl;
        out << "ReacCH4_Psc2 = \"" <<                                 ReacCH4_Psc2 <<  "\"" << Qt::endl;
        out << "ReacCH4_Psc3 = \"" <<                                 ReacCH4_Psc3 <<  "\"" << Qt::endl;
        out << "ReacCH4_X = \""  <<  qSetRealNumberPrecision( 10 ) << ReacCH4_X <<  "\"" << Qt::endl;
        out << "ReacCH4_Y = \""  <<  qSetRealNumberPrecision( 10 ) << ReacCH4_Y <<  "\"" << Qt::endl;


      } else {

        qWarning("Could not open file");
        throw std::runtime_error("Could not open file");
      }
    file.close();

}

void SysConfig::Load(const QString filePath){

    QFile mFile(filePath);
   if(!mFile.open(QFile::ReadOnly | QFile::Text)){  // Проверяем есть ли файл и можно ли его открыть
       qDebug() << "Не получилось открыть или найти файл";
   }
   QTextStream stream(&mFile);  // Создаем потом, чтобы прочитать файл.
   QString buffer = "";
   while (!(buffer = stream.readLine()).isEmpty()){
        buffer = buffer.trimmed();
        if (buffer.length() > 1) //если не пустая строка
            {
                if (buffer[0] != ';') //если не комментарий
                {
                    ParseLine(buffer);
                }
        }
   }
   mFile.close();
}


void SysConfig::ParseLine(QString line){
    QStringList tp = line.split('=');
    if (tp.length()!= 2){
        qDebug() << "Неправильный формат \"" + line + "\"";
        throw std::runtime_error("Неправильный формат строк");
    }
    QString key = tp[0].trimmed();  // ключ
    QString value = tp[1].trimmed();  // значение
    if (value.length() >= 2){
        if (value[0] == '"' && value[value.length() - 1] == '"'){
            value = value.mid(1, value.length() - 2);
        }
        else {
            qDebug() << "Неправильный формат \"" + value + "\"";
            throw std::runtime_error("String parsing error: the value of the key must be in quotation marks");
                }
    }
    else {
        qDebug() << "Неправильный формат \"" + value + "\"";
        throw std::runtime_error("String parsing error: the value of the key must be in quotation marks");
        }


      if (key == "TStat_B1") {
        TStat_B1 = value.toDouble();
    } else if (key == "TStat_K1") {
        TStat_K1 = value.toDouble();

    } else if (key == "TStat_K2") {
        TStat_K2 = value.toDouble();

    } else if (key == "TStat_B2") {
        TStat_B2 = value.toDouble();

    } else if (key == "ReacCH1_Len") {
        ReacCH1_Len = value.toInt();

    } else if (key == "ReacCH1_Vol") {
        ReacCH1_Vol = value.toDouble();

    } else if (key == "ReacCH1_MicroStep") {
        ReacCH1_MicroStep = value.toInt();

    } else if (key == "ReacCH1_Psc1") {
        ReacCH1_Psc1 = value.toInt();

    } else if (key == "ReacCH1_Psc2") {
        ReacCH1_Psc2 = value.toInt();

    } else if (key == "ReacCH1_Psc3") {
        ReacCH1_Psc3 = value.toInt();

    } else if (key == "ReacCH1_X") {
        ReacCH1_X = value.toDouble();

    } else if (key == "ReacCH1_Y") {
        ReacCH1_Y = value.toDouble();

    } else if (key == "ReacCH2_Len") {
        ReacCH2_Len = value.toInt();

    } else if (key == "ReacCH2_Vol") {
        ReacCH2_Vol = value.toDouble();

    } else if (key == "ReacCH2_MicroStep") {
        ReacCH2_MicroStep = value.toInt();

    } else if (key == "ReacCH2_Psc1") {
        ReacCH2_Psc1 = value.toInt();

    } else if (key == "ReacCH2_Psc2") {
        ReacCH2_Psc2 = value.toInt();

    } else if (key == "ReacCH2_Psc3") {
        ReacCH2_Psc3 = value.toInt();

    } else if (key == "ReacCH2_X") {
        ReacCH2_X = value.toDouble();

    } else if (key == "ReacCH2_Y") {
        ReacCH2_Y = value.toDouble();

    } else if (key == "ReacCH3_Len") {
        ReacCH3_Len = value.toInt();

    } else if (key == "ReacCH3_Vol") {
        ReacCH3_Vol = value.toDouble();

    } else if (key == "ReacCH3_MicroStep") {
        ReacCH3_MicroStep = value.toInt();

    } else if (key == "ReacCH3_Psc1") {
        ReacCH3_Psc1 = value.toInt();

    } else if (key == "ReacCH3_Psc2") {
        ReacCH3_Psc2 = value.toInt();

    } else if (key == "ReacCH3_Psc3") {
        ReacCH3_Psc3 = value.toInt();

    } else if (key == "ReacCH3_X") {
        ReacCH3_X = value.toDouble();

    } else if (key == "ReacCH3_Y") {
        ReacCH3_Y = value.toDouble();

    } else if (key == "ReacCH4_Len") {
        ReacCH4_Len = value.toInt();

    } else if (key == "ReacCH4_Vol") {
        ReacCH4_Vol = value.toDouble();

    } else if (key == "ReacCH4_MicroStep") {
        ReacCH4_MicroStep = value.toInt();

    } else if (key == "ReacCH4_Psc1") {
        ReacCH4_Psc1 = value.toInt();

    } else if (key == "ReacCH4_Psc2") {
        ReacCH4_Psc2 = value.toInt();

    } else if (key == "ReacCH4_Psc3") {
        ReacCH4_Psc3 = value.toInt();

    } else if (key == "ReacCH4_X") {
        ReacCH4_X = value.toDouble();

    } else if (key == "ReacCH4_Y") {
        ReacCH4_Y = value.toDouble();

    } else {
        qDebug() << "Неправильный формат: \"" + key + "\"";
        throw std::runtime_error("Неправильный формат строк");

    }

}
