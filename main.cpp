#include "mbed.h"
#include "DS1307.h"

int main() {
    // Initialize the DS1307 object with appropriate I2C pins
    DS1307 rtc(PTB1, PTB0); // Replace I2C_SDA and I2C_SCL with your specific pins

    // Ensure the RTC is running
    if (!rtc.isRtcRunning()) {
        if (rtc.clearChBit()) {
            printf("RTC DS1307 started successfully.\n");
        } else {
            printf("Failed to start RTC DS1307.\n");
            return 1; // Exit the program if the RTC couldn't be started
        }
    } else {
        printf("RTC DS1307 is already running.\n");
    }

    // Example to set the time: 12:30:00 on January 1st, 2024, Monday
    if (rtc.setRtcTime(0, 30, 12, 2, 1, 1, 2024)) {
        printf("Time set successfully.\n");
    } else {
        printf("Failed to set time.\n");
        return 1; // Exit the program if the time couldn't be set
    }

    int seconds, minutes, hours, day, date, month, year;

    while (true) {
        // Read and print the current time from the RTC
        if (rtc.readRtcTime(seconds, minutes, hours, day, date, month, year)) {
            printf("Current RTC time: %02d:%02d:%02d, Date: %02d/%02d/%04d, Day of the week: %d\n",
                   hours, minutes, seconds, date, month, year, day);
        } else {
            printf("Failed to read time from RTC DS1307.\n");
        }

        // Wait for 1 second before reading the time again
        ThisThread::sleep_for(1s);
    }
}
