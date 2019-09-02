#include "nrf24l01.h"

#include "stdlib.h"
#include <avr/io.h>
#include "io_168pa.h"
#include <util/delay.h>


// NRF24L01 相关I/O设置

#define		NRF_MOSI_DIR	DDRB3
#define		NRF_MOSI		PORTB3
#define		NRF_MISO_DIR	DDRB4
#define		NRF_MISO		PORTB4
#define		NRF_SCK_DIR		DDRB5
#define		NRF_SCK			PORTB5

#define     NRF_CE_DIR      DDRC4
#define		NRF_CE			PORTC4			// 第一个NRF使能端口
#define		NRF_CSN_DIR		DDRD7
#define		NRF_CSN			PORTD7			// 第一个NRF片选端口

#define     NRF_CE2_DIR     DDRD4			
#define		NRF_CE2			PORTD4			// 第二个NRF使能端口
#define		NRF_CSN2_DIR	DDRD2
#define		NRF_CSN2		PORTD2			// 第二个NRF片选端口     


// NRF24L01 供电 0：上电，1：断电（用于PIC芯片小板）

//#define     NRF_EN_DIR      TRISA5
//#define     NRF_EN          RA5

/*
 * 写地址时先写入低字节，在写入高字节
 * 通道2-5的地址的前4个字节[39:8]与通道1的地址通用，最后一位低字节不同
 */

unsigned char TX_ADDRESS[5] = {0x0F, 0x0E, 0x0D, 0x0C, 0x0B};			//设置发送接收地址
unsigned char TX_ADDRESS2[5] = {0x0F, 0x0E, 0x0D, 0x0C, 0x0B};			//设置发送接收地址
	
unsigned char RX_ADDRESS_P0[5] = {0x0F, 0x0E, 0x0D, 0x0C, 0x0B};		// 通道0接收地址
unsigned char RX_ADDRESS_P1[5] = {0x0A, 0x10, 0x09, 0x08, 0x07};		// 通道1接收地址
unsigned char RX_ADDRESS_P2[1] = {0x0B};								// 通道2接收地址
unsigned char RX_ADDRESS_P3[1] = {0x0C};								// 通道3接收地址
unsigned char RX_ADDRESS_P4[1] = {0x0D};								// 通道4接收地址
unsigned char RX_ADDRESS_P5[1] = {0x0E};								// 通道5接收地址

unsigned char RX_ADDRESS2_P0[5] = {0x0F, 0x0E, 0x0D, 0x0C, 0x0B};		// 通道0接收地址
unsigned char RX_ADDRESS2_P1[5] = {0x0A, 0x10, 0x09, 0x08, 0x07};		// 通道1接收地址
unsigned char RX_ADDRESS2_P2[1] = {0x0B};								// 通道2接收地址
unsigned char RX_ADDRESS2_P3[1] = {0x0C};								// 通道3接收地址
unsigned char RX_ADDRESS2_P4[1] = {0x0D};								// 通道4接收地址
unsigned char RX_ADDRESS2_P5[1] = {0x0E};								// 通道5接收地址
	
	
void NRF_Port_Init(void) {

	NRF_MOSI_DIR = 1;
	NRF_MISO_DIR = 0;
	NRF_SCK_DIR = 1;
	
	NRF_CSN_DIR = 1;
	NRF_CSN = 1;				// 初始片选状态为高电平 拉低后启动通讯
	
	NRF_CSN2_DIR = 1;
	NRF_CSN2 = 1;				
	
	NRF_CE_DIR = 1;	
	NRF_CE = 0;					// CE低电平为待机状态
		
	NRF_CE2_DIR = 1;
	NRF_CE2 = 0;
}


