/* 
 * File:   nrf24l01.h
 * Author: 梦晓星辰
 *
 * Created on 2017年12月14日, 下午12:36
 */

#ifndef NRF24L01_H
#define	NRF24L01_H

#ifdef	__cplusplus
extern "C" {
#endif

//******************************************************************************
// NRF24L01寄存器操作命令（一共8个）
#define NRF_R_REGISTER          0x00    // 读配置寄存器,低5位为寄存器地址
#define NRF_W_REGISTER          0x20    // 写配置寄存器,低5位为寄存器地址，在掉电和待机模式下操作
#define NRF_R_RX_PAYLOAD        0x61    // 读RX有效数据,1~32字节
#define NRF_W_TX_PAYLOAD        0xA0    // 写TX有效数据,1~32字节
#define NRF_FLUSH_TX            0xE1    // 清除TX FIFO寄存器.发射模式下用
#define NRF_FLUSH_RX            0xE2    // 清除RX FIFO寄存器.接收模式下用
#define NRF_REUSE_TX_PL         0xE3    // 重新使用上一包数据,CE为高,数据包被不断发送.
#define NRF_R_RX_PL_WID         0x60    //
#define NRF_W_ACK_PAYLOAD       0xA8    // 用于接收模式
#define NRF_W_TX_PAYLOAD_NOACK  0xB0    // 用于发射模式
#define NRF_NOP                 0xFF    // 空操作,可以用来读状态寄存器
//******************************************************************************
// NRF24L01寄存器（一共24个）
#define NRF_CONFIG              0x00    // bit7：reserved 0
                                        // bit6：MASK_RX_DR 
                                        // bit5：MASK_TX_DS
                                        // bit4：MASK_MAX_RT
                                        // bit3：EN_CRC  CRC 使能 如果EN_AA任意一位为高，则EN_CRC强制为高
                                        // bit2：CRCO
                                        // bit1：PWR_UP  1：上电    0：掉电
                                        // bit0：PRIM_RX  0：发射模式   1：接收模式
#define NRF_EN_AA               0x01    // 使能自动应答功能  bit5~0,对应通道5~0 默认所有通道自动应答允许 00111111
#define NRF_EN_RXADDR           0x02    // 接收地址允许,bit5~0,对应通道5~0 默认 00000011
#define NRF_SETUP_AW            0x03    // 设置地址宽度(所有数据通道):bit1~0  00：无效 01：3字节 10：4字节 11：5字节
#define NRF_SETUP_RETR          0x04    // 建立自动重发;bit3~0，自动重发计数器;bit7~4，自动重发延时 250*x+86us
#define NRF_RF_CH               0x05    // 射频通道
#define NRF_RF_SETUP            0x06    // 射频寄存器
#define NRF_STATUS              0x07    // 状态寄存器;bit0:TX FIFO满标志;bit3:1,接收数据通道号(最大:6);bit4,达到最多次重发
                                        // bit5:数据发送完成中断;bit6:接收数据中断;
#define NRF_OBSERVE_TX          0x08    // 发送检测寄存器,bit7:4,数据包丢失计数器;bit3:0,重发计数器
#define NRF_CD					0x09    // 载波检测寄存器,bit0,载波检测;
#define NRF_RX_ADDR_P0          0x0A    // 数据通道0接收地址,最大长度5个字节,低字节在前
#define NRF_RX_ADDR_P1          0x0B    // 数据通道1接收地址,最大长度5个字节,低字节在前
#define NRF_RX_ADDR_P2          0x0C    // 数据通道2接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
#define NRF_RX_ADDR_P3          0x0D    // 数据通道3接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
#define NRF_RX_ADDR_P4          0x0E    // 数据通道4接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
#define NRF_RX_ADDR_P5          0x0F    // 数据通道5接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
#define NRF_TX_ADDR             0x10    // 发送地址(低字节在前),ShockBurstTM模式下,RX_ADDR_P0与此地址相等
#define NRF_RX_PW_P0            0x11    // 接收数据通道0有效数据宽度(1~32字节),设置为0则非法
#define NRF_RX_PW_P1            0x12    // 接收数据通道1有效数据宽度(1~32字节),设置为0则非法
#define NRF_RX_PW_P2            0x13    // 接收数据通道2有效数据宽度(1~32字节),设置为0则非法
#define NRF_RX_PW_P3            0x14    // 接收数据通道3有效数据宽度(1~32字节),设置为0则非法
#define NRF_RX_PW_P4            0x15    // 接收数据通道4有效数据宽度(1~32字节),设置为0则非法
#define NRF_RX_PW_P5            0x16    // 接收数据通道5有效数据宽度(1~32字节),设置为0则非法
#define NRF_FIFO_STATUS         0x17    // FIFO状态寄存器;bit0,RX FIFO寄存器空标志;bit1,RX FIFO满标志;bit2,3,保留
                                        // bit4,TX FIFO空标志;bit5,TX FIFO满标志;bit6,1,循环发送上一数据包.0,不循环;
#define NRF_DYNPD               0x1C    // 
#define NRF_FEATURE             0x1D    //
    
// ACK_PLD
// TX_PLD
// RX_PLD    
//******************************************************************************
#define MAX_RT					0X10    // MAX_RTD在第5位| 达到最大发送次数中断
#define TX_DS					0X20    // TX_DS在第6位  | 在读回STATUS时与其相与，就知道该位是否为1 TX发送完成中断
#define RX_DR					0X40    // RX_DR在第7位  | 接收到数据中断

#define RX_P_NO					0x0E    // 状态寄存器的 接收数据通道号
//******************************************************************************
#define TX_ADR_WIDTH			5		// 发送地址宽度，即40位
#define RX_ADR_WIDTH			5		// 接受地址宽度
#define TX_PLOAD_WIDTH			2		// 发送数据长度（字节数，最大32字节）
#define RX_PLOAD_WIDTH			2		// 接受数据长度
//******************************************************************************

//unsigned char Rx_buf[TX_PLOAD_WIDTH];	//接收数据缓冲区
//******************************************************************************
//#define RX_P_NO     0x0E				// 接收数据通道号
//#define TX_FULL     0X01				// FIFO寄存器满标志，1满  0有可用空间
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

