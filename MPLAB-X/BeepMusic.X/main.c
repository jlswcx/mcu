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


// 蜂鸣器控制IO 
#define     BEEP_DIR    TRISC3
#define     BEEP_IO     LATC3

// 音阶频率，超低音，低音，标音，高音，超高音
unsigned int const note_ll[7] = {65, 73, 82, 87, 98, 110, 123};
unsigned int const note_l[7] = {131, 147, 165, 174, 196, 220, 247};
unsigned int const note_m[7] = {262, 294, 330, 349, 392, 440, 494};
unsigned int const note_h[7] = {523, 587, 659, 698, 784, 880, 988};
unsigned int const note_hh[7] = {1046, 1175, 1318, 1397, 1568, 1760, 1967};

//拍速
unsigned int const speed = 120;

float ds_temp = 0;

// 音乐格式，3个字节一组，字节1表示音阶，字节2表示音高，字节3表示几分音符
/*
 * 字节1:1?7 代表 Do、Re、Mi、Fa、So、La、Si，0 代表空音停顿 
 * 字节2： 1：超低音
 *         2：低音
 *         3：标音
 *         4：高音
 *         5：超高音
 * 字节3：代表几分音符，1/4 音符，就写4，1/8 音符，就写8,1/16 音符，就写 16
 */

// 把孤独当做晚餐
//unsigned char const bgddzwc[]={\
0,3,4, 0,3,8, 3,3,8, 3,3,8, 2,3,8, 2,3,8, 1,3,8, \
1,3,4, 6,2,8, 6,2,8, 2,3,8, 3,3,8, 5,3,8, 3,3,8, \
3,3,4, 3,3,8, 3,3,8, 3,3,8, 2,3,8, 2,3,8, 1,3,8, \
1,3,4, 6,2,8, 6,2,8, 1,3,8, 2,3,8, 7,3,8, 5,3,8, \
5,3,4, 5,3,8, 3,3,8, 3,3,8, 2,3,8, 2,3,8, 1,3,8, \
\
6,3,4, 6,3,4, 2,3,4, 2,3,8, 3,3,8, \
5,3,4, 7,3,8, 1,4,8, 1,4,4, 3,3,8, 2,3,8, \
6,2,4, 3,3,4, 2,3,8, 3,3,8, 5,2,8, 6,2,8, \
6,2,4, 6,2,4, 6,2,4, 6,2,4, \
0,3,4, 0,3,8, 3,3,8, 3,3,8, 2,3,8, 2,3,8, 1,3,8, \
\
1,3,4, 6,2,8, 6,2,8, 2,3,8, 3,3,8, 5,3,8, 3,3,8, \
3,3,4, 3,3,8, 3,3,8, 3,3,8, 2,3,8, 2,3,8, 1,3,8, \
1,3,4, 6,2,8, 6,2,8, 1,3,8, 2,3,8, 7,3,8, 5,3,8, \
5,3,4, 5,3,8, 3,3,8, 3,3,8, 2,3,8, 2,3,8, 1,3,8, \
6,3,4, 6,3,4, 2,3,4, 2,3,8, 2,3,8, \
\
5,3,4, 7,3,8, 1,4,8, 1,4,4, 3,3,8, 2,3,8, \
6,2,4, 3,3,4, 2,3,8, 3,3,8, 5,2,8, 6,2,8, \
6,2,4, 6,2,4, 6,2,4, 6,2,4, \
0,3,4, 0,3,8, 3,3,8, 3,3,8, 2,3,8, 2,3,8, 1,3,8, \
1,3,4, 6,2,8, 6,2,8, 2,3,8, 3,3,8, 5,3,8, 3,3,8, \
\
3,3,4, 3,3,8, 3,3,8, 3,3,8, 2,3,8, 2,3,8, 1,3,8, \
1,3,4, 6,2,8, 6,2,8, 1,3,8, 2,3,8, 7,3,8, 5,3,8, \
5,3,4, 5,3,8, 3,3,8, 3,3,8, 2,3,8, 2,3,8, 1,3,8, \
1,3,4, 6,2,4, 2,3,4, 1,3,8, 6,2,8, \
6,2,4, 3,3,4, 3,3,4, 2,3,8, 1,3,8, \
\
2,3,4, 3,3,4, 2,3,8, 3,3,8, 5,2,8, 6,2,8, \
6,2,4, 6,2,4, 6,2,4, 6,2,4, \
0,3,4, 0,3,8, 3,3,8, 3,3,8, 2,3,8, 2,3,8, 1,3,8, \
1,3,4, 6,2,8, 6,2,8, 2,3,8, 3,3,8, 5,3,8, 3,3,8, \
3,3,4, 3,3,8, 3,3,8, 3,3,8, 2,3,8, 2,3,8, 1,3,8, \
\
1,3,4, 6,2,8, 6,2,8, 1,3,8, 2,3,8, 7,3,8, 5,3,8, \
5,3,4, 5,3,8, 3,3,8, 3,3,8, 2,3,8, 2,3,8, 1,3,8, \
6,3,4, 6,3,4, 2,3,4, 2,3,8, 2,3,8, \
5,3,4, 7,3,8, 1,4,8, 1,4,4, 3,3,8, 2,3,8, \
6,2,4, 3,3,4, 2,3,8, 3,3,8, 5,2,8, 6,2,8, \
\
6,2,4, 0,3,8, 3,3,8, 3,3,8, 2,3,8, 2,3,8, 1,3,8, \
1,3,4, 6,2,8, 6,2,8, 2,3,8, 3,3,8, 5,3,8, 3,3,8, \
3,3,4, 3,3,8, 3,3,8, 3,3,8, 2,3,8, 2,3,8, 1,3,8, \
1,3,4, 6,2,8, 6,2,8, 1,3,8, 2,3,8, 7,3,8, 5,3,8, \
\
5,3,4, 5,3,8, 3,3,8, 3,3,8, 2,3,8, 2,3,8, 1,3,8, \
1,3,4, 6,2,4, 2,3,4, 1,3,8, 6,2,8, \
6,2,4, 3,3,4, 3,3,4, 2,3,8, 1,3,8, \
2,3,4, 3,3,4, 2,3,8, 3,3,8, 5,2,8, 6,2,8, \
6,2,4, 0,3,8, 3,3,8, 3,3,8, 2,3,8, 2,3,8, 1,3,8, \
\
1,3,4, 6,2,8, 6,2,8, 2,3,8, 3,3,8, 5,3,8, 3,3,8, \
3,3,4, 3,3,8, 3,3,8, 3,3,8, 2,3,8, 2,3,8, 1,3,8, \
1,3,4, 6,2,8, 6,2,8, 1,3,8, 2,3,8, 7,3,8, 5,3,8, \
5,3,4, 5,3,8, 3,3,8, 3,3,8, 2,3,8, 2,3,8, 1,3,8, \
6,3,4, 6,3,4, 2,3,4, 2,3,8, 3,3,8, \
5,3,4, 7,3,8, 1,4,8, 1,4,4, 3,3,8, 2,3,8, \
\
0,0,0 };

