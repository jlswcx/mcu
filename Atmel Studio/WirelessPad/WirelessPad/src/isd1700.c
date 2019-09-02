#include "isd1700.h"

#include <avr/io.h>
#include "io_168pa.h"
#include "spi.h"
#include <util/delay.h>



// �������صĶ˿�����
#define		KEY_TYPE_DIR		DDRC5
//#define		KEY_TONE_DIR		ADC6
#define		KEY_COM_DIR			DDRB1

// ʵ�ʵ�·�Ķ˿�����

#define		ISD_MOSI_DIR		DDRB3
#define		ISD_MOSI			PORTB3
#define		ISD_MISO_DIR		DDRB4
#define		ISD_MISO			PORTB4
#define		ISD_SCK_DIR			DDRB5
#define		ISD_SCK				PORTB5

#define		ISD_SS_DIR			DDRB2
#define		ISD_SS				PORTB2

#define		AMP_CONTROL_DIR		DDRB0
#define		AMP_CONTROL			PORTB0

unsigned char Status[7];

void ISD_Port_Init(void) {

	AMP_CONTROL_DIR = 1;
	AMP_CONTROL		= 1;
	
	ISD_MOSI_DIR	= 1;
	ISD_MISO_DIR	= 0;
	ISD_SCK_DIR		= 1;
		
	ISD_SS_DIR		= 1;
	ISD_SS			= 1;
}


void ISD_SPI_Init(void) {
	SPCRbit->SPIE = 0;		// �ж�ʹ��λ ��ʹ��SPIF��SREGȫ���ж�
	SPCRbit->SPE  = 1;		// ʹ��SPI
	SPCRbit->DORD = 1;		// ���ݷ���˳��0 ��λ�ȷ���  1 ��λ�ȷ���
	SPCRbit->MSTR = 1;		// ��ģʽ
	SPCRbit->CPOL = 1;		// SCK����ʱΪ�ߵ�ƽ
	SPCRbit->CPHA = 0;		// 0 ��ʼ�ز��� 1 �����ز���

	SPSR = 0;				// ״̬�Ĵ���	
}

void ISD_Init(void) {
	ISD_Port_Init();
	ISD_SPI_Init();
	ISD_Pu();
}

unsigned char ISD_SendData(unsigned char dat) {

	SPDR = dat;
	while(!SPSRbit->SPIF);
	dat = SPDR;
	
	SPSR = 0;
	
	return SPDR;
}


/**************************************************************************
*    ����ԭ��: void ISD_Pu(void);        ����ָ�����Ҫ�ӳ�50ms
*    ��    ��: �ϵ�
**************************************************************************/
unsigned char ISD_Pu(void) {
	ISD_SS = 0;
	ISD_SendData(ISD_PU | ISD_LED);
	ISD_SendData(0x00);
	ISD_SS = 1;
	
	//delay_ms(50);
	ISD_Rd_Status();
	
	if (Status[SR0L]&(1<< ISD_PU))
		return 1;
	else
		return 0;
}


/**************************************************************************
*    ����ԭ��: void ISD_Stop(void);
*    ��    ��: ֹͣ��ǰ����
**************************************************************************/
void ISD_Stop(void) {
	ISD_SS = 0;
	ISD_SendData(ISD_STOP | ISD_LED);
	ISD_SendData(0x00);
	ISD_SS = 1;
}


/**************************************************************************
*    ����ԭ��: void ISD_Reset(void);     ����ָ�����Ҫ�ӳ�50ms
*    ��    ��: ISD1700��λ
**************************************************************************/
void ISD_Rest(void) {
	ISD_SS = 0;
	ISD_SendData(ISD_REST | ISD_LED);
	ISD_SendData(0x00);
	ISD_SS = 1;
}


/**************************************************************************
*    ����ԭ��: void ISD_Clr_Int()(void);
*    ��    ��: ����жϱ�־
**************************************************************************/
void ISD_Clr_Int(void) {
	ISD_SS = 0;
	ISD_SendData(ISD_CLR_INT | ISD_LED);
	ISD_SendData(0x00);
	ISD_SS = 1;
}


/**************************************************************************
*    ����ԭ��: void ISD_Rd_Status(void);
*    ��    ��: ��ȡ״̬�Ĵ�������
**************************************************************************/
void ISD_Rd_Status(void) {
	
	unsigned char i ;

	ISD_SS = 0;
	ISD_SendData(ISD_RD_STAUS | ISD_LED);					// ����
	ISD_SendData(0x00);
	ISD_SendData(0x00);
	ISD_SS = 1;

	for(i=0;i<5;i++){;}							//�ӳ�
	
	ISD_SS = 0;
	Status[0]=ISD_SendData(ISD_RD_STAUS | ISD_LED);		// ��MISO ����״̬
	Status[1]=ISD_SendData(0x00);
	Status[2]=ISD_SendData(0x00);
	ISD_SS = 1;
}


