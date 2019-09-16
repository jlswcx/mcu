#include "macro.h"
#include "port.h"

#define     _XTAL_FREQ        _XTAL_FREQ_4M       // 当前工程使用频率定义

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

#define     SEG_A_DIR       TRISC0
#define     SEG_A_IO        LATC0
#define     SEG_B_DIR       TRISC1
#define     SEG_B_IO        LATC1
#define     SEG_C_DIR       TRISC2
#define     SEG_C_IO        LATC2
#define     SEG_D_DIR       TRISC3
#define     SEG_D_IO        LATC3
#define     SEG_E_DIR       TRISC4
#define     SEG_E_IO        LATC4   
#define     SEG_F_DIR       TRISC5
#define     SEG_F_IO        LATC5
#define     SEG_G_DIR       TRISA0
#define     SEG_G_IO        LATA0    
#define     SEG_P_DIR       TRISA1
#define     SEG_P_IO        LATA1

#define     SEG_1_DIR       TRISA3
#define     SEG_1_IO        RA3
#define     SEG_2_DIR       TRISA4
#define     SEG_2_IO        LATA4
#define     SEG_3_DIR       TRISA5
#define     SEG_3_IO        LATA5
#define     SEG_4_DIR       TRISA2
#define     SEG_4_IO        LATA2

#define  LED_DIR        TRISA4
#define  LED_IO         LATA4

unsigned char ds[4] = {0,0,0,0};

void LED_Init(void) {
    LED_DIR = 0;
    LED_IO = 1;
}

void LEDx(unsigned char num) {
    while(num--) {
        LED_IO = 0; 
        __delay_ms(200);
        LED_IO = 1;
        __delay_ms(200);
    }
}

void ADC_Init(void) {
    FVRCON = 0B01010001;        // FVREN,ADCFVR=01(1.024V)
    ADCON1 = 0B10000000;        // 右对齐，AD时钟为Fosc/32,参考电压为电源电压，
    ADCON0 = 0B01111100;        // 选择通道AN0 
}

unsigned int ADC_Read(void) {       //转换一次
    unsigned int value = 0;
 //   ADCON0bits.CHS =0;            // 选择通道AN0
    ADCON0bits.GO = 1;              // 开始转换
    while(ADCON0bits.GO == 1);      // 等待转换结束
    value = (unsigned int)ADRESH;   // 强制类型转换，因为ADRESH是字符型的只能表示8位二进制。所以必须转换成可以容纳10位二进制的整型。
    value = value<<8;               // 将高两位左移8位
    value += ADRESL;                // 低八位加入ADRESL的值。
    return value;
}

unsigned int ADC_Read_N(void){

    unsigned char num = 8;          // 读取次数
    unsigned short total = 0;
    unsigned short average = 0;
    unsigned short adv[8];
    unsigned short i = 0;
    
    for(i = 0;i < num; i++) {       //进行多次AD转换
        adv[i] = ADC_Read();
    }

    for(i = 0;i < num; i++) {       //计算多次AD转换的平均值
      total += adv[i];
    }
    
    average = total / num;
    return average;                 //得到结果返回
}

