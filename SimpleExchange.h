#ifndef SIMPLEEXCHANGE_H
#define SIMPLEEXCHANGE_H

class SimpleExchange {
private:

    unsigned short int* Crc16Table; // Создание динамического массива

public:
    SimpleExchange();
    ~SimpleExchange();
    unsigned short int Crc16(unsigned short int pcBlock[], int len);
};

#endif // SIMPLEEXCHANGE_H
