#ifndef DS1307_H
#define DS1307_H

#include "mbed.h"

class DS1307 {
public:
    DS1307(PinName sda, PinName scl);

    bool clearChBit();
    bool isRtcRunning();
    bool readRtcTime(int &seconds, int &minutes, int &hours, int &day, int &date, int &month, int &year);
    bool setRtcTime(int seconds, int minutes, int hours, int day, int date, int month, int year);

private:
    I2C i2c;
    static const int DS1307_ADDRESS = 0x68 << 1; // DS1307 I2C address (shifted for mbed)
};

#endif // DS1307_H