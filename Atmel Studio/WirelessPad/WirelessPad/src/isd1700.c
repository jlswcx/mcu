#include "isd1700.h"

#include <avr/io.h>
#include "io_168pa.h"
#include "spi.h"
#include <util/delay.h>



// 拨动开关的端口设置
#define		KEY_TYPE_DIR		DDRC5
//#define		KEY_TONE_DIR		ADC6
#define		KEY_COM_DIR			DDRB1

// 实际电路的端口配置

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
	SPCRbit->SPIE = 0;		// 中断使能位 需使用SPIF和SREG全局中断
	SPCRbit->SPE  = 1;		// 使能SPI
	SPCRbit->DORD = 1;		// 数据发送顺序，0 高位先发送  1 低位先发送
	SPCRbit->MSTR = 1;		// 主模式
	SPCRbit->CPOL = 1;		// SCK空闲时为高电平
	SPCRbit->CPHA = 0;		// 0 起始沿采样 1 结束沿采样

	SPSR = 0;				// 状态寄存器	
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
*    函数原型: void ISD_Pu(void);        发送指令后需要延迟50ms
*    功    能: 上电
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
*    函数原型: void ISD_Stop(void);
*    功    能: 停止当前操作
**************************************************************************/
void ISD_Stop(void) {
	ISD_SS = 0;
	ISD_SendData(ISD_STOP | ISD_LED);
	ISD_SendData(0x00);
	ISD_SS = 1;
}


/**************************************************************************
*    函数原型: void ISD_Reset(void);     发送指令后需要延迟50ms
*    功    能: ISD1700复位
**************************************************************************/
void ISD_Rest(void) {
	ISD_SS = 0;
	ISD_SendData(ISD_REST | ISD_LED);
	ISD_SendData(0x00);
	ISD_SS = 1;
}


/**************************************************************************
*    函数原型: void ISD_Clr_Int()(void);
*    功    能: 清除中断标志
**************************************************************************/
void ISD_Clr_Int(void) {
	ISD_SS = 0;
	ISD_SendData(ISD_CLR_INT | ISD_LED);
	ISD_SendData(0x00);
	ISD_SS = 1;
}


/**************************************************************************
*    函数原型: void ISD_Rd_Status(void);
*    功    能: 读取状态寄存器内容
**************************************************************************/
void ISD_Rd_Status(void) {
	
	unsigned char i ;

	ISD_SS = 0;
	ISD_SendData(ISD_RD_STAUS | ISD_LED);					// 发送
	ISD_SendData(0x00);
	ISD_SendData(0x00);
	ISD_SS = 1;

	for(i=0;i<5;i++){;}							//延迟
	
	ISD_SS = 0;
	Status[0]=ISD_SendData(ISD_RD_STAUS | ISD_LED);		// 从MISO 读出状态
	Status[1]=ISD_SendData(0x00);
	Status[2]=ISD_SendData(0x00);
	ISD_SS = 1;
}


/**************************************************************************
*    函数原型: void ISD_Rd_PlayPtr(void);
*    功    能: 读取放音指针
**************************************************************************/
void ISD_Rd_PlayPtr(void) {
	
	unsigned char i;
	
	ISD_SS = 0;
	ISD_SendData(ISD_RD_PLAY_PTR | ISD_LED);              // 发送
	ISD_SendData(0x00);
	ISD_SendData(0x00);
	ISD_SendData(0x00);
	ISD_SS = 1;

	for(i=5;i>0;i--){;}							//延迟
	
	ISD_SS = 0;
	Status[SR0L] = ISD_SendData(ISD_RD_PLAY_PTR | ISD_LED);		// 从MISO 读出状态
	Status[SR0H] = ISD_SendData(0x00);
	Status[VOCADDL] = ISD_SendData(0x00);				// 读取放音指针低8位
	Status[VOCADDH] = ISD_SendData(0x00);				// 读取放音指针高8位
	ISD_SS = 1;
}


/**************************************************************************
*    函数原型: void ISD_PD(void);        发送指令后需要延迟50ms
*    功    能: 下电
**************************************************************************/
void ISD_Pd(void) {
	ISD_SS = 0;
	ISD_SendData(ISD_PD | ISD_LED);            // 发送PD命令
	ISD_SendData(0x00);
	ISD_SS = 1;
}

