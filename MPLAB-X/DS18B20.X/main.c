#include "config.h"

#include "port.h"
#include "ds18b20.h"


char ds_temp = 0;
char glo_i = 0;


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
  
    OSCCON  = OSCCON_4M;  
    
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