// 缘分一道桥
//unsigned char const yfydq[]={\
1,4,4,  6,3,8, 5,3,8, 6,3,4, 6,3,4, \
5,3,4, 5,3,8, 3,3,8, 5,3,4, 5,3,4, \
3,3,4, 5,3,4, 6,3,4, 1,4,4, \
2,4,8, 1,4,8, 6,3,8, 5,3,8, 6,3,4, 6,3,4, \
6,3,4, 1,4,4, 2,4,4, 3,4,4, \
\
5,4,8, 3,4,8, 5,4,4, 6,4,4, 6,4,4, \
2,4,16, 2,4,8, 2,4,16, 2,4,4, 6,3,8, 1,4,8, 3,3,8, 2,3,8, \
3,3,4, 5,3,8, 6,3,8, 6,3,4, 6,3,4, \
3,3,8, 2,3,8, 3,3,4, 2,3,8, 1,3,4, 1,3,8, \
5,2,4, 5,2,8, 7,2,8, 6,2,4, 6,2,4, \
\
3,3,8, 2,3,8, 3,3,8, 3,3,4, 3,3,8, 3,3,4, \
6,3,4, 3,3,16, 2,3,16, 3,3,8, 3,3,4, 3,3,4, \
3,3,4, 5,3,8, 6,3,8, 6,3,4, 3,3,4, \
7,3,4, 7,3,8, 3,3,16, 2,3,16, 3,3,4, 3,3,4, \
1,4,16, 1,4,8, 1,4,16, 1,4,4, 3,3,8, 5,3,8, 2,3,8, 1,3,8, \
\
2,3,8, 3,3,8, 1,3,4, 6,2,4, 5,2,8, 6,2,8, \
1,3,4, 6,2,8, 1,3,8, 2,3,4, 1,3,4, \
3,3,8, 5,3,8, 6,3,8, 5,3,8, 3,3,4, 6,3,8, 5,3,8, \
3,3,4, 5,3,8, 3,3,8, 2,3,8, 1,3,8, 1,3,8, 3,3,8, \
2,3,4, 2,3,4, 2,3,4, 5,2,8, 6,2,8, \
\
1,3,4, 6,2,8, 1,3,8, 2,3,4, 1,3,4, \
3,3,8, 5,3,8, 6,3,8, 7,3,8, 7,3,4, 6,3,8, 5,3,8, \
3,3,4, 5,3,8, 3,3,8, 2,3,8, 1,3,8, 1,3,8, 2,3,8, \
3,3,8, 3,3,8, 2,3,8, 2,3,8, 1,3,4, 5,2,8, 6,2,8, \
1,3,4, 6,2,8, 1,3,8, 2,3,4, 1,3,4, \
\
3,3,8, 5,3,8, 6,3,8, 5,3,8, 3,3,4, 6,3,8, 5,3,8, \
3,3,4, 5,3,8, 3,3,8, 2,3,8, 1,3,8, 1,3,8, 3,3,8, \
2,3,4, 2,3,4, 2,3,4, 5,2,8, 6,2,8, \
1,3,4, 6,2,8, 1,3,8, 2,3,4, 1,3,4, \
3,3,8, 5,3,8, 6,3,8, 7,3,8, 7,3,4, 6,3,8, 5,3,8, \
\
3,3,4, 5,3,8, 3,3,8, 2,3,8, 1,3,8, 1,3,8, 2,3,8, \
3,3,4, 3,3,4, 2,3,4, 2,3,4, \
6,2,4, 6,2,4, 6,2,4, 6,2,4, \
\
0,0,0 };

