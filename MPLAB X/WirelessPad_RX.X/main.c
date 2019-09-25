#include "fosc.h"
#include "port.h"
#include "nrf24l01.h"
#include "ledx.h"
#include "eusart.h"

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

/* RC3 控制蜂鸣器 */
#define     BEEP_DIR        TRISC3      
#define     BEEP_IO         RC3


/* NRF发送的5种数据类型 */
#define     PAD_AUX         0    
#define     PAD_CHAIR       1
#define     PAD_NCI         2
#define     PAD_POTTY       3
#define     PAD_BED         4
#define     PAD_TYPE        0

/* 定义发送接收模式 0 发射 1接收*/
#define     PAD_TX          0
#define     PAD_RX          1

unsigned char const tx_pad_aux[2]    = {0x66, 0x88};
unsigned char const tx_pad_chair[2]  = {0x68, 0x86};
unsigned char const tx_pad_nci[2]    = {0x86, 0x66};
unsigned char const tx_pad_potty[2]  = {0x88, 0x68};
unsigned char const tx_pad_bed[2]    = {0x98, 0x58};

unsigned char const pad_aux_ok[2]   = {0x01,0x01};  // 正常连接
unsigned char const pad_aux_on[2]   = {0x02,0x02};  // 按下按键
unsigned char const pad_aux_off[2]  = {0x03,0x03};  // 松开按键

unsigned char pad_aux_dat[2]  = {0x00, 0x00};       // 当前链接数据缓存

unsigned char PAD_MODE = PAD_TX;                    // 0发射 1接收

unsigned char tx_addr[5] = {0x0F, 0x0E, 0x0D, 0x0C, 0x0B};       // 设置发送接收地址

unsigned char rx_addr_0[5] = {0x0F, 0x0E, 0x0D, 0x0C, 0x0B};     // 通道0接收地址
unsigned char rx_addr_1[5] = {0x66, 0x8E, 0x29, 0xF3, 0xA5};     // 通道1接收地址
unsigned char rx_addr_2[5] = {0x99, 0x8E, 0x29, 0xF3, 0xA5};     // 通道2接收地址
unsigned char rx_addr_3[5] = {0x33, 0x8E, 0x29, 0xF3, 0xA5};     // 通道3接收地址
unsigned char rx_addr_4[5] = {0x88, 0x8E, 0x29, 0xF3, 0xA5};     // 通道4接收地址
unsigned char rx_addr_5[5] = {0x55, 0x8E, 0x29, 0xF3, 0xA5};     // 通道5接收地址


void WirelessPad(void);                           // 无线通讯测试

void BEEP_Init(void);
void BEEPx(unsigned char num);

void ADC_Init(void);                               // 模数转换
unsigned int ADC_Read(void);
unsigned int ADC_Read_N(void);



void BEEP_Init(void) {
    BEEP_DIR = 0;
    BEEP_IO = 1;
}

void BEEPx(unsigned char num) {
    while(num--) {
        BEEP_IO = 0; 
        __delay_ms(200);
        BEEP_IO = 1;
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

/**
 * 中断服务程序，所有 中断都是这个入口
 */
void __interrupt() ISR() {
    
    if(IOCIF && IOCAF4) {        // 导通按键 RA4
        IOCIE = 0;
        IOCIF = 0;
        IOCAF4 = 0;

        if(RA4 == 1) {          
//            LEDx(1);
//            NRF_TxPacket(pad_aux_dat);  
        }
        
        IOCIE = 1;
    }
}

void main() {

    FOSC_Init();
    
    PORT_Init();
    
    LED_Init();
    
    BEEP_Init();       
    
    ADC_Init();
    
    EUSART_Init();
    
    /********* 逻辑代码区 Start **********/
    
    WirelessPad();
    
    /********* 逻辑代码区 End ************/
    
    while(1);                   // 如果开启看门狗 WDTE，此处会被复位
}

void WirelessPad() {
    
    unsigned char ledx; 
    unsigned char rxbuf[2] ={0, 0};         // 接收数据缓存
    
    
    
    NRF_On();
    NRF_Port_Init();
    NRF_SPI_Init();
   
    __delay_ms(500);
    
    if(NRF_Check()) {
        while(1) {
            LEDx(1);
        }
    }
  
    NRF_Rx_Set(NRF_NO_ACK, NRF_P0_ACK);
    NRF_Rx_Set_P0(rx_addr_0, 1);
    NRF_Rx_Receive();   
    
    while(1) {
        
        unsigned int i = 0;
        unsigned char adc = 0;
        
        rxbuf[0] = 0;
        ledx = NRF_RxPacket(rxbuf);
        NRF_Clear_IRQ();

        LEDx(1);
        
        EUSART_Send(rxbuf[0]);
        
//        if(rxbuf[0] == pad_aux_ok[0]) {
//            BEEP_IO = 0;
//            __delay_ms(100);  
//        }else {
//            BEEP_IO = 1;
//            adc = 1;
//        }
//
//        if(adc == 1) {
//            FVREN = 1;      // FVR 使能
//            while(!FVRRDY); // FVR 是否稳定
//
//            ADON = 1;
//            __delay_ms(2);
//            
//            i = ADC_Read();
//
//            if(i >= 524) {//判断输入电压是否小于2V
//                LEDx(1);
//            }
//
//            FVREN = 0;      // FVR 使能关闭，节能
////            ds_temp = 104800 / i;                    
//        }
        
        __delay_ms(1000);
    }    
}