/**************************************************************************
*    函数原型: void ISD_Rd_RecPtr(void);
*    功    能: 读取录音指针
**************************************************************************/
void ISD_Rd_RecPtr(void) {
	
	unsigned char i;
	
	ISD_SS = 0;
	ISD_SendData(ISD_RD_REC_PTR | ISD_LED);              // 发送
	ISD_SendData(0x00);
	ISD_SendData(0x00);
	ISD_SendData(0x00);
	ISD_SS = 1;

	for(i=5;i>0;i--) {;}            //延迟
	
	ISD_SS = 0;
	Status[SR0L] = ISD_SendData(ISD_RD_REC_PTR | ISD_LED);        // 从MISO 读出状态
	Status[SR0H] = ISD_SendData(0x00);
	Status[VOCADDL] = ISD_SendData(0x00);     // 读取录音指针低8位
	Status[VOCADDH] = ISD_SendData(0x00);     // 读取录音指针高8位
	ISD_SS = 1;
}


unsigned char ISD_Devid(void) {
	
	unsigned char i;
	
	ISD_SS = 0;
	ISD_SendData(ISD_DEVID | ISD_LED);
	ISD_SendData(0x00);
	ISD_SendData(0x00);
	ISD_SS = 1;

	for(i=5;i>0;i--){;}             //延迟
	
	ISD_SS = 0;
	ISD_SendData(ISD_DEVID | ISD_LED);
	ISD_SendData(0x00);
	Status[2] = ISD_SendData(0x00);
	ISD_SS = 1;

	return Status[2] ;
}


















/**************************************************************************
*    函数原型: void ISD_Play()(void);
*    功    能: 放音
**************************************************************************/
void ISD_Play(void) {
	
	ISD_SS = 0;
	
	ISD_SendData(ISD_PLAY | ISD_LED);
	ISD_SendData(0x00);
	
	ISD_SS = 1;
}











/**************************************************************************
*    函数原型: void ISD_Rec()(void);
*    功    能: 录音
**************************************************************************/
void ISD_rec(void) {
	
	ISD_SS = 0;
	ISD_SendData(ISD_REC);            // 发送录音命令
	ISD_SendData(0x00);
	ISD_SS = 1;
}

/**************************************************************************
*    函数原型: void ISD_Erase(void);
*    功    能: 擦除
**************************************************************************/
void ISD_erase(void) {
	
	ISD_SS = 0;
	ISD_SendData(ISD_ERASE);            // 发送擦除指令
	ISD_SendData(0x00);
	ISD_SS = 1;
}

/**************************************************************************
*    函数原型: void ISD_G_Erase(void);
*    功    能: 全体擦除
**************************************************************************/
void ISD_g_erase(void) {
	
	ISD_SS = 0;
	ISD_SendData(ISD_G_ERASE);            // 发送全体擦除命令
	ISD_SendData(0x00);
	ISD_SS = 1;
}

/**************************************************************************
*    函数原型: void ISD_RD_APC(void);
*    功    能: 读取APC 寄存器内容
**************************************************************************/
void ISD_rd_apc(void) {   
	
	unsigned char i;
	
	ISD_SS = 0;
	ISD_SendData(ISD_RD_APC);
	ISD_SendData(0x00);
	ISD_SendData(0x00);
	ISD_SendData(0x00);
	ISD_SS = 1;

	for(i=2;i>0;i--)             //延迟
	{;}

	ISD_SS = 0;
	Status[SR0L] = ISD_SendData(ISD_RD_APC);        // 从MISO 读出状态
	Status[SR0H] = ISD_SendData(0x00);
	Status[APCL] = ISD_SendData(0x00);     // 读取APC低8位
	Status[APCH] = ISD_SendData(0x00);     // 读取APC高8位
	ISD_SS = 1;
	
}

/**************************************************************************
*    函数原型: void ISD_WR_APC2(unsigned char apcdatl,apcdath);
*    功    能: 设置APC2
**************************************************************************/
void ISD_wr_apc2(unsigned char apcdatl,unsigned char apcdath) {
	
	ISD_SS = 0;
	ISD_SendData(ISD_WR_APC2);
	ISD_SendData(apcdatl);        //发送低8位数据
	ISD_SendData(apcdath);        //发送高8位数据
	ISD_SS = 1;


}