/**************************************************************************
*    ����ԭ��: void ISD_Rd_PlayPtr(void);
*    ��    ��: ��ȡ����ָ��
**************************************************************************/
void ISD_Rd_PlayPtr(void) {
	
	unsigned char i;
	
	ISD_SS = 0;
	ISD_SendData(ISD_RD_PLAY_PTR | ISD_LED);              // ����
	ISD_SendData(0x00);
	ISD_SendData(0x00);
	ISD_SendData(0x00);
	ISD_SS = 1;

	for(i=5;i>0;i--){;}							//�ӳ�
	
	ISD_SS = 0;
	Status[SR0L] = ISD_SendData(ISD_RD_PLAY_PTR | ISD_LED);		// ��MISO ����״̬
	Status[SR0H] = ISD_SendData(0x00);
	Status[VOCADDL] = ISD_SendData(0x00);				// ��ȡ����ָ���8λ
	Status[VOCADDH] = ISD_SendData(0x00);				// ��ȡ����ָ���8λ
	ISD_SS = 1;
}


/**************************************************************************
*    ����ԭ��: void ISD_PD(void);        ����ָ�����Ҫ�ӳ�50ms
*    ��    ��: �µ�
**************************************************************************/
void ISD_Pd(void) {
	ISD_SS = 0;
	ISD_SendData(ISD_PD | ISD_LED);            // ����PD����
	ISD_SendData(0x00);
	ISD_SS = 1;
}

/**************************************************************************
*    ����ԭ��: void ISD_Rd_RecPtr(void);
*    ��    ��: ��ȡ¼��ָ��
**************************************************************************/
void ISD_Rd_RecPtr(void) {
	
	unsigned char i;
	
	ISD_SS = 0;
	ISD_SendData(ISD_RD_REC_PTR | ISD_LED);              // ����
	ISD_SendData(0x00);
	ISD_SendData(0x00);
	ISD_SendData(0x00);
	ISD_SS = 1;

	for(i=5;i>0;i--) {;}            //�ӳ�
	
	ISD_SS = 0;
	Status[SR0L] = ISD_SendData(ISD_RD_REC_PTR | ISD_LED);        // ��MISO ����״̬
	Status[SR0H] = ISD_SendData(0x00);
	Status[VOCADDL] = ISD_SendData(0x00);     // ��ȡ¼��ָ���8λ
	Status[VOCADDH] = ISD_SendData(0x00);     // ��ȡ¼��ָ���8λ
	ISD_SS = 1;
}


unsigned char ISD_Devid(void) {
	
	unsigned char i;
	
	ISD_SS = 0;
	ISD_SendData(ISD_DEVID | ISD_LED);
	ISD_SendData(0x00);
	ISD_SendData(0x00);
	ISD_SS = 1;

	for(i=5;i>0;i--){;}             //�ӳ�
	
	ISD_SS = 0;
	ISD_SendData(ISD_DEVID | ISD_LED);
	ISD_SendData(0x00);
	Status[2] = ISD_SendData(0x00);
	ISD_SS = 1;

	return Status[2] ;
}


















/**************************************************************************
*    ����ԭ��: void ISD_Play()(void);
*    ��    ��: ����
**************************************************************************/
void ISD_Play(void) {
	
	ISD_SS = 0;
	
	ISD_SendData(ISD_PLAY | ISD_LED);
	ISD_SendData(0x00);
	
	ISD_SS = 1;
}











/**************************************************************************
*    ����ԭ��: void ISD_Rec()(void);
*    ��    ��: ¼��
**************************************************************************/
void ISD_rec(void) {
	
	ISD_SS = 0;
	ISD_SendData(ISD_REC);            // ����¼������
	ISD_SendData(0x00);
	ISD_SS = 1;
}

/**************************************************************************
*    ����ԭ��: void ISD_Erase(void);
*    ��    ��: ����
**************************************************************************/
void ISD_erase(void) {
	
	ISD_SS = 0;
	ISD_SendData(ISD_ERASE);            // ���Ͳ���ָ��
	ISD_SendData(0x00);
	ISD_SS = 1;
}

/**************************************************************************
*    ����ԭ��: void ISD_G_Erase(void);
*    ��    ��: ȫ�����
**************************************************************************/
void ISD_g_erase(void) {
	
	ISD_SS = 0;
	ISD_SendData(ISD_G_ERASE);            // ����ȫ���������
	ISD_SendData(0x00);
	ISD_SS = 1;
}

/**************************************************************************
*    ����ԭ��: void ISD_RD_APC(void);
*    ��    ��: ��ȡAPC �Ĵ�������
**************************************************************************/
void ISD_rd_apc(void) {   
	
	unsigned char i;
	
	ISD_SS = 0;
	ISD_SendData(ISD_RD_APC);
	ISD_SendData(0x00);
	ISD_SendData(0x00);
	ISD_SendData(0x00);
	ISD_SS = 1;

	for(i=2;i>0;i--)             //�ӳ�
	{;}

	ISD_SS = 0;
	Status[SR0L] = ISD_SendData(ISD_RD_APC);        // ��MISO ����״̬
	Status[SR0H] = ISD_SendData(0x00);
	Status[APCL] = ISD_SendData(0x00);     // ��ȡAPC��8λ
	Status[APCH] = ISD_SendData(0x00);     // ��ȡAPC��8λ
	ISD_SS = 1;
	
}

