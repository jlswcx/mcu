#include "fosc.h"
#include "macro.h"
#include "port.h"
#include "nrf24l01.h"
#include "ds18b20.h"
#include "ledx.h"

// PIC16F1823 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF        // Watchdog Timer Enable (WDT enabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = ON        // Internal/External Switchover (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN = ON       // PLL Enable (4x PLL enabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>


/* NRF发送的5种数据类型 */
#define     PAD_AUX         0    
#define     PAD_CHAIR       1
#define     PAD_NCI         2
#define     PAD_POTTY       3
#define     PAD_BED         4
#define     PAD_TYPE        0

/* 按键中断 */
#define     KEY_DIR          TRISA4                 // 按键中断引脚
#define     KEY_UP           IOCAP4                 // 上升沿
#define     KEY_DOWN         IOCAN4                 // 下降沿
#define     KEY_FLAG         IOCAF4                 // 标志位
#define     KEY_IO           RA4                    // 电平      


unsigned char const tx_pad_aux[2]    = {0x66, 0x88};
unsigned char const tx_pad_chair[2]  = {0x68, 0x86};
unsigned char const tx_pad_nci[2]    = {0x86, 0x66};
unsigned char const tx_pad_potty[2]  = {0x88, 0x68};
unsigned char const tx_pad_bed[2]    = {0x98, 0x58};


unsigned char const pad_aux_ok[2]   = {0x01,0x01};  // 正常连接
unsigned char const pad_aux_on[2]   = {0x02,0x02};  // 按下按键
unsigned char const pad_aux_off[2]  = {0x03,0x03};  // 松开按键

unsigned char pad_aux_dat[2]  = {0x00, 0x00};       // 当前连接
unsigned char pad_dat[2]  = {0x00, 0x00};           // 当前连接

unsigned char tx_addr[5] = {0x0F, 0x0E, 0x0D, 0x0C, 0x0B};     //设置发送接收地址



void WirelessPad(void);                           // 无线通讯测试
 

void Key_Init(void) {
    
    KEY_DIR = 1;                                    // 设置按键引脚为输入
    
}

/**
 * 中断服务程序，所有 中断都是这个入口
 */
void __interrupt() ISR() {
    
    if(IOCIF && IOCAF4) {        // 导通按键 RA4
        IOCIE = 0;
        IOCIF = 0;
        IOCAF4 = 0;

        if(RA4 == 1) {

        }
        
        IOCIE = 1;
    }
}

void main() {

    FOSC_Init();
    
    PORT_Init();
    
    LED_Init();
    
//    Key_Init();
    
    /********* 逻辑代码区 Start **********/
    
    WirelessPad();
    
    /********* 逻辑代码区 End ************/
    
    while(1);                   // 如果开启看门狗 WDTE，此处会被复位
}

// 按键触发发送
void Send_Key(void) {
    
    memcpy(pad_aux_dat, pad_aux_ok, 2);
    
    while(1) {
        if(RA4 == 1) {
            LEDs(LED_ON);
            // 注意：如果设置了自动应答，则可能一直循环，自动重发
            NRF_Tx_Send(pad_aux_dat, 2);  
            __delay_ms(1);

        }else {
            LEDs(LED_OFF);
            __delay_ms(1);        // 5秒发送一次连接情况
        }   
    }  
 
}

// 发送温度数据
void Send_DS(void) {
    
    uint temp = 0;
    
    while(1) {
    
        DS_Convert();              
        temp = DS_GetTemp();  

        pad_dat[0] = (char)temp;

        NRF_Tx_Send(pad_dat, 1);  
        
        LEDx(1);
        
        __delay_ms(1);
    }

}

void WirelessPad() {
    
    NRF_On();
    NRF_Port_Init();
    NRF_SPI_Init();
 
    __delay_ms(500);
 
    if(NRF_Check()) {
        while(1) {
            LEDx(1);
        }
    }

    NRF_Tx_Set(tx_addr, 0x00);  
    
    Send_DS();
    
}