// 来自天堂的魔鬼
unsigned char const lzttdmg_speed = 72;
unsigned char const lzttdmg[]={\
6,2,8, 6,2,8, 6,2,16, 1,3,16, 6,2,16, 1,3,16, 5,3,8, 1,3,8, 1,3,8, 1,3,16, 1,3,16, \
3,3,8, 3,3,8, 3,3,16, 4,3,16, 3,3,16, 4,3,16, 3,3,8, 2,3,8, 2,3,8, 1,3,16, 7,2,16, \
6,2,8, 6,2,8, 6,2,16, 1,3,16, 6,2,16, 1,3,16, 5,3,8, 1,3,8, 1,3,8, 1,3,16, 2,3,16, \
\
3,3,16, 3,3,16, 3,3,8, 3,3,8, 2,3,16, 1,3,16, 7,2,4, 1,3,8, 2,3,8, \
3,3,8, 3,3,8, 3,3,8, 1,4,8, 4,3,8, 4,3,8, 4,3,8, 1,4,8, \
5,3,8, 5,3,8, 5,3,16, 4,3,16, 3,3,16, 4,3,16, 3,3,8, 2,3,8, 2,3,8, 1,3,16, 7,2,16, \
6,2,8, 6,2,8, 6,2,16, 1,3,16, 6,2,16, 1,3,16, 5,3,8, 1,3,8, 1,3,8, 1,3,16, 1,3,16, \
\
3,3,16, 3,3,16, 3,3,8, 3,3,8, 2,3,16, 1,3,16, 7,2,4, 0,3,4,\
0,3,4, 0,3,4, 0,3,8, 0,3,16, 5,3,16, 1,4,16, 7,3,16, 6,3,16, 7,3,16, \
1,4,4, 1,4,8, 6,3,16, 7,3,16, 7,3,4, 7,3,8, 5,3,16, 5,3,16, \
5,3,8, 5,3,16, 5,3,16, 5,3,4, 0,3,8, 0,3,16, 7,3,16, 7,3,16, 7,3,16, 6,3,16, 7,3,16, \
\
1,4,4, 1,4,8, 7,3,16, 7,3,16, 7,3,4, 7,3,8, 5,3,16, 5,3,16, \
0,3,8, 5,3,16, 5,3,16, 0,3,4, 0,3,8, 0,3,16, 1,4,16, 1,4,16, 7,3,16, 6,3,16, 7,3,16, \
1,4,4, 1,4,8, 1,4,16, 2,4,16, 2,4,4, 2,4,8, 1,4,16, 3,4,16, \
3,4,4, 3,4,8, 2,4,16, 7,3,16, 0,3,8, 7,3,16, 7,3,16, 1,4,16, 7,3,16, 6,3,16, 7,3,16, \
\
1,4,8, 1,4,16, 1,4,16, 0,3,8, 7,3,16, 1,4,16, 0,3,8, 7,3,16, 1,4,16, 0,3,8, 5,3,16, 6,3,16, \
0,3,8, 5,3,16, 6,3,16, 0,3,8, 5,3,16, 6,3,16, 0,3,8, 5,3,16, 5,3,16, 5,3,16, 4,3,16, 3,3,16, 2,3,16, \
1,3,4, 1,4,4, 7,3,8, 7,3,16, 5,3,16, 5,3,16, 4,3,16, 3,3,16, 2,3,16, \
\

0,0,0 };

