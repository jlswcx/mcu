#include <stdio.h>
#include <stdlib.h>

#include <pic.h>
#include <eeprom_routines.h>
// PIC16F1823 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select (MCLR/VPP pin function is digital input)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable (Brown-out Reset disabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = OFF       // Internal/External Switchover (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is disabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN = OFF      // PLL Enable (4x PLL disabled)
#pragma config STVREN = OFF     // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will not cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = OFF         // Low-Voltage Programming Enable (Low-voltage programming enabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>


#define     _XTAL_FREQ      125000      

void TestLED(void);

void main(void) {
    
    
    TestLED();
}



void TestLED(void){
    int i;
    i = 1;
    
    TRISA = 0x00;
    TRISC = 0x00;      
                  
    while(1){
       
        // À≥ ±’Î
        i = 0;
        do{
        PORTA = 0x1; PORTC = 0;
        delay();
        PORTA = 0x2;
        delay();
        PORTA = 0x4;
        delay();
        PORTA = 0x0; PORTC = 0x1;
        delay();
        PORTC = 0x2;
        delay();
        PORTC = 0x4;
        delay();
        PORTC = 0x8;
        delay();
        PORTC = 0x10;
        delay();
        }while(i++ < 1);
        
        // ƒÊ ±’Î
        i = 0;
        do{
        PORTC = 0x8;
        delay();
        PORTC = 0x4;
        delay();
        PORTC = 0x2;
        delay();
        PORTC = 0x1;
        delay();
        PORTA = 0x4; PORTC = 0x0;
        delay();
        PORTA = 0x2;
        delay();
        PORTA = 0x1;
        delay();
        PORTA = 0x0; PORTC = 0x10;
        delay();
        }while(i++ < 1);
        
        // …¡À∏
        i = 0;
        do{
        PORTA = 0xff; PORTC = 0xff;
        delay();
        PORTA = 0x0; PORTC = 0x0;
        delay();
        }while(i++ < 2);
        
        
        // ÷Ω•»´¡¡µΩ»´√
        i = 0;
        do{
        PORTA = 0x01;
        delay();
        PORTA = 0x03;
        delay();
        PORTA = 0x07;
        delay();
        PORTC = 0x01;
        delay();
        PORTC = 0x03;
        delay();
        PORTC = 0x07;
        delay();
        PORTC = 0x0F;
        delay();
        PORTC = 0x1F;
        delay();
        PORTA = 0x06;
        delay();
        PORTA = 0x04;
        delay();
        PORTA = 0x00;
        delay();
        PORTC = 0x1E;
        delay();
        PORTC = 0x1C;
        delay();
        PORTC = 0x18;
        delay();
        PORTC = 0x10;
        delay();
        PORTC = 0x00;
        delay();
        }while(i++ < 1);
        
        // …¡À∏
        i = 0;
        do{
        PORTA = 0xff; PORTC = 0xff;
        delay();
        PORTA = 0x0; PORTC = 0x0;
        delay();
        }while(i++ < 2);
        
        
        
        // À≥ ±’Î
        i = 0;
        do{
        PORTA = 0x1; PORTC = 0x2;
        delay();
        PORTA = 0x2; PORTC = 0x4;
        delay();
        PORTA = 0x4; PORTC = 0x8;
        delay();
        PORTA = 0x0; PORTC = 0x11;
        delay();
        }while(i++ < 1);
    
        // ƒÊ ±’Î
        i = 0;
        do{
        PORTA = 0x4; PORTC = 0x8;
        delay();
        PORTA = 0x2; PORTC = 0x4;
        delay();
        PORTA = 0x1; PORTC = 0x2;
        delay();
         }while(i++ < 1);

        
        // …¡À∏
        i = 0;
        do{
        PORTA = 0xff; PORTC = 0xff;
        delay();
        PORTA = 0x0; PORTC = 0x0;
        delay();
        }while(i++ < 2);
    
    }   
}