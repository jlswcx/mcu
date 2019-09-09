#include "nrf24l01.h"

#include "stdlib.h"
#include <avr/io.h>
#include "io_168pa.h"
#include <util/delay.h>


// NRF24L01 ���I/O����

#define		NRF_MOSI_DIR	DDRB3
#define		NRF_MOSI		PORTB3
#define		NRF_MISO_DIR	DDRB4
#define		NRF_MISO		PORTB4
#define		NRF_SCK_DIR		DDRB5
#define		NRF_SCK			PORTB5

#define     NRF_CE_DIR      DDRC4
#define		NRF_CE			PORTC4			// ��һ��NRFʹ�ܶ˿�
#define		NRF_CSN_DIR		DDRD7
#define		NRF_CSN			PORTD7			// ��һ��NRFƬѡ�˿�

#define     NRF_CE2_DIR     DDRD4			
#define		NRF_CE2			PORTD4			// �ڶ���NRFʹ�ܶ˿�
#define		NRF_CSN2_DIR	DDRD2
#define		NRF_CSN2		PORTD2			// �ڶ���NRFƬѡ�˿�     


// NRF24L01 ���� 0���ϵ磬1���ϵ磨����PICоƬС�壩

//#define     NRF_EN_DIR      TRISA5
//#define     NRF_EN          RA5

/*
 * д��ַʱ��д����ֽڣ���д����ֽ�
 * ͨ��2-5�ĵ�ַ��ǰ4���ֽ�[39:8]��ͨ��1�ĵ�ַͨ�ã����һλ���ֽڲ�ͬ
 */

unsigned char TX_ADDRESS[5] = {0x0F, 0x0E, 0x0D, 0x0C, 0x0B};			//���÷��ͽ��յ�ַ
unsigned char TX_ADDRESS2[5] = {0x0F, 0x0E, 0x0D, 0x0C, 0x0B};			//���÷��ͽ��յ�ַ
	
unsigned char RX_ADDRESS_P0[5] = {0x0F, 0x0E, 0x0D, 0x0C, 0x0B};		// ͨ��0���յ�ַ
unsigned char RX_ADDRESS_P1[5] = {0x0A, 0x10, 0x09, 0x08, 0x07};		// ͨ��1���յ�ַ
unsigned char RX_ADDRESS_P2[1] = {0x0B};								// ͨ��2���յ�ַ
unsigned char RX_ADDRESS_P3[1] = {0x0C};								// ͨ��3���յ�ַ
unsigned char RX_ADDRESS_P4[1] = {0x0D};								// ͨ��4���յ�ַ
unsigned char RX_ADDRESS_P5[1] = {0x0E};								// ͨ��5���յ�ַ

unsigned char RX_ADDRESS2_P0[5] = {0x0F, 0x0E, 0x0D, 0x0C, 0x0B};		// ͨ��0���յ�ַ
unsigned char RX_ADDRESS2_P1[5] = {0x0A, 0x10, 0x09, 0x08, 0x07};		// ͨ��1���յ�ַ
unsigned char RX_ADDRESS2_P2[1] = {0x0B};								// ͨ��2���յ�ַ
unsigned char RX_ADDRESS2_P3[1] = {0x0C};								// ͨ��3���յ�ַ
unsigned char RX_ADDRESS2_P4[1] = {0x0D};								// ͨ��4���յ�ַ
unsigned char RX_ADDRESS2_P5[1] = {0x0E};								// ͨ��5���յ�ַ
	
	
void NRF_Port_Init(void) {

	NRF_MOSI_DIR = 1;
	NRF_MISO_DIR = 0;
	NRF_SCK_DIR = 1;
	
	NRF_CSN_DIR = 1;
	NRF_CSN = 1;				// ��ʼƬѡ״̬Ϊ�ߵ�ƽ ���ͺ�����ͨѶ
	
	NRF_CSN2_DIR = 1;
	NRF_CSN2 = 1;				
	
	NRF_CE_DIR = 1;	
	NRF_CE = 0;					// CE�͵�ƽΪ����״̬
		
	NRF_CE2_DIR = 1;
	NRF_CE2 = 0;
}


void NRF_SPI_Init(void) {
	SPCRbit->SPIE = 0;			// �ж�ʹ��λ ��ʹ��SPIF��SREGȫ���ж�
	SPCRbit->SPE  = 1;			// ʹ��SPI
	SPCRbit->DORD = 0;			// ���ݷ���˳��0 ��λ�ȷ���  1 ��λ�ȷ���
	SPCRbit->MSTR = 1;			// ��ģʽ
	SPCRbit->CPOL = 0;			// SCK����ʱΪ�͵�ƽ
	SPCRbit->CPHA = 0;			// 0 ��ʼ�ز��� 1 �����ز���
	
	SPSR = 0;					// ״̬�Ĵ���	
}


