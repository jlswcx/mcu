/* 
 * File:   nrf24l01.h
 * Author: �����ǳ�
 *
 * Created on 2017��12��14��, ����12:36
 */

#ifndef NRF24L01_H
#define	NRF24L01_H

#ifdef	__cplusplus
extern "C" {
#endif

//******************************************************************************
// NRF24L01�Ĵ����������һ��8����
#define NRF_R_REGISTER          0x00    // �����üĴ���,��5λΪ�Ĵ�����ַ
#define NRF_W_REGISTER          0x20    // д���üĴ���,��5λΪ�Ĵ�����ַ���ڵ���ʹ���ģʽ�²���
#define NRF_R_RX_PAYLOAD        0x61    // ��RX��Ч����,1~32�ֽ�
#define NRF_W_TX_PAYLOAD        0xA0    // дTX��Ч����,1~32�ֽ�
#define NRF_FLUSH_TX            0xE1    // ���TX FIFO�Ĵ���.����ģʽ����
#define NRF_FLUSH_RX            0xE2    // ���RX FIFO�Ĵ���.����ģʽ����
#define NRF_REUSE_TX_PL         0xE3    // ����ʹ����һ������,CEΪ��,���ݰ������Ϸ���.
#define NRF_R_RX_PL_WID         0x60    //
#define NRF_W_ACK_PAYLOAD       0xA8    // ���ڽ���ģʽ
#define NRF_W_TX_PAYLOAD_NOACK  0xB0    // ���ڷ���ģʽ
#define NRF_NOP                 0xFF    // �ղ���,����������״̬�Ĵ���
//******************************************************************************
// NRF24L01�Ĵ�����һ��24����
#define NRF_CONFIG              0x00    // bit7��reserved 0
                                        // bit6��MASK_RX_DR 
                                        // bit5��MASK_TX_DS
                                        // bit4��MASK_MAX_RT
                                        // bit3��EN_CRC  CRC ʹ�� ���EN_AA����һλΪ�ߣ���EN_CRCǿ��Ϊ��
                                        // bit2��CRCO
                                        // bit1��PWR_UP  1���ϵ�    0������
                                        // bit0��PRIM_RX  0������ģʽ   1������ģʽ
#define NRF_EN_AA               0x01    // ʹ���Զ�Ӧ����  bit5~0,��Ӧͨ��5~0 Ĭ������ͨ���Զ�Ӧ������ 00111111
#define NRF_EN_RXADDR           0x02    // ���յ�ַ����,bit5~0,��Ӧͨ��5~0 Ĭ�� 00000011
#define NRF_SETUP_AW            0x03    // ���õ�ַ���(��������ͨ��):bit1~0  00����Ч 01��3�ֽ� 10��4�ֽ� 11��5�ֽ�
#define NRF_SETUP_RETR          0x04    // �����Զ��ط�;bit3~0���Զ��ط�������;bit7~4���Զ��ط���ʱ 250*x+86us
#define NRF_RF_CH               0x05    // ��Ƶͨ��
#define NRF_RF_SETUP            0x06    // ��Ƶ�Ĵ���
#define NRF_STATUS              0x07    // ״̬�Ĵ���;bit0:TX FIFO����־;bit3:1,��������ͨ����(���:6);bit4,�ﵽ�����ط�
                                        // bit5:���ݷ�������ж�;bit6:���������ж�;
#define NRF_OBSERVE_TX          0x08    // ���ͼ��Ĵ���,bit7:4,���ݰ���ʧ������;bit3:0,�ط�������
#define NRF_CD					0x09    // �ز����Ĵ���,bit0,�ز����;
#define NRF_RX_ADDR_P0          0x0A    // ����ͨ��0���յ�ַ,��󳤶�5���ֽ�,���ֽ���ǰ
#define NRF_RX_ADDR_P1          0x0B    // ����ͨ��1���յ�ַ,��󳤶�5���ֽ�,���ֽ���ǰ
#define NRF_RX_ADDR_P2          0x0C    // ����ͨ��2���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define NRF_RX_ADDR_P3          0x0D    // ����ͨ��3���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define NRF_RX_ADDR_P4          0x0E    // ����ͨ��4���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define NRF_RX_ADDR_P5          0x0F    // ����ͨ��5���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define NRF_TX_ADDR             0x10    // ���͵�ַ(���ֽ���ǰ),ShockBurstTMģʽ��,RX_ADDR_P0��˵�ַ���
#define NRF_RX_PW_P0            0x11    // ��������ͨ��0��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define NRF_RX_PW_P1            0x12    // ��������ͨ��1��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define NRF_RX_PW_P2            0x13    // ��������ͨ��2��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define NRF_RX_PW_P3            0x14    // ��������ͨ��3��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define NRF_RX_PW_P4            0x15    // ��������ͨ��4��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define NRF_RX_PW_P5            0x16    // ��������ͨ��5��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define NRF_FIFO_STATUS         0x17    // FIFO״̬�Ĵ���;bit0,RX FIFO�Ĵ����ձ�־;bit1,RX FIFO����־;bit2,3,����
                                        // bit4,TX FIFO�ձ�־;bit5,TX FIFO����־;bit6,1,ѭ��������һ���ݰ�.0,��ѭ��;
#define NRF_DYNPD               0x1C    // 
#define NRF_FEATURE             0x1D    //
    
// ACK_PLD
// TX_PLD
// RX_PLD    
//******************************************************************************
#define MAX_RT					0X10    // MAX_RTD�ڵ�5λ| �ﵽ����ʹ����ж�
#define TX_DS					0X20    // TX_DS�ڵ�6λ  | �ڶ���STATUSʱ�������룬��֪����λ�Ƿ�Ϊ1 TX��������ж�
#define RX_DR					0X40    // RX_DR�ڵ�7λ  | ���յ������ж�

#define RX_P_NO					0x0E    // ״̬�Ĵ����� ��������ͨ����
//******************************************************************************
#define TX_ADR_WIDTH			5		// ���͵�ַ��ȣ���40λ
#define RX_ADR_WIDTH			5		// ���ܵ�ַ���
#define TX_PLOAD_WIDTH			2		// �������ݳ��ȣ��ֽ��������32�ֽڣ�
#define RX_PLOAD_WIDTH			2		// �������ݳ���
//******************************************************************************

//unsigned char Rx_buf[TX_PLOAD_WIDTH];	//�������ݻ�����
//******************************************************************************
//#define RX_P_NO     0x0E				// ��������ͨ����
//#define TX_FULL     0X01				// FIFO�Ĵ�������־��1��  0�п��ÿռ�
//******************************************************************************


void NRF_Port_Init(void);

void NRF_SPI_Init(void);
unsigned char NRF_SPI_RW(unsigned char dat);

unsigned char NRF_Read_Reg(unsigned char num, unsigned char reg);
unsigned char NRF_Write_Reg(unsigned char num, unsigned char reg, unsigned char value);

unsigned char NRF_Read_Buf(unsigned char num, unsigned char reg, unsigned char *pBuf, unsigned char buf);
unsigned char NRF_Write_Buf(unsigned char num, unsigned char reg, unsigned char *pBuf, unsigned char buf);

unsigned char NRF_Check(unsigned char num);
void NRF_Power_off(unsigned char num);

void NRF_TX_Mode(unsigned char num, unsigned char *address, unsigned char *txbuf);
void NRF_RX_Mode(unsigned char num);

unsigned char NRF_TxPacket(unsigned char num, unsigned char *address, unsigned char *txbuf);
unsigned char NRF_RxPacket(unsigned char num, unsigned char *rxbuf);

unsigned char NRF_Get_Current_Pipenum(unsigned char num);
unsigned char NRF_Read_RX_Pload(unsigned char num, unsigned char *pBuf);  
unsigned char NRF_Get_Status(unsigned char num);
void NRF_Flush_TX(unsigned char num);
void NRF_Flush_RX(unsigned char num);

#ifdef	__cplusplus
}
#endif

#endif	/* NRF24L01_H */

