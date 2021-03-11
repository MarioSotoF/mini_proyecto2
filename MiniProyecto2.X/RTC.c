/*
 * File            : RTC.c
 * Author          : SIMPLE CIRCUIT
 * Basado en el Link: https://simple-circuit.com/mplab-xc8-ds3231-i2c-lcd-pic-mcu/
 */

#include "RTC.h"
#include "I2C.h"

uint8_t i, second, minute, hour, m_day, month, year;
// convert BCD to decimal function

uint8_t bcd_to_decimal(uint8_t number) {
    return ((number >> 4) * 10 + (number & 0x0F));
}

// convert decimal to BCD function

uint8_t decimal_to_bcd(uint8_t number) {
    return (((number / 10) << 4) + (number % 10));
}
//Configuracion de Comunicacion de Sensor
uint8_t CONFIGRTC(uint8_t minute, uint8_t hour, uint8_t m_day, uint8_t month, uint8_t year) {
    // convert decimal to BCD
    minute = decimal_to_bcd(minute);
    hour = decimal_to_bcd(hour);
    m_day = decimal_to_bcd(m_day);
    month = decimal_to_bcd(month);
    year = decimal_to_bcd(year);
    // end conversion
    // write data to RTC chip
    I2C_Master_Start(); // start I2C
    I2C_Master_Write(0xD0); // RTC chip address
    I2C_Master_Write(0); // send register address
    I2C_Master_Write(0); // reset seconds and start oscillator
    I2C_Master_Write(minute); // write minute value to RTC chip
    I2C_Master_Write(hour); // write hour value to RTC chip
    I2C_Master_Write(1); // write day value (not used)
    I2C_Master_Write(m_day); // write date value to RTC chip
    I2C_Master_Write(month); // write month value to RTC chip
    I2C_Master_Write(year); // write year value to RTC chip
    I2C_Master_Stop(); // stop I2C
    __delay_ms(200);
    return 1;
}
//Lectura de Sensor.
uint8_t LECTURARTC() {
    // read current time and date from the RTC chip
    I2C_Master_Start(); // start I2C
    I2C_Master_Write(0xD0); // RTC chip address
    I2C_Master_Write(0); // send register address
    I2C_Master_RepeatedStart(); // restart I2C
    I2C_Master_Write(0xD1); // initialize data read
    second = I2C_Master_Read(1); // read seconds from register 0
    minute = I2C_Master_Read(1); // read minutes from register 1
    hour = I2C_Master_Read(1); // read hour from register 2
    I2C_Master_Read(1); // read day from register 3 (not used)
    m_day = I2C_Master_Read(1); // read date from register 4
    month = I2C_Master_Read(1); // read month from register 5
    year = I2C_Master_Read(0); // read year from register 6
    I2C_Master_Stop(); // stop I2C
    __delay_ms(50); // wait 50 ms
    return second, minute, hour, m_day, month, year;
}