// SPIӲ��ͨ��ģʽ
unsigned char NRF_SPI_RW(unsigned char dat) {
	SPDR = dat;
	while(!SPSRbit->SPIF);
	dat = SPDR;
	
	SPSR = 0;
	
	return dat;
}


unsigned char NRF_Read_Reg(unsigned char num, unsigned char reg) {
	unsigned char status;
	if (num == 1) {
		NRF_CSN = 0;				// Ƭѡ����
	}else if(num == 2) {
		NRF_CSN2 = 0;
	}                 
  	NRF_SPI_RW(reg);                    // д��Ĵ�����ַ,
  	status = NRF_SPI_RW(NRF_NOP);    	// ���д��Ĵ�������,�����Ĵ�������
	if (num == 1) {
		NRF_CSN = 1;				// Ƭѡ����
	}else if(num == 2) {
		NRF_CSN2 = 1;
	}
  	return(status);                 // ���ؼĴ�������
}


unsigned char NRF_Write_Reg(unsigned char num, unsigned char reg, unsigned char value) {
	unsigned char status;
	if (num == 1) {
		NRF_CSN = 0;				// Ƭѡ����
	}else if(num == 2) {
		NRF_CSN2 = 0;
	}
  	status = NRF_SPI_RW(reg);           // д��Ĵ�����ַ,ͬʱ����״̬�Ĵ�������
  	NRF_SPI_RW(value);                  // д��Ĵ�������
	if (num == 1) {
		NRF_CSN = 1;				// Ƭѡ����
	}else if(num == 2) {
		NRF_CSN2 = 1;
	}
  	return(status);                 // ����״̬�Ĵ�������
}


unsigned char NRF_Read_Buf(unsigned char num, unsigned char reg, unsigned char *pBuf, unsigned char buf) {
	unsigned char status,i;
	if (num == 1) {
		NRF_CSN = 0;					// Ƭѡ����
	}else if(num == 2) {
		NRF_CSN2 = 0;
	}
  	status = NRF_SPI_RW(reg);               // д��Ĵ�����ַ
  	for(i = 0; i < buf; i++)            // ��ȡI������
	{
   		pBuf[i] = NRF_SPI_RW(NRF_NOP);    	// �����Ĵ������ݷ�������
  	}
	if (num == 1) {
		NRF_CSN = 1;					// Ƭѡ����
	}else if(num == 2) {
		NRF_CSN2 = 1;
	}
  	return(status);                     // ����״̬�Ĵ�������
}


unsigned char NRF_Write_Buf(unsigned char num, unsigned char reg, unsigned char *pBuf, unsigned char buf) {
	unsigned char status, i;
	if (num == 1) {
		NRF_CSN = 0;					// Ƭѡ����
	}else if(num == 2) {
		NRF_CSN2 = 0;
	}
  	status = NRF_SPI_RW(reg);    			// д��Ĵ�����ַ
  	for(i = 0; i < buf; i++) {			// д��I������
    	NRF_SPI_RW(*pBuf++);				// ����������д��Ĵ���
	}
	if (num == 1) {
		NRF_CSN = 1;					// Ƭѡ����
	}else if(num == 2) {
		NRF_CSN2 = 1;
	}
  	return(status);						// ����״̬�Ĵ�������
}

//---------------------------------------
//����: 24L01���纯�� �����ܹر� ֹͣ����
//---------------------------------------
void NRF_Power_off(unsigned char num) {
	NRF_Write_Reg(num, NRF_W_REGISTER + NRF_CONFIG, 0x00);
}


unsigned char NRF_Check(unsigned char num) {
	unsigned char buf[5] = {0, 0, 0, 0, 0};
	unsigned char i;
	NRF_Write_Buf(num, NRF_W_REGISTER + NRF_TX_ADDR, TX_ADDRESS, 5);     // д��5���ֽڵĵ�ַ.
	NRF_Read_Buf(num, NRF_TX_ADDR, buf, 5);                              // ����д��ĵ�ַ
	if(num == 1) {
		for(i = 0; i < 5; i++) if(buf[i] != TX_ADDRESS[i]) break;
		}else if(num == 2) {
		for(i = 0; i < 5; i++) if(buf[i] != TX_ADDRESS2[i]) break;
	}
	if(i != 5) return 1;												// ���24L01����
	return 0;															// ��⵽24L01
}


