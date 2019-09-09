#include "spi.h"

#include <avr/io.h>
#include "io_168pa.h"

/*
 *		SPCR			控制寄存器
 *			SPIE		中断使能
 *			SPE			SPI使能
 *			DORD		0：MSB首先发送	1：LSB首先发送
 *			MSTR		1：主机模式	0：从机模式
 *			CPOL		时钟极性		0：SCK空闲为低电平	1：SCK空闲为高电平
 *			CPHA		时钟相位		0：起始沿采样		1: 结束沿采样
 *			SPR1,SPR0	时钟速率选择
 *		SPSR			状态寄存器
 *			SPIF		中断标志位
 *			WCOL		写冲突标志位
 *		SPDR			数据寄存器			
			
 *
 *      正确初始化I/O端口数据方向
 *      SDI 必须将相应的TRIS 位置1  SDI是MOSI,指主器件数据输出,从器件数据输入
 *      SDO 必须将相应的TRIS 位清零 SDO是MISO,指主器件数据输入,从器件数据输出
 *      SCK （主模式）必须将相应的TRIS 位清零
 *      SCK （从模式）必须将相应的TRIS 位置1
 *      SS 必须将相应的TRIS 位置1（从器件）
*/


#define SPI_MOSI_DIR		DDRB3
#define SPI_MOSI			PORTB3
#define SPI_MISO_DIR		DDRB4
#define SPI_MISO			PORTB4
#define SPI_SCK_DIR			DDRB5
#define SPI_SCK				PORTB5

#define SPI_CSN_DIR			DDRD2
#define SPI_CSN				PORTD2         // SPI SS 片选


void SPI_Port_Init(void) {
	SPI_MOSI_DIR = 1;
	SPI_MISO_DIR = 0;
	SPI_SCK_DIR = 1;
	SPI_CSN_DIR = 1;
	
	SPI_CSN = 1;                // 片选处于高电平
}


void SPI_Init(void) {  
	
	SPI_Port_Init();		// 端口初始化
	
	SPCRbit->SPIE = 0;		// 中断使能位 需使用SPIF和SREG全局中断
	SPCRbit->SPE  = 1;		// 使能SPI
	SPCRbit->DORD = 0;		// 数据发送顺序，0 高位先发送  1 低位先发送
	SPCRbit->MSTR = 1;		// 主模式
	SPCRbit->CPOL = 0;		// SCK空闲时为低电平
	SPCRbit->CPHA = 0;		// 0 起始沿采样 1 结束沿采样
	
	SPSR = 0;

}

// SPI硬件通信模式
unsigned char SPI_RW(unsigned char dat) {
	
    SPDR = dat;
	while(!SPSRbit->SPIF);
	dat = SPDR;
	
	return dat;
}




