/*
 * isd1700.h
 *
 * Created: 2018/1/13 0:40:53
 *  Author: tianlunvip
 */ 


#ifndef AVR_ISD1700_H_
#define AVR_ISD1700_H_


#ifdef	__cplusplus
extern "C" {
#endif
	
extern unsigned char Status[7];

// 1700 SPI ָ���

#define ISD_PU          0x01			// �ϵ�
#define ISD_STOP        0X02			// ֹͣ��ǰ����
#define ISD_REST        0x03			// оƬ��λ
#define ISD_CLR_INT     0x04			// ���EOM��־���ж��ź�
#define ISD_RD_STAUS    0x05			// ����״̬�Ĵ�������
#define ISD_RD_PLAY_PTR 0x06			// ��1,2�ֽڷ���״̬�Ĵ�����Ϣ��2��3�ֽڷ��ط���ָ��
#define ISD_PD          0x07			// оƬ�µ�
#define ISD_RD_REC_PTR  0x08			// ��1,2�ֽڷ���״̬�Ĵ�����Ϣ��2��3�ֽڷ���¼��ָ��
#define ISD_DEVID       0x09			// ��ȡоƬID����

#define ISD_PLAY        0x40			// �ӵ�ǰ��ַ����
#define ISD_REC         0x41			// �ӵ�ǰ��ַ¼��
#define ISD_ERASE       0x42			// ������ǰ������
#define ISD_G_ERASE     0x43			// ����оƬ��������?
#define ISD_RD_APC      0x44			// ����״̬�Ĵ�����Ϣ��1,2�ֽڣ�APC�Ĵ�����Ϣ��3�ֽ�
#define ISD_WR_APC1     0x45			// ��<D11:D0>д��APC�Ĵ���
#define ISD_WR_APC2     0x65			// ��<D11:D0>д��APC�Ĵ���,����<D2:D0>λ��������

#define ISD_WR_NVCFG    0x46			// ��APC����д��NVCFG
#define ISD_LD_NVCFG    0x47			// ��NVCFG����д��APC
#define ISD_FWD         0x48			// ������ָ��ָ����һ��������ַ��
#define ISD_CHK_MEM     0x49			// ��黷״�洢��ϵ
#define ISD_EXTCLK      0x4A			// �ⲿʱ��ʹ��
#define ISD_SET_PLAY    0x80			// ����ʼ��ַ<S10:S0>������������ַ<E10:E0>����
#define ISD_SET_REC     0x81			// ����ʼ��ַ<S10:S0>¼����������ַ<E10:E0>����
#define ISD_SET_ERASE   0x82			// ����ʼ��ַ<S10:S0>������������ַ<E10:E0>����

#define ISD_NOP			0x00
#define ISD_LED         0x10			// ʹ��LED

 // ISD1700 SPI����ֵ����
 
 #define SR0L			0				// ״̬��Ϣ���ֽ�
 #define SR0H			1				// ״̬��Ϣ���ֽ�
 #define SR1			2
 #define DATA1			2
 #define DATA2			3
 #define VOCADDL		2
 #define VOCADDH		3
 #define APCL			2
 #define APCH			3
 
  // ISD1700״̬�Ĵ���λSR0
  
  #define CMD_ERR		0
  #define FULL			1
  #define PU			2
  #define EOM			3
  #define INT			4
  
  // ISD1700״̬�Ĵ���λSR1
  
  #define RDY			0
  #define ERASE			1
  #define PLAY			2
  #define REC			3
 
 
void ISD_Port_Init(void);
void ISD_SPI_Init(void);
void ISD_Init(void);

unsigned char ISD_SendData(unsigned char dat);

unsigned char ISD_Pu(void);
void ISD_Stop(void);
void ISD_Rest(void);
void ISD_Clr_Int(void);
void ISD_Rd_Status(void);
void ISD_Rd_PlayPtr(void);
void ISD_Pd(void);
void ISD_Rd_RecPtr(void);


unsigned char ISD_Devid(void);



void ISD_Play(void);




void ISD_rec(void);
void ISD_erase(void);
void ISD_g_erase(void);
void ISD_rd_apc(void);
void ISD_wr_apc2(unsigned char apcdatl,unsigned char apcdath);
void ISD_wr_nvcfg(unsigned char apcdatl,unsigned char apcdath);
void ISD_ld_nvcfg(void);
void ISD_Fwd(void);
void ISD_Chk_Mem(void);
void ISD_extclk(void);
void ISD_set_play( unsigned char Saddl,unsigned char Saddh,unsigned char Eaddl,unsigned char Eaddh);
void ISD_set_rec( unsigned char Saddl,unsigned char Saddh,unsigned char Eaddl,unsigned char Eaddh);
void ISD_set_erase( unsigned char Saddl,unsigned char Saddh,unsigned char Eaddl,unsigned char Eaddh);






#ifdef	__cplusplus
}
#endif

#endif /* AVR_ISD1700_H_ */