inline void  show_seg(char num, char dp, char seg){

    // 关闭
    switch(seg) {
        case 4: 
                SEG_1_DIR = SEG_2_DIR = SEG_3_DIR = 1; 
                SEG_1_IO = SEG_2_IO = SEG_3_IO = 0; 
//                SEG_4_DIR = 0; 
//                SEG_4_IO = 1; 
                break;
        case 3: 
                SEG_1_DIR = SEG_2_DIR = SEG_4_DIR = 1; 
                SEG_1_IO = SEG_2_IO = SEG_4_IO = 0; 
//                SEG_3_DIR = 0; 
//                SEG_3_IO = 1; 
                break;
        case 2: 
                SEG_1_DIR = SEG_3_DIR = SEG_4_DIR = 1; 
                SEG_1_IO = SEG_3_IO = SEG_4_IO = 0; 
//                SEG_2_DIR = 0; 
//                SEG_2_IO = 1; 
                break;
        case 1: 
                SEG_2_DIR = SEG_3_DIR = SEG_4_DIR = 1; 
                SEG_2_IO = SEG_3_IO = SEG_4_IO = 0; 
//                SEG_1_DIR = 0; 
//                SEG_1_IO = 1; 
                break;
    }   
    
    // 段选
    switch(num) {
        case 0: SEG_A_DIR = SEG_B_DIR = SEG_C_DIR = SEG_D_DIR = SEG_E_DIR = SEG_F_DIR = 0; 
                SEG_A_IO = SEG_B_IO = SEG_C_IO = SEG_D_IO = SEG_E_IO = SEG_F_IO = 0; 
                SEG_G_DIR = SEG_P_DIR = 1; 
                SEG_G_IO = SEG_P_IO = 0;
                break;
        case 1: SEG_B_DIR = SEG_C_DIR = 0;
                SEG_B_IO = SEG_C_IO = 0;
                SEG_A_DIR = SEG_D_DIR = SEG_E_DIR = SEG_F_DIR = SEG_G_DIR = SEG_P_DIR = 1; 
                SEG_A_IO = SEG_D_IO = SEG_E_IO = SEG_F_IO = SEG_G_IO = SEG_P_IO = 0; 
                break;
        case 2: SEG_A_DIR = SEG_B_DIR = SEG_G_DIR = SEG_E_DIR = SEG_D_DIR = 0; 
                SEG_A_IO = SEG_B_IO = SEG_G_IO = SEG_E_IO = SEG_D_IO = 0; 
                SEG_C_DIR = SEG_F_DIR = SEG_P_DIR = 1;
                SEG_C_IO = SEG_F_IO = SEG_P_IO = 0; 
                break;
        case 3: SEG_A_DIR = SEG_B_DIR = SEG_C_DIR = SEG_D_DIR = SEG_G_DIR = 0; 
                SEG_A_IO = SEG_B_IO = SEG_C_IO = SEG_D_IO = SEG_G_IO = 0; 
                SEG_E_DIR = SEG_F_DIR = SEG_P_DIR = 1; 
                SEG_E_IO = SEG_F_IO = SEG_P_IO = 0; 
                break;   
        case 4: SEG_B_DIR = SEG_C_DIR = SEG_F_DIR = SEG_G_DIR = 0; 
                SEG_B_IO = SEG_C_IO = SEG_F_IO = SEG_G_IO = 0; 
                SEG_A_DIR = SEG_D_DIR = SEG_E_DIR = SEG_P_DIR = 1; 
                SEG_A_IO = SEG_D_IO = SEG_E_IO = SEG_P_IO = 0;
                break;
        case 5: SEG_A_DIR = SEG_C_DIR = SEG_D_DIR = SEG_F_DIR = SEG_G_DIR = 0; 
                SEG_A_IO = SEG_C_IO = SEG_D_IO = SEG_F_IO = SEG_G_IO = 0; 
                SEG_B_DIR = SEG_E_DIR = SEG_P_DIR = 1; 
                SEG_B_IO = SEG_E_IO = SEG_P_IO = 0; 
                break;    
        case 6: SEG_A_DIR = SEG_C_DIR = SEG_D_DIR = SEG_E_DIR = SEG_F_DIR = SEG_G_DIR = 0; 
                SEG_A_IO = SEG_C_IO = SEG_D_IO = SEG_E_IO = SEG_F_IO = SEG_G_IO = 0; 
                SEG_B_DIR = SEG_P_DIR = 1; 
                SEG_B_IO = SEG_P_IO = 0;
                break;
        case 7: SEG_A_DIR = SEG_B_DIR = SEG_C_DIR = 0; 
                SEG_A_IO = SEG_B_IO = SEG_C_IO = 0; 
                SEG_D_DIR = SEG_E_DIR = SEG_F_DIR = SEG_G_DIR = SEG_P_DIR = 1;
                SEG_D_IO = SEG_E_IO = SEG_F_IO = SEG_G_IO = SEG_P_IO = 0;
                break; 
        case 8: SEG_A_DIR = SEG_B_DIR = SEG_C_DIR = SEG_D_DIR = SEG_E_DIR = SEG_F_DIR = SEG_G_DIR = 0; 
                SEG_A_IO = SEG_B_IO = SEG_C_IO = SEG_D_IO = SEG_E_IO = SEG_F_IO = SEG_G_IO = 0; 
                break;
        case 9: SEG_A_DIR = SEG_B_DIR = SEG_C_DIR = SEG_D_DIR = SEG_F_DIR = SEG_G_DIR = 0; 
                SEG_A_IO = SEG_B_IO = SEG_C_IO = SEG_D_IO = SEG_F_IO = SEG_G_IO = 0; 
                SEG_E_DIR = SEG_P_DIR = 1; 
                SEG_E_IO = SEG_P_IO = 0;
                break;

        default: ;
        case 'E': SEG_A_DIR = SEG_D_DIR = SEG_E_DIR = SEG_F_DIR = SEG_G_DIR = 0; 
                  SEG_A_IO = SEG_D_IO = SEG_E_IO = SEG_F_IO = SEG_G_IO = 0; 
                  SEG_B_DIR = SEG_C_DIR = SEG_P_IO = 1; 
                  SEG_B_IO = SEG_C_IO = SEG_P_IO =  0; 
                  break;
    }
    
    // 小数点
    if(dp) {
        SEG_P_DIR = 0;
        SEG_P_IO = 0;  
    }else {
        SEG_P_DIR = 1;
        SEG_P_IO = 0; 
    }

    // 位选
    switch(seg) {
        case 4: 
                SEG_4_DIR = 0; 
                SEG_4_IO = 1; 
                break;
        case 3: 
                SEG_3_DIR = 0; 
                SEG_3_IO = 1; 
                break;
        case 2: 
                SEG_2_DIR = 0; 
                SEG_2_IO = 1; 
                break;
        case 1: 
                SEG_1_DIR = 0; 
                SEG_1_IO = 1; 
                break;
    }   
    
}

