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

// 1700 SPI 指令表

#define ISD_PU          0x01			// 上电
#define ISD_STOP        0X02			// 停止当前操作
#define ISD_REST        0x03			// 芯片复位
#define ISD_CLR_INT     0x04			// 清除EOM标志和中断信号
#define ISD_RD_STAUS    0x05			// 返回状态寄存器内容
#define ISD_RD_PLAY_PTR 0x06			// 在1,2字节返回状态寄存器信息，2，3字节返回放音指针
#define ISD_PD          0x07			// 芯片下电
#define ISD_RD_REC_PTR  0x08			// 在1,2字节返回状态寄存器信息，2，3字节返回录音指针
#define ISD_DEVID       0x09			// 读取芯片ID号码

#define ISD_PLAY        0x40			// 从当前地址放音
#define ISD_REC         0x41			// 从当前地址录音
#define ISD_ERASE       0x42			// 擦除当前语音段
#define ISD_G_ERASE     0x43			// 擦除芯片所有语音?
#define ISD_RD_APC      0x44			// 返回状态寄存器信息在1,2字节，APC寄存器信息在3字节
#define ISD_WR_APC1     0x45			// 将<D11:D0>写入APC寄存器
#define ISD_WR_APC2     0x65			// 将<D11:D0>写入APC寄存器,并由<D2:D0>位控制音量

#define ISD_WR_NVCFG    0x46			// 将APC内容写入NVCFG
#define ISD_LD_NVCFG    0x47			// 将NVCFG内容写入APC
#define ISD_FWD         0x48			// 将放音指针指向下一段语音地址处
#define ISD_CHK_MEM     0x49			// 检查环状存储体系
#define ISD_EXTCLK      0x4A			// 外部时钟使能
#define ISD_SET_PLAY    0x80			// 从起始地址<S10:S0>放音到结束地址<E10:E0>结束
#define ISD_SET_REC     0x81			// 从起始地址<S10:S0>录音到结束地址<E10:E0>结束
#define ISD_SET_ERASE   0x82			// 从起始地址<S10:S0>擦除到结束地址<E10:E0>结束

#define ISD_NOP			0x00
#define ISD_LED         0x10			// 使能LED

 // ISD1700 SPI返回值设置
 
 #define SR0L			0				// 状态信息低字节
 #define SR0H			1				// 状态信息高字节
 #define SR1			2
 #define DATA1			2
 #define DATA2			3
 #define VOCADDL		2
 #define VOCADDH		3
 #define APCL			2
 #define APCH			3
 
  // ISD1700状态寄存器位SR0
  
  #define CMD_ERR		0
  #define FULL			1
  #define PU			2
  #define EOM			3
  #define INT			4
  
  // ISD1700状态寄存器位SR1
  
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