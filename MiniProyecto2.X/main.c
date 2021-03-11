/* 
 * File:   MiniProyecto 2
 * Author: Mario Soto
 */

//**************************
// Importación de librerías
//**************************

#include <xc.h>
#include <stdint.h>
#include "pic16f887.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "I2C.h"
#include "RTC.h"
#define _XTAL_FREQ 4000000
//Definimos Variables de Entrada y Escritura del Sensor RTC.
uint8_t i;
uint8_t second;
uint8_t minute;
uint8_t hour;
uint8_t m_day;
uint8_t month;
uint8_t year;
uint8_t Semaforo;

//**************************
// Palabra de configuración
//**************************
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming
// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)
//Definimos Prototipos de Funciones
void Setup(void);
//Iniciamos con las Funciones

void main(void) {
    Setup();
    //Loop Principal
    while (1) {
        LECTURARTC();
        TXREG = minute;
        __delay_ms(20);
        PORTD = Semaforo;
    }
}
//Configuramos todo lo necesario para el proyecto.

void Setup() {
    //Configuracion del Oscilador.
    OSCCONbits.IRCF = 0b110; //4Mhz
    OSCCONbits.OSTS = 0;
    OSCCONbits.HTS = 0;
    OSCCONbits.LTS = 0;
    OSCCONbits.SCS = 1;
    //Configuracion de Puertos
    TRISD = 0;
    TRISC = 0b11011000;
    PORTC = 0;
    PORTD = 0;
    //Configuracion de Variables para el RTC
    minute = 30;
    hour = 2;
    m_day = 3;
    month = 1;
    year = 16;
    Semaforo = 0b00000000;
    //Configuracion de Interrupciones
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PIE1bits.RCIE = 1;
    // Reiniciamos las Banderas
    PIR1bits.RCIF = 0;
    //Configuramos el BaudRate de Eusart a 9600
    SPBRGH = 0;
    SPBRG = 0b00011001;
    BAUDCTLbits.BRG16 = 0;
    //Configuramos La Transmision Serial
    TXSTA = 0b00100100;
    RCSTA = 0b10010000;
    //Inicializamos Comunicacion I2C
    I2C_Master_Init(100000);
    CONFIGRTC(minute, hour, m_day, month, year);
}

void __interrupt() ISR(void) {
    if (PIR1bits.RCIF == 1) {
        PIR1bits.RCIF = 0;
        Semaforo = RCREG;
        return;
    }
}