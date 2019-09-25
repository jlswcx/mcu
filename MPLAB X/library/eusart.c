#include "eusart.h"
#include <pic.h>


void EUSART_Tx(void) {
    
//    TXCKSEL = 1;    // RA0 具有 TX/CK 功能 
    TXCKSEL = 0;    // RC4 具有 TX/CK 功能 
    
//    TXIE = 1;       // 发送中断
    TXEN = 1;       // 使能发送器电路
 //   TXIF = 0;       // 发送中断标志位
}

void EUSART_Rx(void) {
    
//    RXDTSEL = 1;    // RA1 具有 RX/DT 功能
    RXDTSEL = 0;    // RC5 具有 RX/DT 功能
    TRISC5 = 1;     // RX 输入   
    
    RCIE = 1;       // 接收中断
    CREN = 1;       // 使能接收
    RCIF = 0;       // 接收中断标志位
}

// 波特率初始化
void EUSART_Init(void) {
        
    EUSART_Tx();
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

void EUSART_Send(unsigned char dat) {
    TXREG = dat;
    while(TRMT == 0);       // loop
}
