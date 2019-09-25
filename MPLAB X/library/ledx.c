#include "ledx.h"
#include "fosc.h"


void LED_Init(void) {
    LED_DIR = 0;
    LED_IO = LED_OFF;
}

void LEDx(unsigned char num) {
    while(num--) {
        LED_IO = LED_ON; 
        __delay_ms(100);
        LED_IO = LED_OFF;
        __delay_ms(100);
    }
}

void LEDs(unsigned char sta) {
    LED_IO = sta; 
}