/**************************************************************************
*    ����ԭ��: void ISD_WR_APC2(unsigned char apcdatl,apcdath);
*    ��    ��: ����APC2
**************************************************************************/
void ISD_wr_apc2(unsigned char apcdatl,unsigned char apcdath) {
	
	ISD_SS = 0;
	ISD_SendData(ISD_WR_APC2);
	ISD_SendData(apcdatl);        //���͵�8λ����
	ISD_SendData(apcdath);        //���͸�8λ����
	ISD_SS = 1;


}


/**************************************************************************
*    ����ԭ��: void ISD_WR_NVCFG(unsigned char apcdatl,apcdath);
*    ��    ��: ��APC����д��NVCFG�Ĵ�����
**************************************************************************/
void ISD_wr_nvcfg(unsigned char apcdatl,unsigned char apcdath) {
	
	ISD_SS = 0;
	ISD_SendData(ISD_WR_NVCFG);
	ISD_SendData(apcdatl);        //���͵�8λ����
	ISD_SendData(apcdath);        //���͸�8λ����
	ISD_SS = 1;
}

/**************************************************************************
*    ����ԭ��: void ISD_LD_NVCFG(void);
*    ��    ��: ��NVCFG��������APC�Ĵ�����
**************************************************************************/
void ISD_ld_nvcfg(void) {
	
	ISD_SS = 0;
	ISD_SendData(ISD_LD_NVCFG);
	ISD_SendData(0x00);
	ISD_SS = 1;
}

/**************************************************************************
*    ����ԭ��: void ISD_Fwd(void);
*    ��    ��: ����ָ������һ�Σ�ָ����һ����������ʼ��ַ
**************************************************************************/
void ISD_Fwd(void) {
	
	ISD_SS = 0;
	
	ISD_SendData(ISD_FWD);
	ISD_SendData(0x00);
	
	ISD_SS = 1;
}


/**************************************************************************
*    ����ԭ��: void ISD_CHK_MEM()(void);
*    ��    ��: �ռ���
**************************************************************************/

void ISD_Chk_Mem(void)
{
	ISD_SS = 0;
	ISD_SendData(ISD_CHK_MEM);
	ISD_SendData(0x00);

	ISD_SS = 1;
}

/**************************************************************************
*    ����ԭ��: void ISD_EXTCLK(void);
*    ��    ��: ����/�ر� �ⲿʱ��ģʽ
**************************************************************************/

void ISD_extclk(void)
{
	ISD_SS = 0;
	ISD_SendData(ISD_EXTCLK);
	ISD_SendData(0x00);

	ISD_SS = 1;
}



/**************************************************************************
*    ����ԭ��: void ISD_SET_PLAY( unsigned char Saddl��Saddh��Eaddl��Eaddh)
*    ��    ��: ���㲥��
*
**************************************************************************/
void ISD_set_play( unsigned char Saddl,unsigned char Saddh,unsigned char Eaddl,unsigned char Eaddh) {

	ISD_SS = 0;
	
	ISD_SendData(ISD_SET_PLAY);
	ISD_SendData(0x00);
	ISD_SendData(Saddl);     // ��ʼ��ַ��8λ
	ISD_SendData(Saddh);     // ��ʼ��ַ��8λ
	ISD_SendData(Eaddl);     // ������ַ��8λ
	ISD_SendData(Eaddh);     // ������ַ��8λ
	ISD_SendData(0x00);

	ISD_SS = 1;

}

/**************************************************************************
*    ����ԭ��: void ISD_SET_Rec( unsigned char Saddl��Saddh��Eaddl��Eaddh)
*    ��    ��: ����¼��
*
**************************************************************************/
void ISD_set_rec( unsigned char Saddl,unsigned char Saddh,unsigned char Eaddl,unsigned char Eaddh) {

	ISD_SS = 0;
	
	ISD_SendData(ISD_SET_REC);
	ISD_SendData(0x00);
	ISD_SendData(Saddl);     // ��ʼ��ַ��8λ
	ISD_SendData(Saddh);     // ��ʼ��ַ��8λ
	ISD_SendData(Eaddl);     // ������ַ��8λ
	ISD_SendData(Eaddh);     // ������ַ��8λ
	ISD_SendData(0x00);

	ISD_SS = 1;

}


/**************************************************************************
*    ����ԭ��: void ISD_SET_Erase( unsigned char Saddl��Saddh��Eaddl��Eaddh)
*    ��    ��: ����¼��
*
**************************************************************************/
void ISD_set_erase( unsigned char Saddl,unsigned char Saddh,unsigned char Eaddl,unsigned char Eaddh) {

	ISD_SS = 0;
	
	ISD_SendData(ISD_SET_ERASE);
	ISD_SendData(0x00);
	ISD_SendData(Saddl);     // ��ʼ��ַ��8λ
	ISD_SendData(Saddh);     // ��ʼ��ַ��8λ
	ISD_SendData(Eaddl);     // ������ַ��8λ
	ISD_SendData(Eaddh);     // ������ַ��8λ
	ISD_SendData(0x00);

	ISD_SS = 1;

}
