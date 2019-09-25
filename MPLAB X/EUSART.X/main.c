#include "fosc.h"
#include "macro.h"
#include "port.h"
#include "ledx.h"

#include "config.h"


void EUSART_TX() {
    
    TXCKSEL = 0;    // RC4 具有 TX/CK 功能 
    
//    TXIE = 1;       // 发送中断
    TXEN = 1;       // 使能发送器电路
 //   TXIF = 0;       // 发送中断标志位
}

void EUSART_RX() {
    
    RXDTSEL = 0;    // RC5 具有 RX/DT 功能
    TRISC5 = 1;     // RX 输入   
    
    RCIE = 1;       // 接收中断
    CREN = 1;       // 使能接收
    RCIF = 0;       // 接收中断标志位
}

// 波特率初始化
void EUSART_Init() {
        
    EUSART_TX();
//    EUSART_RX();
    
    // 波特率设置
      SYNC = 0;
      BRG16 = 1;
      BRGH = 1;


      //SPBRG = 0x67;       // 9600
      SPBRGH = 0;
      SPBRGL = 0x67;      
      
      SPEN = 1;       // 使能 EUSART
      PEIE = 1;
 //       GIE = 1;
}

void __interrupt() isr() {
    
    if (RCIF) {

        
        RCIF = 0;      
    }
    
    if(TXIF) {
        

        
  //      TXIF = 0;
    }
 
}

void main(void) {
    
    FOSC_Init();
    PORT_Init();
    
    LED_Init();
    
    EUSART_Init();
             
    while(1) {

        TXREG = 'A';     // 写入字节 ，启动发送
  //      TXIE = 1;

        while(TRMT == 0);       // 传送为 1 传完清 0
        LEDx(1);
        __delay_ms(1500);
        
    }
}