//---------------------------------------
//����: �趨24L01Ϊ���ͷ�ʽ����------------
void NRF_TX_Mode(unsigned char num, unsigned char *address, unsigned char *txbuf) {

	if (num == 1) {
		NRF_CE = 0;
	}else if(num == 2) {
		NRF_CE2 = 0;
	}
    
    NRF_Write_Buf(num, NRF_W_REGISTER + NRF_TX_ADDR, address, 5);		// Writes TX_Address to nRF24L01 д���͵�ַ
    NRF_Write_Buf(num, NRF_W_REGISTER + NRF_RX_ADDR_P0, address, 5);	// RX_Addr0 same as TX_Adr for Auto.Ack ͨ��0��Ϊ����Ӧ���źţ��뷢�͵�ַҪһ��
    NRF_Write_Buf(num, NRF_W_TX_PAYLOAD, txbuf, TX_PLOAD_WIDTH);         // Writes data to TX payload д��������

    NRF_Write_Reg(num, NRF_W_REGISTER + NRF_EN_AA, 0x01);				 // Enable Auto.Ack:Pipe0
    NRF_Write_Reg(num, NRF_W_REGISTER + NRF_EN_RXADDR, 0x01);			// Enable Pipe0
    NRF_Write_Reg(num, NRF_W_REGISTER + NRF_SETUP_RETR, 0x1a);			// 500?s + 86?s, 10 retrans...
    NRF_Write_Reg(num, NRF_W_REGISTER + NRF_RF_CH, 40);					// Select RF channel 40
    NRF_Write_Reg(num, NRF_W_REGISTER + NRF_RF_SETUP, 0x0f);			// TX_PWR:0dBm, Datarate:2Mbps, LNA:HCURR
    NRF_Write_Reg(num, NRF_W_REGISTER + NRF_CONFIG, 0x0e);				// Set PWR_UP bit, enable CRC(2 bytes) & Prim:TX. MAX_RT & TX_DS enabled..

	if (num == 1) {
		NRF_CE = 1;
	}else if(num == 2) {
		NRF_CE2 = 1;
	}
}

//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:�������״��
unsigned char NRF_TxPacket(unsigned char num, unsigned char *address, unsigned char *txbuf) {
    
	unsigned char status = TX_DS;
    
	NRF_TX_Mode(num, address, txbuf);

    do {
        status = NRF_Read_Reg(num, NRF_STATUS);                      // ��ȡ״̬�Ĵ�����ֵ	 
        if(status & MAX_RT || status & TX_DS) break;
    }while(1);
    
    if(status & MAX_RT) {
        status = MAX_RT;
    }
    if(status & TX_DS) {
        status = TX_DS;
    }

	if (num == 1) {
		NRF_CE = 0;
		}else if(num == 2) {
		NRF_CE2 = 0;
	}
	
    NRF_Write_Reg(num, NRF_W_REGISTER + NRF_STATUS, 0xff);               // ���TX_DS��MAX_RT�жϱ�־
    NRF_Write_Reg(num, NRF_FLUSH_TX, 0);                              // ���RX FIFO�Ĵ��� 
    
    return status;
}

//---------------------------------------
//����: �趨24L01Ϊ���շ�ʽ����
//---------------------------------------
void NRF_RX_Mode(unsigned char num) {
    
	if (num == 1) {
		NRF_CE = 0;
		}else if(num == 2) {
		NRF_CE2 = 0;
	}
    
    NRF_Write_Buf(num, NRF_W_REGISTER + NRF_RX_ADDR_P0, RX_ADDRESS_P0, 5);   // Use the same address on the RX device as the TX device
    NRF_Write_Buf(num, NRF_W_REGISTER + NRF_RX_ADDR_P1, RX_ADDRESS_P1, 5);
    NRF_Write_Buf(num, NRF_W_REGISTER + NRF_RX_ADDR_P2, RX_ADDRESS_P2, 1);
    NRF_Write_Buf(num, NRF_W_REGISTER + NRF_RX_ADDR_P3, RX_ADDRESS_P3, 1);
    NRF_Write_Buf(num, NRF_W_REGISTER + NRF_RX_ADDR_P4, RX_ADDRESS_P4, 1);
    NRF_Write_Buf(num, NRF_W_REGISTER + NRF_RX_ADDR_P5, RX_ADDRESS_P5, 1);
  
  
    NRF_Write_Reg(num, NRF_W_REGISTER + NRF_EN_AA, 0x3F);                // Enable Auto.Ack:Pipe0 ʹ������ͨ���Զ�Ӧ��
    NRF_Write_Reg(num, NRF_W_REGISTER + NRF_EN_RXADDR, 0x3F);            // Enable Pipe0 ʹ������ͨ����������
    NRF_Write_Reg(num, NRF_W_REGISTER + NRF_RF_CH, 40);                  // Select RF channel 40
    NRF_Write_Reg(num, NRF_W_REGISTER + NRF_RX_PW_P0, RX_PLOAD_WIDTH);   // Select same RX payload width as TX Payload width
    NRF_Write_Reg(num, NRF_W_REGISTER + NRF_RX_PW_P1, RX_PLOAD_WIDTH);   // Select same RX payload width as TX Payload width
    NRF_Write_Reg(num, NRF_W_REGISTER + NRF_RX_PW_P2, RX_PLOAD_WIDTH);   // Select same RX payload width as TX Payload width
    NRF_Write_Reg(num, NRF_W_REGISTER + NRF_RX_PW_P3, RX_PLOAD_WIDTH);   // Select same RX payload width as TX Payload width
    NRF_Write_Reg(num, NRF_W_REGISTER + NRF_RX_PW_P4, RX_PLOAD_WIDTH);   // Select same RX payload width as TX Payload width
    NRF_Write_Reg(num, NRF_W_REGISTER + NRF_RX_PW_P5, RX_PLOAD_WIDTH);   // Select same RX payload width as TX Payload width
    NRF_Write_Reg(num, NRF_W_REGISTER + NRF_RF_SETUP, 0x0f);             // TX_PWR:0dBm, Datarate:2Mbps, LNA:HCURR
    NRF_Write_Reg(num, NRF_W_REGISTER + NRF_CONFIG, 0x0f);               // Set PWR_UP bit, enable CRC(2 bytes) & Prim:RX. RX_DR enabled..   

   	if (num == 1) {
	   	NRF_CE = 1;													// CEΪ��,�������ģʽ   
	   	}else if(num == 2) {
	   	NRF_CE2 = 1;
   	}                                                    
    
    _delay_us(150);                                                // 130?s�����ź�
}