// 定时器初始值存储变量
unsigned char tmr1_con, tmr1_h, tmr1_l;

void Beep_Init() {
    BEEP_DIR = 0;
    BEEP_IO = 0;
}



void TMR1_Init() {
    TMR1H = tmr1_h;                     // 定时器
    TMR1L = tmr1_l; 
    
    TMR1ON = 1;
    TMR1IE = 1 ;
    TMR1IF = 0 ;
    PEIE = 1;
    GIE = 1;
}

void __interrupt() isr(void) {

     if(TMR1IF == 1) { 
        BEEP_IO = !BEEP_IO;
        
        TMR1H = tmr1_h;                     // 定时器
        TMR1L = tmr1_l; 
        
        TMR1IF = 0;
     }
}

void delay(unsigned char t) {
    if(t == 1) {
        __delay_ms(208);
    }else if(t == 2){
        __delay_ms(416);
    }else if(t == 3){
        __delay_ms(600);
    }else if(t == 4){
        __delay_ms(833);
    }else if(t == 5){
        __delay_ms(1000);
    }else if(t == 6){
        __delay_ms(1200);
    }

    
}

void main() {

    OSCCON = OSCCON_4M;       // 初始化晶振
    
    unsigned int i = 0;
    unsigned int t = 0;
    unsigned int n = 0;
    
    PORT_Init();
    
    Beep_Init();
       
    while(1){
        unsigned int audio, tone, duration, tune;       // 音阶，音调，音长，曲调
        unsigned int T, M;
        
        audio = tone = duration = tune = T = M = 0;
        unsigned char const *p = lzttdmg;               // 赋值音乐数组
        
        audio = p[i];
        tone = p[i + 1];
        duration = 16 / p[i + 2]; 
        
        ds_temp = 1048.00 / (float)i;
                
        if(audio == 0 && tone == 0 && duration == 0) {
            i = 0;
        }else {
            i = i + 3;
            
            if(audio == 0) {
                TMR1IE = 0;            // 音阶为0时 空音停顿 停止中断 
                GIE = 0;
                t = 0;
                BEEP_IO = 0;                // 对于无源蜂鸣或喇叭不是必须的
            }else {
                audio = audio - 1;

                if(tone == 1){
                    audio = note_ll[audio];
                }else if(tone == 2) {
                    audio = note_l[audio];
                }else if(tone == 3) {
                    audio = note_m[audio];
                }else if(tone == 4) {
                    audio = note_h[audio];
                }else if(tone == 5) {
                    audio = note_hh[audio];
                }    

                T = T + _XTAL_FREQ_1M / audio / 2;
                M = 0xFFFF - T * (_XTAL_FREQ / (_XTAL_FREQ_1M * 4));   // 定时初始值计算公式 T*（F/N ）    
                
                tmr1_h = M / 256;
                tmr1_l = M % 256;  
                
                if(!t) {                                                                 
                    TMR1_Init();    // 此函数只运行一次
                    t++;
                }
            }
            
            delay(duration);
        }

    };
}