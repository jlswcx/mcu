#include "header.h"
#include "ds18b20.h"

// PIC16F1823 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTOSC       // Oscillator Selection (ECH, External Clock, High Power Mode (4-32 MHz): device clock supplied to CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT enabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = OFF        // Internal/External Switchover (Internal/External Switchover mode is enabled)
#pragma config FCMEN = OFF       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)
//
//// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN = OFF       // PLL Enable (4x PLL enabled)
#pragma config STVREN = OFF      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = OFF         // Low-Voltage Programming Enable (Low-voltage programming enabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

char ds_temp = 0;
char glo_i = 0;

// 初始化内部时钟源
void FOSC_Init() {
    OSCCON  = 0x78;      
}

void PORT_Init() {
    PORTA   = 0x00;
    PORTC   = 0x00;  
    LATA    = 0x00;
    LATC    = 0x00;
    ANSELA  = 0x00;
    ANSELC  = 0x00;
    TRISA   = 0x00;
    TRISC   = 0x00;  
}

void TIME0_Init() {
    GIE = 1;
    PEIE = 1;
    TMR0IE = 1;             // 定时器
    
    OPTION_REG = 0x85;      //预分频为8
    
    TMR0IF = 0;
    TMR0 = 100;
}

char bit_swnp(char s) {
	char t, l, f;
	t = 0B00000001;
	f = 0B00000000;
	for(signed char i = 7; i >= 0; i--){
		l = s & t;				// 取最右位
		l = l << i;				// 左移7位
		s = s >> 1;
		f = f | l;
	};
	return f;
}

void show_seg(char num, char seg){
    char s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, sd, se;
    
    s0 = 0B11000000;    // 0B00111111;      // 0
    s1 = 0B11111001;    // 0B00000110;      // 1
    s2 = 0B10100100;    // 0B01011011;      // 2
    s3 = 0B10110000;    // 0B01001111;      // 3
    s4 = 0B10011001;    // 0B01100110;      // 4
    s5 = 0B10010010;    // 0B01101101;      // 5
    s6 = 0B10000010;    // 0B01111101;      // 6
    s7 = 0B11111000;    // 0B00000111;      // 7
    s8 = 0B10000000;    // 0B01111111;      // 8
    s9 = 0B10010000;    // 0B01101111;      // 9
    sd = 0B01111111;    // 0B10000000;      // 点
    se = 0B00000110;    // E

    // 段选
    switch(num) {
        case 0: PORTC = TRISC = s0; RA0 = TRISA0 = 1; RA1 = TRISA1 = 1; break;
        case 1: PORTC = TRISC = s1; RA0 = TRISA0 = 1; RA1 = TRISA1 = 1; break;
        case 2: PORTC = TRISC = s2; RA0 = TRISA0 = 0; RA1 = TRISA1 = 1; break;
        case 3: PORTC = TRISC = s3; RA0 = TRISA0 = 0; RA1 = TRISA1 = 1; break;
        case 4: PORTC = TRISC = s4; RA0 = TRISA0 = 0; RA1 = TRISA1 = 1; break;
        case 5: PORTC = TRISC = s5; RA0 = TRISA0 = 0; RA1 = TRISA1 = 1; break;
        case 6: PORTC = TRISC = s6; RA0 = TRISA0 = 0; RA1 = TRISA1 = 1; break;
        case 7: PORTC = TRISC = s7; RA0 = TRISA0 = 1; RA1 = TRISA1 = 1; break;
        case 8: PORTC = TRISC = s8; RA0 = TRISA0 = 0; RA1 = TRISA1 = 1; break;
        case 9: PORTC = TRISC = s9; RA0 = TRISA0 = 0; RA1 = TRISA1 = 1; break;
        
        case 'E': PORTC = TRISC = se; RA0 = TRISA0 = 0; RA1 = TRISA1 = 1; break;
        default:  PORTC = TRISC = sd; RA0 = TRISA0 = 1; RA1 = TRISA1 = 0; break;
    }
    
    // 位选
    switch(seg) {
        case 2: TRISA5 = 0; RA5 = 1; TRISA4 = 1; RA4 = 0; break;
        case 1: TRISA4 = 0; RA4 = 1; TRISA5 = 1; RA5 = 0; break;
    }   
    
    __delay_ms(8);
    
    // 关闭位选
    TRISA4 = TRISA5 = 1;
}

void show_res(char num) {
    char t[3];
    t[0] = num / 100;               // 百位
    t[1] = (num / 10) % 10;         // 十位
    t[2] = num % 10;                // 个位

//    show_seg(t[0], 2);
//    __delay_ms(10);
    show_seg(t[1], 2);
    show_seg(t[2], 1);
}

void __interrupt() isr() {
    if(TMR0IF) {
        if(glo_i) {
            char t[3];
            t[0] = ds_temp / 100;               // 百位
            t[1] = (ds_temp / 10) % 10;         // 十位
            t[2] = ds_temp % 10;                // 个位

            show_seg(t[1], 2);
            show_seg(t[2], 1);
        }
        TMR0IF = 0;
        TMR0 = 100;
    }
}

void main() {
  
    FOSC_Init();        // 振荡器设置
    PORT_Init();        // 初始化端口
//    TIME0_Init();

    while(1) {
        glo_i = 0;
        DS_Convert();
        ds_temp = DS_GetTemp();
        glo_i = 1;
        show_res(ds_temp);
//        __delay_ms(200);
    }   
}