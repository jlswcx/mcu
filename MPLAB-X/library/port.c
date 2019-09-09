#include "port.h"
#include <pic.h>

void PORT_Init(void) {
    PORTA_Init();
    PORTC_Init();
}

void PORTA_Init(void) {
    PORTA = 0;
    LATA = 0;
    ANSELA = 0;
    TRISA = 0;
}

void PORTC_Init(void) {
    PORTC = 0;
    LATC = 0;
    ANSELC = 0;
    TRISC = 0;
}