void NRF_SPI_Init(void) {
	SPCRbit->SPIE = 0;			// 中断使能位 需使用SPIF和SREG全局中断
	SPCRbit->SPE  = 1;			// 使能SPI
	SPCRbit->DORD = 0;			// 数据发送顺序，0 高位先发送  1 低位先发送
	SPCRbit->MSTR = 1;			// 主模式
	SPCRbit->CPOL = 0;			// SCK空闲时为低电平
	SPCRbit->CPHA = 0;			// 0 起始沿采样 1 结束沿采样
	
	SPSR = 0;					// 状态寄存器	
}


// SPI硬件通信模式
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
		NRF_CSN = 0;				// 片选启动
	}else if(num == 2) {
		NRF_CSN2 = 0;
	}                 
  	NRF_SPI_RW(reg);                    // 写入寄存器地址,
  	status = NRF_SPI_RW(NRF_NOP);    	// 随便写入寄存器内容,读出寄存器内容
	if (num == 1) {
		NRF_CSN = 1;				// 片选结束
	}else if(num == 2) {
		NRF_CSN2 = 1;
	}
  	return(status);                 // 返回寄存器内容
}


unsigned char NRF_Write_Reg(unsigned char num, unsigned char reg, unsigned char value) {
	unsigned char status;
	if (num == 1) {
		NRF_CSN = 0;				// 片选启动
	}else if(num == 2) {
		NRF_CSN2 = 0;
	}
  	status = NRF_SPI_RW(reg);           // 写入寄存器地址,同时读出状态寄存器内容
  	NRF_SPI_RW(value);                  // 写入寄存器内容
	if (num == 1) {
		NRF_CSN = 1;				// 片选结束
	}else if(num == 2) {
		NRF_CSN2 = 1;
	}
  	return(status);                 // 返回状态寄存器内容
}


unsigned char NRF_Read_Buf(unsigned char num, unsigned char reg, unsigned char *pBuf, unsigned char buf) {
	unsigned char status,i;
	if (num == 1) {
		NRF_CSN = 0;					// 片选启动
	}else if(num == 2) {
		NRF_CSN2 = 0;
	}
  	status = NRF_SPI_RW(reg);               // 写入寄存器地址
  	for(i = 0; i < buf; i++)            // 读取I个数据
	{
   		pBuf[i] = NRF_SPI_RW(NRF_NOP);    	// 读出寄存器内容放入数组
  	}
	if (num == 1) {
		NRF_CSN = 1;					// 片选结束
	}else if(num == 2) {
		NRF_CSN2 = 1;
	}
  	return(status);                     // 返回状态寄存器内容
}


unsigned char NRF_Write_Buf(unsigned char num, unsigned char reg, unsigned char *pBuf, unsigned char buf) {
	unsigned char status, i;
	if (num == 1) {
		NRF_CSN = 0;					// 片选启动
	}else if(num == 2) {
		NRF_CSN2 = 0;
	}
  	status = NRF_SPI_RW(reg);    			// 写入寄存器地址
  	for(i = 0; i < buf; i++) {			// 写入I个数据
    	NRF_SPI_RW(*pBuf++);				// 把数组内容写入寄存器
	}
	if (num == 1) {
		NRF_CSN = 1;					// 片选结束
	}else if(num == 2) {
		NRF_CSN2 = 1;
	}
  	return(status);						// 返回状态寄存器内容
}

//---------------------------------------
//名称: 24L01掉电函数 各功能关闭 停止工作
//---------------------------------------
void NRF_Power_off(unsigned char num) {
	NRF_Write_Reg(num, NRF_W_REGISTER + NRF_CONFIG, 0x00);
}


unsigned char NRF_Check(unsigned char num) {
	unsigned char buf[5] = {0, 0, 0, 0, 0};
	unsigned char i;
	NRF_Write_Buf(num, NRF_W_REGISTER + NRF_TX_ADDR, TX_ADDRESS, 5);     // 写入5个字节的地址.
	NRF_Read_Buf(num, NRF_TX_ADDR, buf, 5);                              // 读出写入的地址
	if(num == 1) {
		for(i = 0; i < 5; i++) if(buf[i] != TX_ADDRESS[i]) break;
		}else if(num == 2) {
		for(i = 0; i < 5; i++) if(buf[i] != TX_ADDRESS2[i]) break;
	}
	if(i != 5) return 1;												// 检测24L01错误
	return 0;															// 检测到24L01
}


