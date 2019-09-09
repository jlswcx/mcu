#include "spi.h"

#include <avr/io.h>
#include "io_168pa.h"

/*
 *		SPCR			���ƼĴ���
 *			SPIE		�ж�ʹ��
 *			SPE			SPIʹ��
 *			DORD		0��MSB���ȷ���	1��LSB���ȷ���
 *			MSTR		1������ģʽ	0���ӻ�ģʽ
 *			CPOL		ʱ�Ӽ���		0��SCK����Ϊ�͵�ƽ	1��SCK����Ϊ�ߵ�ƽ
 *			CPHA		ʱ����λ		0����ʼ�ز���		1: �����ز���
 *			SPR1,SPR0	ʱ������ѡ��
 *		SPSR			״̬�Ĵ���
 *			SPIF		�жϱ�־λ
 *			WCOL		д��ͻ��־λ
 *		SPDR			���ݼĴ���			
			
 *
 *      ��ȷ��ʼ��I/O�˿����ݷ���
 *      SDI ���뽫��Ӧ��TRIS λ��1  SDI��MOSI,ָ�������������,��������������
 *      SDO ���뽫��Ӧ��TRIS λ���� SDO��MISO,ָ��������������,�������������
 *      SCK ����ģʽ�����뽫��Ӧ��TRIS λ����
 *      SCK ����ģʽ�����뽫��Ӧ��TRIS λ��1
 *      SS ���뽫��Ӧ��TRIS λ��1����������
*/


#define SPI_MOSI_DIR		DDRB3
#define SPI_MOSI			PORTB3
#define SPI_MISO_DIR		DDRB4
#define SPI_MISO			PORTB4
#define SPI_SCK_DIR			DDRB5
#define SPI_SCK				PORTB5

#define SPI_CSN_DIR			DDRD2
#define SPI_CSN				PORTD2         // SPI SS Ƭѡ


void SPI_Port_Init(void) {
	SPI_MOSI_DIR = 1;
	SPI_MISO_DIR = 0;
	SPI_SCK_DIR = 1;
	SPI_CSN_DIR = 1;
	
	SPI_CSN = 1;                // Ƭѡ���ڸߵ�ƽ
}


void SPI_Init(void) {  
	
	SPI_Port_Init();		// �˿ڳ�ʼ��
	
	SPCRbit->SPIE = 0;		// �ж�ʹ��λ ��ʹ��SPIF��SREGȫ���ж�
	SPCRbit->SPE  = 1;		// ʹ��SPI
	SPCRbit->DORD = 0;		// ���ݷ���˳��0 ��λ�ȷ���  1 ��λ�ȷ���
	SPCRbit->MSTR = 1;		// ��ģʽ
	SPCRbit->CPOL = 0;		// SCK����ʱΪ�͵�ƽ
	SPCRbit->CPHA = 0;		// 0 ��ʼ�ز��� 1 �����ز���
	
	SPSR = 0;

}

// SPIӲ��ͨ��ģʽ
unsigned char SPI_RW(unsigned char dat) {
	
    SPDR = dat;
	while(!SPSRbit->SPIF);
	dat = SPDR;
	
	return dat;
}