//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:0��������ɣ��������������
unsigned char NRF_RxPacket(unsigned char num, unsigned char *rxbuf)
{
	unsigned char status = 0; 

	memset(rxbuf, 0, 2);
	
    if(NRF_Get_Status(num) & RX_DR) {                                        // ���յ�����     ;;;
        status = NRF_Read_RX_Pload(num, rxbuf);  
        NRF_Write_Reg(num, NRF_W_REGISTER + NRF_STATUS, 0xff);           // ���TX_DS��MAX_RT�жϱ�־
        NRF_Flush_RX(num);                                         // ���RX FIFO�Ĵ���      
        status = status + 1;
    }else {
        status = 0;
    } 
       
	return status;                                                   // û�յ��κ�����
}	

/*
 * Get current pipe#
 * ��ǰ��������ͨ����0-5��6��δʹ�� 7��RX FIFO �Ĵ���Ϊ�� 
 */
unsigned char NRF_Get_Current_Pipenum(unsigned char num) {
  return ((NRF_Get_Status(num) & RX_P_NO) >> 1);
}
unsigned char NRF_Read_RX_Pload(unsigned char num, unsigned char *pBuf) {
    unsigned char pipe, width;
    pipe = NRF_Get_Current_Pipenum(num);
    width = NRF_Read_Reg(num, NRF_RX_PW_P0 + pipe);
    NRF_Read_Buf(num, NRF_R_RX_PAYLOAD, pBuf, width);   
    return pipe;
}

/*
 * ��ȡ״̬�Ĵ���
 */
unsigned char NRF_Get_Status(unsigned char num) {
    return NRF_Read_Reg(num, NRF_STATUS);
}

/*
 * ��� TX FIFO �Ĵ���
 */
void NRF_Flush_TX(unsigned char num) {
    NRF_Write_Reg(num, NRF_FLUSH_TX, 0);
}

/*
 * ��� RX FIFO �Ĵ���
 */
void NRF_Flush_RX(unsigned char num) {
    NRF_Write_Reg(num, NRF_FLUSH_RX, 0);
}

/*
 void nRF24L01_IRQ(void) interrupt EXT_INT0
{
BYTE temp,rx_pw;

  EA = 0; // disable global interrupt during processing
  temp = SPI_RW_Reg(WRITE_REG + STATUS, 0x70);    // Read STATUS byte and clear IRQ flag's(nRF24L01)

  if(temp & MAX_RT) IRQ_Source = MAX_RT;          // Indicates max #of retransmit interrupt
  if(temp & TX_DS)  IRQ_Source = TX_DS;           // Indicates TX data succsessfully sent

  if(temp & RX_DR)  // In RX mode, check for data received
  {
    // Data received, so find out which datapipe the data was received on:
    temp = (0x07 & (temp > 1)); // Shift bits in status byte one bit to LSB and mask 'Data Pipe Number'
    rx_pw = SPI_Read(READ_REG + RX_PW_P0 + temp); // Read current RX_PW_Pn register, where Pn is the pipe the data was received on..
    SPI_Read_Buf(RD_RX_PLOAD, SPI_Buffer, rx_pw); // Data from RX Payload register is now copied to SPI_Buffer[].

    IRQ_Source = RX_DR; // Indicates RX data received
  }
  EA = 1; // enable global interrupt again
}
*/
 