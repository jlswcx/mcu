#include "macro.h"
#include "port.h"
#include "nrf24l01.h"

#include "config.h"

/* RC3 ����LED�� */
#define     LED_DIR         TRISC3          // RC3 ���ſ���LED
#define     LED_IO          RC3
#define     LED_ON          0               // �͵�ƽ ��
#define     LED_OFF         1               // �ߵ�ƽ ��     


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

unsigned char const pad_mode = 0;                  // 0���� 1����

unsigned char tx_addr[5] = {0x0F, 0x0E, 0x0D, 0x0C, 0x0B};     //���÷��ͽ��յ�ַ


#define     KEY_DIR          TRISA4                 // �����ж�����
#define     KEY_UP           IOCAP4                 // ������
#define     KEY_DOWN         IOCAN4                 // �½���
#define     KEY_FLAG         IOCAF4                 // ��־λ
#define     KEY_IO           RA4                    // ��ƽ      

void WirelessPad(void);                           // ����ͨѶ����
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
    
    KEY_DIR = 1;         // ���ð�������Ϊ����
    KEY_IO = 0;
     
//    KEY_UP  = 1;         // �����ؼ��
//    KEY_DOWN = 0;        // �����ؼ��
//    KEY_FLAG = 0;         // RA4 �жϱ仯��־λ
//    
//    GIE = 1;            // �ж�������
//    IOCIE = 1;          // ��ƽ�ж�ʹ��������
}

/**
 * �жϷ���������� �ж϶���������
 */
void __interrupt() ISR() {
    
    if(IOCIF && IOCAF4) {        // ��ͨ���� RA4
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

    OSCCON = OSCCON_125K;       // ��ʼ������
    
    PORT_Init();
    
    LED_Init();
    
    Key_Init();
    
    /********* �߼������� Start **********/
    
    WirelessPad();
    
    /********* �߼������� End ************/
    
    while(1);                   // ����������Ź� WDTE���˴��ᱻ��λ
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
            
            // ע�⣺����������Զ�Ӧ�������һֱѭ�����Զ��ط�
            NRF_Tx_Send(pad_aux_dat, 2);  
            __delay_ms(1);
        }else {
            LED_IO = LED_OFF;
            __delay_ms(1);        // 5�뷢��һ���������
        }
    }    
}
