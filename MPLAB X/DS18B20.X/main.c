#include "fosc.h"
#include "macro.h"
#include "port.h"
#include "ds18b20.h"
#include "ledtube.h"

#include "config.h"


unsigned char ds[4] = {0,0,0,0};

void TIME0_Init() {
    GIE = 1;
    PEIE = 1;
    TMR0IE = 1;                    
    
    OPTION_REG = 0B10000111;      //预分频为 256
    
    TMR0IF = 0;
    TMR0 = 210;
}

void __interrupt() isr() {
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
        TMR0 = 210;
        TMR0IF = 0;
    }  
}

void main() {
  
    FOSC_Init();
    PORT_Init();    
    TIME0_Init();

    unsigned int temp = 0;
    
    while(1) {
        
        DS_Convert();
        GIE = 0; 
        temp = DS_GetTemp();
        
                                   // 防止数写到一半被中断调用
        // 4位数码管 
        ds[0] = (temp / 1000) % 10;             // 千位
        ds[1] = (temp / 100) % 10;              // 百位
        ds[2] = (temp / 10) % 10;;              // 十位   
        ds[3] = temp % 10;;                     // 个位   
        GIE = 1;
        
        __delay_ms(200);
    }   
    
}