#include "macro.h"
#include "port.h"
#include "nrf24l01.h"

#include "config.h"

/* RC3 控制LED灯 */
#define     LED_DIR         TRISC3          // RC3 引脚控制LED
#define     LED_IO          RC3
#define     LED_ON          0               // 低电平 亮
#define     LED_OFF         1               // 高电平 灭     


/* NRF发送的5种数据类型 */
#define     PAD_AUX         0    
#define     PAD_CHAIR       1
#define     PAD_NCI         2
#define     PAD_POTTY       3
#define     PAD_BED         4
#define     PAD_TYPE        0

unsigned char const tx_pad_aux[2]    = {0x66, 0x88};
unsigned char const tx_pad_chair[2]  = {0x68, 0x86};
unsigned char const tx_pad_nci[2]    = {0x86, 0x66};
unsigned char const tx_pad_potty[2]  = {0x88, 0x68};
unsigned char const tx_pad_bed[2]    = {0x98, 0x58};


unsigned char const pad_aux_ok[2]   = {0x01,0x01};  // 正常连接
unsigned char const pad_aux_on[2]   = {0x02,0x02};  // 按下按键
unsigned char const pad_aux_off[2]  = {0x03,0x03};  // 松开按键

unsigned char pad_aux_dat[2]  = {0x00, 0x00};       // 当前连接

unsigned char const pad_mode = 0;                  // 0发射 1接收

unsigned char tx_addr[5] = {0x0F, 0x0E, 0x0D, 0x0C, 0x0B};     //设置发送接收地址


#define     KEY_DIR          TRISA4                 // 按键中断引脚
#define     KEY_UP           IOCAP4                 // 上升沿
#define     KEY_DOWN         IOCAN4                 // 下降沿
#define     KEY_FLAG         IOCAF4                 // 标志位
#define     KEY_IO           RA4                    // 电平      

void WirelessPad(void);                           // 无线通讯测试
void LED_Init(void);
void LEDx(unsigned char num);
 

void LED_Init(void) {
    LED_DIR = 0;
    LED_IO = LED_OFF;
}

void LEDx(unsigned char num) {
    while(num--) {
        LED_IO = LED_ON; 
        __delay_ms(200);
        LED_IO = LED_OFF;
        __delay_ms(200);
    }
}

void Key_Init(void) {
    
    KEY_DIR = 1;         // 设置按键引脚为输入
    KEY_IO = 0;
     
//    KEY_UP  = 1;         // 上升沿检测
//    KEY_DOWN = 0;        // 下升沿检测
//    KEY_FLAG = 0;         // RA4 中断变化标志位
//    
//    GIE = 1;            // 中断主开关
//    IOCIE = 1;          // 电平中断使能主开关
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

    OSCCON = OSCCON_125K;       // 初始化晶振
    
    PORT_Init();
    
    LED_Init();
    
    Key_Init();
    
    /********* 逻辑代码区 Start **********/
    
    WirelessPad();
    
    /********* 逻辑代码区 End ************/
    
    while(1);                   // 如果开启看门狗 WDTE，此处会被复位
}

void WirelessPad() {
    
    NRF_On();
    NRF_Port_Init();
    NRF_SPI_Init();
 
    __delay_ms(500);
 
    if(NRF_Check()) {
        while(1) {
            LED_IO = LED_ON; 
            __delay_ms(100);
            LED_IO = LED_OFF;
            __delay_ms(100);
        }
    }
    
    memcpy(pad_aux_dat, pad_aux_ok, 2);
    
    NRF_Tx_Set(tx_addr, 0x00);  
    
    while(1) {

        if(RA4 == 1) {
            LED_IO = LED_ON;
            
            // 注意：如果设置了自动应答，则可能一直循环，自动重发
            NRF_Tx_Send(pad_aux_dat, 2);  
            __delay_ms(1);
        }else {
            LED_IO = LED_OFF;
            __delay_ms(1);        // 5秒发送一次连接情况
        }
    }    
}