/**************************************************************************
*    函数原型: void ISD_WR_NVCFG(unsigned char apcdatl,apcdath);
*    功    能: 将APC数据写入NVCFG寄存器中
**************************************************************************/
void ISD_wr_nvcfg(unsigned char apcdatl,unsigned char apcdath) {
	
	ISD_SS = 0;
	ISD_SendData(ISD_WR_NVCFG);
	ISD_SendData(apcdatl);        //发送低8位数据
	ISD_SendData(apcdath);        //发送高8位数据
	ISD_SS = 1;
}

/**************************************************************************
*    函数原型: void ISD_LD_NVCFG(void);
*    功    能: 将NVCFG数据载入APC寄存器中
**************************************************************************/
void ISD_ld_nvcfg(void) {
	
	ISD_SS = 0;
	ISD_SendData(ISD_LD_NVCFG);
	ISD_SendData(0x00);
	ISD_SS = 1;
}

/**************************************************************************
*    函数原型: void ISD_Fwd(void);
*    功    能: 放音指针下跳一段，指向下一段语音的起始地址
**************************************************************************/
void ISD_Fwd(void) {
	
	ISD_SS = 0;
	
	ISD_SendData(ISD_FWD);
	ISD_SendData(0x00);
	
	ISD_SS = 1;
}


/**************************************************************************
*    函数原型: void ISD_CHK_MEM()(void);
*    功    能: 空间检查
**************************************************************************/

void ISD_Chk_Mem(void)
{
	ISD_SS = 0;
	ISD_SendData(ISD_CHK_MEM);
	ISD_SendData(0x00);

	ISD_SS = 1;
}

/**************************************************************************
*    函数原型: void ISD_EXTCLK(void);
*    功    能: 启用/关闭 外部时钟模式
**************************************************************************/

void ISD_extclk(void)
{
	ISD_SS = 0;
	ISD_SendData(ISD_EXTCLK);
	ISD_SendData(0x00);

	ISD_SS = 1;
}



/**************************************************************************
*    函数原型: void ISD_SET_PLAY( unsigned char Saddl，Saddh，Eaddl，Eaddh)
*    功    能: 定点播放
*
**************************************************************************/
void ISD_set_play( unsigned char Saddl,unsigned char Saddh,unsigned char Eaddl,unsigned char Eaddh) {

	ISD_SS = 0;
	
	ISD_SendData(ISD_SET_PLAY);
	ISD_SendData(0x00);
	ISD_SendData(Saddl);     // 开始地址低8位
	ISD_SendData(Saddh);     // 开始地址高8位
	ISD_SendData(Eaddl);     // 结束地址低8位
	ISD_SendData(Eaddh);     // 结束地址高8位
	ISD_SendData(0x00);

	ISD_SS = 1;

}

/**************************************************************************
*    函数原型: void ISD_SET_Rec( unsigned char Saddl，Saddh，Eaddl，Eaddh)
*    功    能: 定点录音
*
**************************************************************************/
void ISD_set_rec( unsigned char Saddl,unsigned char Saddh,unsigned char Eaddl,unsigned char Eaddh) {

	ISD_SS = 0;
	
	ISD_SendData(ISD_SET_REC);
	ISD_SendData(0x00);
	ISD_SendData(Saddl);     // 开始地址低8位
	ISD_SendData(Saddh);     // 开始地址高8位
	ISD_SendData(Eaddl);     // 结束地址低8位
	ISD_SendData(Eaddh);     // 结束地址高8位
	ISD_SendData(0x00);

	ISD_SS = 1;

}


/**************************************************************************
*    函数原型: void ISD_SET_Erase( unsigned char Saddl，Saddh，Eaddl，Eaddh)
*    功    能: 定点录音
*
**************************************************************************/
void ISD_set_erase( unsigned char Saddl,unsigned char Saddh,unsigned char Eaddl,unsigned char Eaddh) {

	ISD_SS = 0;
	
	ISD_SendData(ISD_SET_ERASE);
	ISD_SendData(0x00);
	ISD_SendData(Saddl);     // 开始地址低8位
	ISD_SendData(Saddh);     // 开始地址高8位
	ISD_SendData(Eaddl);     // 结束地址低8位
	ISD_SendData(Eaddh);     // 结束地址高8位
	ISD_SendData(0x00);

	ISD_SS = 1;

}
