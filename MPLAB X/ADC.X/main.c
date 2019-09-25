#include "fosc.h"
#include "macro.h"
#include "port.h"
#include "ds18b20.h"
#include "ledtube.h"
#include "ledx.h"
#include "adc.h"

#include "config.h"

unsigned char ds[4] = {0,0,0,0};

void __interrupt() ISR() {

    unsigned char static i = 1;

    if(TMR0IF) {
        
        if(i > 4) i = 1;

        switch(i) {
        case 1: 
                display_2seg_hc(ds[0], 0, i);
                break; 
        case 2: 
                display_2seg_hc(ds[1], 1, i);
                break;
        case 3: 
                display_2seg_hc(ds[2], 0, i);
                break;
        case 4: 
                display_2seg_hc(ds[3], 0, i);
                break;
        }   

        i++;
        TMR0IF = 0;
        TMR0 = 210;
    }  
}

void TIME0_Init() {
    
    OPTION_REG = 0B10000111;      //预分频 
   
    GIE = 1;
    PEIE = 1;
    TMR0IE = 1;             // 定时器 
    TMR0IF = 0;

    TMR0 = 210;
}

void main(void) {

    FOSC_Init();
    PORT_Init();
    TIME0_Init();
    
    TRA0 = 1;
    ANSA0 = 1;
    ADCON0 = 0B00000000;            // 通道 AN0 
    ADCON1 = 0B11000000;            // 右对齐 FOSC/4 电源电压为参考电压
      
    unsigned int i = 0;
    unsigned int t = 0;
    
    while(1) {
        
///        FVREN = 1;
 //       while(!FVRRDY);
        
        ADON = 1;
        __delay_ms(2);
        
        i = ADC_Read();
        
       if(i >= 524) {//判断输入电压是否小于2V
//           LEDx(1);
        }
        
 //      FVREN = 0;                 // 节能

        t = i;//104800 / i;         // (1.024/v)*1024=524    
                                // 1.024/v = x/1024
        GIE = 0; 
        // 4位数码管 
        ds[0] = (t / 1000) % 10;        // 千位
        ds[1] = (t / 100) % 10;         // 百位
        ds[2] = (t / 10) % 10;          // 十位   
        ds[3] = t % 10;                 // 个位     
        GIE = 1;  
        __delay_ms(200);
    }
}
 