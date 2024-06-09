#include "DS1307.h"

DS1307::DS1307(PinName sda, PinName scl) : i2c(sda, scl) {
    i2c.frequency(100000); // 100kHz I2C frequency
}

bool DS1307::clearChBit() {
    char data[2];
    data[0] = 0x00; // Register address for the seconds register

    // Read the current value of the seconds register
    if (i2c.write(DS1307_ADDRESS, data, 1, true) != 0) {
        return false; // I2C write failed
    }
    if (i2c.read(DS1307_ADDRESS, &data[1], 1) != 0) {
        return false; // I2C read failed
    }

    // Clear the CH bit (bit 7)
    data[1] &= 0x7F;

    // Write the modified value back to the seconds register
    if (i2c.write(DS1307_ADDRESS, data, 2) != 0) {
        return false; // I2C write failed
    }

    return true;
}

bool DS1307::isRtcRunning() {
    char data[1] = {0}; // Register address for the seconds register

    // Write the register address
    if (i2c.write(DS1307_ADDRESS, data, 1, true) != 0) {
        return false; // I2C write failed
    }

    // Read the register value
    if (i2c.read(DS1307_ADDRESS, data, 1) != 0) {
        return false; // I2C read failed
    }

    // Check if the clock halt bit (bit 7) is 0
    return (data[0] & 0x80) == 0;
}

bool DS1307::readRtcTime(int &seconds, int &minutes, int &hours, int &day, int &date, int &month, int &year) {
    char data[7];
    char regAddr = 0x00;

    // Write the register address
    if (i2c.write(DS1307_ADDRESS, &regAddr, 1, true) != 0) {
        return false; // I2C write failed
    }

    // Read the time registers
    if (i2c.read(DS1307_ADDRESS, data, 7) != 0) {
        return false; // I2C read failed
    }

    // Convert BCD to decimal
    seconds = ((data[0] >> 4) * 10) + (data[0] & 0x0F);
    minutes = ((data[1] >> 4) * 10) + (data[1] & 0x0F);
    hours = ((data[2] >> 4) * 10) + (data[2] & 0x0F);
    day = data[3] & 0x07;
    date = ((data[4] >> 4) * 10) + (data[4] & 0x0F);
    month = ((data[5] >> 4) * 10) + (data[5] & 0x0F);
    year = ((data[6] >> 4) * 10) + (data[6] & 0x0F) + 2000;

    return true;
}

bool DS1307::setRtcTime(int seconds, int minutes, int hours, int day, int date, int month, int year) {
    char data[8];
    data[0] = 0x00; // Register address for the seconds register
    data[1] = ((seconds / 10) << 4) + (seconds % 10); // Seconds
    data[2] = ((minutes / 10) << 4) + (minutes % 10); // Minutes
    data[3] = ((hours / 10) << 4) + (hours % 10); // Hours
    data[4] = day & 0x07; // Day of the week
    data[5] = ((date / 10) << 4) + (date % 10); // Date
    data[6] = ((month / 10) << 4) + (month % 10); // Month
    data[7] = (((year % 100) / 10) << 4) + ((year % 100) % 10); // Year

    // Write the time data to the RTC
    if (i2c.write(DS1307_ADDRESS, data, 8) != 0) {
        return false; // I2C write failed
    }

    return true;
}