//---------------------------------------
//名称: 设定24L01为发送方式函数------------
void NRF_TX_Mode(unsigned char num, unsigned char *address, unsigned char *txbuf) {

	if (num == 1) {
		NRF_CE = 0;
	}else if(num == 2) {
		NRF_CE2 = 0;
	}
    
    NRF_Write_Buf(num, NRF_W_REGISTER + NRF_TX_ADDR, address, 5);		// Writes TX_Address to nRF24L01 写发送地址
    NRF_Write_Buf(num, NRF_W_REGISTER + NRF_RX_ADDR_P0, address, 5);	// RX_Addr0 same as TX_Adr for Auto.Ack 通道0作为接收应答信号，与发送地址要一致
    NRF_Write_Buf(num, NRF_W_TX_PAYLOAD, txbuf, TX_PLOAD_WIDTH);         // Writes data to TX payload 写缓冲数据

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

//启动NRF24L01发送一次数据
//txbuf:待发送数据首地址
//返回值:发送完成状况
unsigned char NRF_TxPacket(unsigned char num, unsigned char *address, unsigned char *txbuf) {
    
	unsigned char status = TX_DS;
    
	NRF_TX_Mode(num, address, txbuf);

    do {
        status = NRF_Read_Reg(num, NRF_STATUS);                      // 读取状态寄存器的值	 
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
	
    NRF_Write_Reg(num, NRF_W_REGISTER + NRF_STATUS, 0xff);               // 清除TX_DS或MAX_RT中断标志
    NRF_Write_Reg(num, NRF_FLUSH_TX, 0);                              // 清除RX FIFO寄存器 
    
    return status;
}

//---------------------------------------
//名称: 设定24L01为接收方式函数
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
  
  
    NRF_Write_Reg(num, NRF_W_REGISTER + NRF_EN_AA, 0x3F);                // Enable Auto.Ack:Pipe0 使能所有通道自动应答
    NRF_Write_Reg(num, NRF_W_REGISTER + NRF_EN_RXADDR, 0x3F);            // Enable Pipe0 使能所有通道接收数据
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
	   	NRF_CE = 1;													// CE为高,进入接收模式   
	   	}else if(num == 2) {
	   	NRF_CE2 = 1;
   	}                                                    
    
    _delay_us(150);                                                // 130?s后检测信号
}

//启动NRF24L01发送一次数据
//txbuf:待发送数据首地址
//返回值:0，接收完成；其他，错误代码
unsigned char NRF_RxPacket(unsigned char num, unsigned char *rxbuf)
{
	unsigned char status = 0; 

	memset(rxbuf, 0, 2);
	
    if(NRF_Get_Status(num) & RX_DR) {                                        // 接收到数据     ;;;
        status = NRF_Read_RX_Pload(num, rxbuf);  
        NRF_Write_Reg(num, NRF_W_REGISTER + NRF_STATUS, 0xff);           // 清除TX_DS或MAX_RT中断标志
        NRF_Flush_RX(num);                                         // 清除RX FIFO寄存器      
        status = status + 1;
    }else {
        status = 0;
    } 
       
	return status;                                                   // 没收到任何数据
}	

/*
 * Get current pipe#
 * 当前接收数据通道号0-5，6：未使用 7：RX FIFO 寄存器为空 
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
 * 获取状态寄存器
 */
unsigned char NRF_Get_Status(unsigned char num) {
    return NRF_Read_Reg(num, NRF_STATUS);
}

/*
 * 清除 TX FIFO 寄存器
 */
void NRF_Flush_TX(unsigned char num) {
    NRF_Write_Reg(num, NRF_FLUSH_TX, 0);
}

/*
 * 清除 RX FIFO 寄存器
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
 