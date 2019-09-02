#include "macro.h"
#include "port.h"
#include "nrf24l01.h"

#include "config.h"

/*
 * ��Ʒ��������
 * 5�뷢��һ���������
 * ���¼����Ͱ�����Ϣ
 * �ɿ��������ɿ���Ϣ
 */

/* RC3 ����LED�� */
#define     LED_DIR         TRISC3          // RC3 ���ſ���LED
#define     LED             RC3
#define     LED_ON          0               // �͵�ƽ ��
#define     LED_OFF         1               // �ߵ�ƽ ��


#define     LED_RED_DIR         TRISC4          // ���
#define     LED_BLUE_DIR        TRISC5          // ����
#define     LED_RED_             RC4          
#define     LED_BLUE_            RC5          

#define     NRF_IRQ_DIR         TRISA4          // RA4 IRQ�ж�����
#define     NRF_IRQ             RA4

#define     BEEP_DIR            TRISA5          // RA4 IRQ�ж�����
#define     BEEP_IO             RA5

/* NRF���͵�5���������� */
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


unsigned char const pad_aux_ok[2]   = {0x01,0x01};  // ��������
unsigned char const pad_aux_on[2]   = {0x02,0x02};  // ���°���
unsigned char const pad_aux_off[2]  = {0x03,0x03};  // �ɿ�����

unsigned char pad_aux_dat[2]  = {0x00, 0x00};       // ��ǰ����

unsigned char rx_addr_0[5] = {0x0F, 0x0E, 0x0D, 0x0C, 0x0B};     //���÷��ͽ��յ�ַ

void WirelessPad(void);                         // ����ͨѶ����

void NRF_IRQ_Init(void);
void LED_Reds(unsigned char num);
void LED_Blues(unsigned char num);

void LED_Red(unsigned char num) {
    while(num--) {
        LED_RED_ = 1;;
        __delay_ms(200);
        LED_RED_ = 0;
        __delay_ms(200);   
    }
    __delay_ms(1000);
}

void LED_Blue(unsigned char num) {
    while(num--) {
        LED_BLUE_ = 1;;
        __delay_ms(200);
        LED_BLUE_ = 0;
        __delay_ms(200);   
    }
    __delay_ms(1000);
}

void main() {
    
    OSCCON = OSCCON_1M;       // ��ʼ������
    
    PORT_Init();
    
    NRF_IRQ_Init();

    /********* �߼������� Start **********/
   
    WirelessPad();
    
    /********* �߼������� End ************/
    
    while(1);                   // ����������Ź� WDTE���˴��ᱻ��λ
}


void NRF_IRQ_Init(void) {
    NRF_IRQ_DIR = 1;    // ���ð�������Ϊ����
//    NRF_IRQ = 0;
            
//    IOCAP4 = 1;         // RA4 �����ؼ��
//    IOCAN4 = 1;       // RA4 �����ؼ��
//    IOCAF4 = 0;         // RA4 �жϱ仯��־λ
//    
//    GIE = 1;            // �ж�������
//    IOCIE = 1;          // ��ƽ�ж�ʹ��������
}

/**
 * �жϷ���������� �ж϶���������
 */
void __interrupt() ISR() {
    
//    unsigned char rxbuf[2] ={0, 0};
//    
//    
//    if(IOCIF && IOCAF4) {        // ��ͨ���� RA4
//        IOCIE = 0;
//        IOCIF = 0;
//        IOCAF4 = 0;
//            
//        LED_Red(1);
//        /******** Start *******/
//        
//        if(RA4 == 1) {
//            
//            NRF_RxPacket(rxbuf);
//            if(rxbuf[0] == pad_aux_ok[0]) {
//                LED_Blue(2);
//            }
//            NRF_IRQ_Reset();
//            
//        }else {
//
//        }
//        
//        /******** End *********/
//        
//        IOCIE = 1;
//    }
    
//    if(IOCIF && IOCAF5) {        // ��ͨ���� RA5
//        GIE = 0;
//        IOCIF = 0;
//        IOCAF5 = 0;
//        
//        /******** Start *******/
//        
//        while(RA5);
//        
//        if(!RA5) {
//            if(click == 0) {
//                PORTA = 0;
//                PORTC = 0;
//                SLEEP();
//                
//                click = 1;
//            }else { 
//                write_ee(0x00, ++led_t);
//                click = 0;
//                RESET(); 
//            }    
//        }
//        
//        /******** End *********/
//         GIE = 1;
//    }
    
}



void WirelessPad() {
    
    unsigned char ledx; 
    unsigned char rxbuf[2] ={0, 0};
    
    NRF_On();
    NRF_Port_Init();
    NRF_SPI_Init();
 
    __delay_ms(500);
    
    if(NRF_Check()) {
        while(1) {
            LED_Red(1);
        }
    }else {
        LED_Blue(1);
    }
    
    NRF_Rx_Set(NRF_P0_ACK);
    NRF_Rx_Set_P0(rx_addr_0, 2);
 
    memcpy(pad_aux_dat, pad_aux_ok, 2);
    
    while(1) {
        // �����ж�
        if(NRF_IRQ == 0) {
            rxbuf[0] = 0;
            ledx = NRF_RxPacket(rxbuf);
            NRF_Clear_IRQ();
            if(rxbuf[0] == pad_aux_ok[0]) {
                BEEP_DIR = 0; BEEP_IO = 1;
            //    LED_Blue(2);
            }  
        }else {
            BEEP_DIR = 1; BEEP_IO = 0;
        //    LED_Red(3);
        }
        __delay_ms(500);        // 5�뷢��һ���������
    }    
}