void __interrupt() ISR() {

    unsigned char static i = 1;

    if(TMR0IF) {
        
        if(i > 4) i = 1;

        switch(i) {
        case 1: 
                if(ds[0] != 0) {
                    show_seg(ds[0], 0, i);
                }
                break;
        case 2: 
            
                show_seg(ds[1], 1, i);
                break;
        case 3: 
                show_seg(ds[2], 0, i);
                break;
        case 4: 
                show_seg(ds[3], 0, i);
                break;
        }   

        i++;
        TMR0IF = 0;
        TMR0 = 55;
    }  
}

void TIME0_Init() {
    
    OPTION_REG = 0B10000100;      //预分频 
   
    GIE = 1;
    PEIE = 1;
    TMR0IE = 1;             // 定时器 
    TMR0IF = 0;

    TMR0 = 55;
}

void main(void) {

    OSCCON = OSCCON_4M;
    
    PORT_Init();
    
    LED_Init();
    
    ADC_Init();
    
    TIME0_Init();
      
    unsigned int i = 0;
    unsigned int t = 0;
    
    while(1) {
        
        FVREN = 1;
        while(!FVRRDY);
        
        ADON = 1;
        __delay_ms(2);
        
        i = ADC_Read_N();
        
       if(i >= 524) {//判断输入电压是否小于2V
           LEDx(1);
        }
        
        FVREN = 0;

        t = i;//104800 / i;         // （1.024/v)*1024=524    
                                // 1.024/v = x/1024
        
        // 4位数码管 
        memset(ds, 0, 4);
        ds[0] = (t / 1000) % 10;        // 千位
        ds[1] = (t / 100) % 10;          // 百位
        ds[2] = (t / 10) % 10;         // 十位   
        ds[3] = t % 10;                // 个位     
          
        __delay_ms(2000);
    }
}
 