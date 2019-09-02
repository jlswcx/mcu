#include "spi.h"
#include <pic.h>

/*
 *      SSP1CON1        ���ƼĴ���
 *          WCOL        д��ͻ���λ��1���������ʹ�Ժ��SSP1BUF��д��ɹ����
 *          SSPOV
 *          SSPEN       ʹ��SPI����λ����������ʱ�����㣬���³�ʼ��SSPICONx�Ĵ���������1
 *          CKP         SPIʱ�Ӽ��Կ��� 0��SCLK����Ϊ�͵�ƽ   1��SCLK����Ϊ�ߵ�ƽ
 *          SSP1M       ʱ��Ƶ��
 *      SSP1STAT        ״̬�Ĵ���
 *          SMP         �����������λ��0��ǰʱ���ز��� 1����ʱ���ز���
 *          CKE         ʱ�ӱ���ѡ��λ��0��ʱ��ǰ��������1��ʱ�ӿ���״̬ʱ����������е����ԣ�
 *          BF          SSP1BUF����Ĵ��������λ��1��SSP1BUF����ȡ��ᱻ����
 *          
 *      SSP1BUF         ���ݻ���Ĵ��������ڲ���SSP1SR�Ĵ������������ݷ���/����
 *      
 *      SSP1IF          ����/�����жϱ�־�ã�
 
 *      ��ȷ��ʼ��I/O�˿����ݷ���
 *      SDI ���뽫��Ӧ��TRIS λ��1  SDI��MISO,ָ��������������,�������������
 *      SDO ���뽫��Ӧ��TRIS λ���� SDO��MOSI,ָ�������������,��������������
 *      SCK ����ģʽ�����뽫��Ӧ��TRIS λ����
 *      SCK ����ģʽ�����뽫��Ӧ��TRIS λ��1
 *      SS ���뽫��Ӧ��TRIS λ��1����������
 * 
 * 
 *      ע�⣺�����SPI�˿ڵ� ANSEL ģ��˿ڳ�ʼ�����㣬�����޷�ʹ�� SPI
 * 
 *      ���� SDI��SDO��MOSI��MISO �����
 *      SDI Ϊ�������룬���������ݣ�������Ҫ���ӵĴ�����Ϊ������� MISO
 *      SDO Ϊ����������� SDI �պ��෴
 */

#define     SPI_SCK_DIR     TRISC0          // SCK
#define     SPI_SCK         RC0
#define     SPI_MOSI_DIR    TRISC1          // MOSI   
#define     SPI_MOSI        RC1    
#define     SPI_MISO_DIR    TRISC2          // MISO
#define     SPI_MISO        RC2

#define     SPI_CSN_DIR     TRISA2          // SS
#define     SPI_CSN         RA2


void SPI_Port_Init() {
    SPI_MOSI_DIR = 1;
    SPI_MISO_DIR = 0;
    SPI_SCK_DIR = 0;
    SPI_CSN_DIR = 0;
    
    SPI_CSN = 1;                    // Ƭѡ���ڸߵ�ƽ     
}


void SPI_Init() {  
    SPI_Port_Init();
    
    SSP1STAT = 0x00;
    SSP1STATbits.CKE = 1;           // ʱ��״̬�ӿ���ת������Чʱ����

    SSP1BUF = 0;                    // �������
    SSP1IF = 0;                     // ����жϱ�־λ          
    SSP1CON1 = 0x20;                // ʹ��SPI����ͨ��      
}


unsigned char SPI_RW(unsigned char dat) {
    SSP1IF = 0;                     // 
    SSP1BUF = dat;
    while(!SSP1IF);                 // д��ɺ�BF��1����ȡSSP1BUF��BF�ᱻ����
    
    return(SSP1BUF);
}  


unsigned char SPI_RW_SW(unsigned char dat) {
    unsigned char bit_ctr;
    
    for(bit_ctr = 0; bit_ctr < 8; bit_ctr++) {
        SPI_MOSI = (dat & 0x80);                      // output 'byte', MSB to MOSI
        dat = (unsigned char)(dat << 1);                             // shift next bit into MSB..
        SPI_SCK = 1;                                  // Set SCK high..
        dat |= (unsigned char)SPI_MISO;                              // capture current MISO bit
        SPI_SCK = 0;                                  // ..then set SCK low again
    }
    SPI_MOSI = 0;                                   // MOSI pin low before return

    return(dat);
}