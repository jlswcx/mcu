#include "fosc.h"
#include "macro.h"
#include "port.h"
#include "ledx.h"

#include "config.h"


void EUSART_TX() {
    
    TXCKSEL = 0;    // RC4 ���� TX/CK ���� 
    
//    TXIE = 1;       // �����ж�
    TXEN = 1;       // ʹ�ܷ�������·
 //   TXIF = 0;       // �����жϱ�־λ
}

void EUSART_RX() {
    
    RXDTSEL = 0;    // RC5 ���� RX/DT ����
    TRISC5 = 1;     // RX ����   
    
    RCIE = 1;       // �����ж�
    CREN = 1;       // ʹ�ܽ���
    RCIF = 0;       // �����жϱ�־λ
}

// �����ʳ�ʼ��
void EUSART_Init() {
        
    EUSART_TX();
//    EUSART_RX();
    
    // ����������
      SYNC = 0;
      BRG16 = 1;
      BRGH = 1;


      //SPBRG = 0x67;       // 9600
      SPBRGH = 0;
      SPBRGL = 0x67;      
      
      SPEN = 1;       // ʹ�� EUSART
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

        TXREG = 'A';     // д���ֽ� ����������
  //      TXIE = 1;

        while(TRMT == 0);       // ����Ϊ 1 ������ 0
        LEDx(1);
        __delay_ms(1500);
        
    }
}
