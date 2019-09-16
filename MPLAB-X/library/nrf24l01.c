#include "nrf24l01.h"
#include "macro.h"

#define     _XTAL_FREQ        _XTAL_FREQ_125K       // 当前工程使用频率定义

// NRF24L01 相关I/O设置

#define     NRF_CE_DIR          TRISA0
#define     NRF_CE              RA0         // 24L01片选信号 拉高电平 发送接收数据
#define     NRF_CSN_DIR         TRISA2         
#define     NRF_CSN             RA2         // SPI SS 片选

#define     NRF_SCK_DIR         TRISC0
#define     NRF_SCK             RC0
#define     NRF_MOSI_DIR        TRISC1
#define     NRF_MOSI            RC1
#define     NRF_MISO_DIR        TRISC2
#define     NRF_MISO            RC2

#define     NRF_IRQ_DIR         TRISA4      // 有的没有适用此接口
#define     NRF_IRQ             RA4


// NRF24L01 供电 0：上电，1：断电  仅适用于 MH Pad 电路板
#define     NRF_EN_DIR          TRISA5
#define     NRF_EN              RA5


/*
 * 写地址时先写入低字节，再写入高字节
 * 通道2-5的地址的前4个字节[39:8]与通道1的地址通用，最后一位低字节不同
 */
unsigned char TX_ADDRESS[5] = {0x0F, 0x0E, 0x0D, 0x0C, 0x0B};           //设置发送接收地址

unsigned char RX_ADDRESS_P0[5] = {0x0F, 0x0E, 0x0D, 0x0C, 0x0B};        // 通道0接收地址
unsigned char RX_ADDRESS_P1[5] = {0x0A, 0x10, 0x09, 0x08, 0x07};        // 通道1接收地址
unsigned char RX_ADDRESS_P2[5] = {0x0B, 0x10, 0x09, 0x08, 0x07};        // 通道2接收地址
unsigned char RX_ADDRESS_P3[5] = {0x0C, 0x10, 0x09, 0x08, 0x07};        // 通道3接收地址
unsigned char RX_ADDRESS_P4[5] = {0x0D, 0x10, 0x09, 0x08, 0x07};        // 通道4接收地址
unsigned char RX_ADDRESS_P5[5] = {0x0E, 0x10, 0x09, 0x08, 0x07};        // 通道5接收地址


/*
 * 相关端口设置
 */
void NRF_Port_Init(void) {  
    NRF_MOSI_DIR = 1;
    NRF_MISO_DIR = 0;
    NRF_SCK_DIR = 0;
    NRF_CSN_DIR = 0;
    NRF_CE_DIR  = 0;
    
//    NRF_IRQ_DIR = 1;
//    NRF_EN_DIR = 0;  
    
    NRF_CE  = 0;            // 待机
    NRF_CSN = 1;            // 片选
}

void NRF_On(void) {
    NRF_EN = 0;             // 三极管控制上电
}

void NRF_Down(void) {
    NRF_EN = 1;             // 三极管控制掉电
}


/*
 * SPI通信设置
 */
void NRF_SPI_Init(void) {  
    SSP1STAT = 0x00;
    SSP1STATbits.CKE = 1;           // 时钟状态从空闲转换到有效时发送

    SSP1BUF = 0;                    // 清除缓存
    SSP1IF = 0;                     // 清除中断标志位          
    SSP1CON1 = 0x20;                // 使能SPI串口通信      
}

unsigned char NRF_SPI_RW(unsigned char dat) {
    SSP1IF = 0;                     // 
    SSP1BUF = dat;
    while(!SSP1IF);                 // 写完成后BF置1，读取SSP1BUF后BF会被清零
    
    return(SSP1BUF);
}  

unsigned char NRF_Read_Reg(unsigned char reg) {
	unsigned char status;
  	NRF_CSN = 0;                        // 片选有效
  	NRF_SPI_RW(reg);                    // 写入寄存器地址,
  	status = NRF_SPI_RW(NRF_NOP);    	// 随便写入寄存器内容,读出寄存器内容
  	NRF_CSN = 1;                        // 片选失效
  	return(status);                     // 返回寄存器内容
}
//---------------------------------------
//名称: 寄存器访问函数
//---------------------------------------
//函数描述：
//寄存器访问函数:用来设置24L01的寄存器的值,基本思路就是通过WRITE_REG命令(也就是
//0x20+寄存器地址)把要设定的值写到相应的寄存器地址里面去,并读取返回值.对于函数
//来说也就是把value值写到reg寄存器中。需要注意的是,访问NRF24L01之前首先要使能
//芯片(CSN=0),访问完以后失能芯片(CSN=1)
unsigned char NRF_Write_Reg(unsigned char reg, unsigned char value) {
	unsigned char status;
  	NRF_CSN = 0;                   	// 24L01片选有效
  	status = NRF_SPI_RW(reg);       // 写入寄存器地址,同时读出状态寄存器内容
  	NRF_SPI_RW(value);              // 写入寄存器内容
  	NRF_CSN = 1;                   	// 24L01片选失效
  	return(status);                 // 返回状态寄存器内容
}


//---------------------------------------
//名称: 接收缓冲区访问函数
//---------------------------------------
//函数描述：
//用来在接收时读取FIFO 缓冲区中的值,基本思路就是通过READ_REG命令把数据从接收FIF(RD_RX_PLOA)
//中读出并存到数组里面去。
unsigned char NRF_Read_Buf(unsigned char reg, unsigned char *pBuf, unsigned char len)
{
	unsigned char status,i;
  	NRF_CSN = 0;                        // 24L01片选有效
  	status = NRF_SPI_RW(reg);               // 写入寄存器地址
  	for(i = 0; i < len; i++)            // 读取I个数据
	{
   		pBuf[i] = NRF_SPI_RW(NRF_NOP);    	// 读出寄存器内容放入数组
  	}
	NRF_CSN = 1;                        // 24L01片选失效
  	return(status);                     // 返回状态寄存器内容
}


//---------------------------------------
//名称: 发射缓冲区访问函数--------------
//函数描述：
//要用来把数组里的数放到发射FIFO缓冲区中,基本思路就是通过WRITE_REG命令把数据存到发射
//FIFO(WR_TX_PLOAD)中去。
unsigned char NRF_Write_Buf(unsigned char reg, unsigned char *pBuf, unsigned char len)
{
	unsigned char status, i;
  	NRF_CSN = 0;                   	// 24L01片选有效
  	status = NRF_SPI_RW(reg);    		// 写入寄存器地址
  	for(i = 0; i < len; i++) {      // 写入I个数据
    	NRF_SPI_RW(*pBuf++);            // 把数组内容写入寄存器
	}
  	NRF_CSN = 1;                 		// 24L01片选失效
  	return(status);                 // 返回状态寄存器内容
}

//---------------------------------------
//名称: 24L01掉电函数 各功能关闭 停止工作
//---------------------------------------
void NRF_Power_off() {
    NRF_CE = 0;
    NRF_Write_Reg(NRF_W_REGISTER + NRF_CONFIG, 0x00);  
}

char NRF_Check(void) {
    unsigned char tuf[5] = {0xE1, 0xE2, 0xE3, 0xE4, 0x05};
	unsigned char buf[5] = {0, 0, 0, 0, 0};
	unsigned char i;
    NRF_Write_Buf(NRF_W_REGISTER + NRF_TX_ADDR, tuf, 5);        // 写入5个字节的地址
	NRF_Read_Buf(NRF_TX_ADDR, buf, 5);                          // 读出写入的地址  
	for(i = 0; i < 5; i++) if(tuf[i] != buf[i]) break;	 							   
	if(i != 5) return 1;                // 检测24L01错误	
	return 0;                           // 检测到24L01
}	

//---------------------------------------
//名称: 设定24L01为发送方式函数------------
void NRF_TX_Mode(unsigned char *txbuf) {

    NRF_CE = 0;
    
    NRF_Write_Buf(NRF_W_REGISTER + NRF_TX_ADDR, RX_ADDRESS_P0, 5);     // Writes TX_Address to nRF24L01 写发送地址
    NRF_Write_Buf(NRF_W_REGISTER + NRF_RX_ADDR_P0, RX_ADDRESS_P0, 5);  // RX_Addr0 same as TX_Adr for Auto.Ack 通道0作为接收应答信号，与发送地址要一致
    NRF_Write_Buf(NRF_W_TX_PAYLOAD, txbuf, TX_PLOAD_WIDTH);         // Writes data to TX payload 写缓冲数据

    NRF_Write_Reg(NRF_W_REGISTER + NRF_EN_AA, 0x01);            // Enable Auto.Ack:Pipe0
    NRF_Write_Reg(NRF_W_REGISTER + NRF_EN_RXADDR, 0x01);        // Enable Pipe0
    NRF_Write_Reg(NRF_W_REGISTER + NRF_SETUP_RETR, 0x1a);       // 500?s + 86?s, 10 retrans...
    NRF_Write_Reg(NRF_W_REGISTER + NRF_RF_CH, 40);              // Select RF channel 40
    NRF_Write_Reg(NRF_W_REGISTER + NRF_RF_SETUP, 0x0f);         // TX_PWR:0dBm, Datarate:2Mbps, LNA:HCURR
    NRF_Write_Reg(NRF_W_REGISTER + NRF_CONFIG, 0x0e);           // Set PWR_UP bit, enable CRC(2 bytes) & Prim:TX. MAX_RT & TX_DS enabled..

    NRF_CE = 1;

}

//启动NRF24L01发送一次数据
//txbuf:待发送数据首地址
//返回值:发送完成状况
unsigned char NRF_TxPacket(unsigned char *txbuf) {
    
	unsigned char status = TX_DS;
    
	NRF_TX_Mode(txbuf);

    do {
        status = NRF_Read_Reg(NRF_STATUS);                      // 读取状态寄存器的值	 
        if(status & MAX_RT || status & TX_DS) break;
    }while(1);
    
    if(status & MAX_RT) {
        status = MAX_RT;
    }
    if(status & TX_DS) {
        status = TX_DS;
    }

    NRF_CE = 0;
    NRF_Write_Reg(NRF_W_REGISTER + NRF_STATUS, 0xff);               // 清除TX_DS或MAX_RT中断标志
    NRF_Write_Reg(NRF_FLUSH_TX, 0);                              // 清除RX FIFO寄存器 
    
    return status;
}

/*
 * 名称: 设定24L01为接收方式函数
 * 启动所有通道接收数据
 */
void NRF_RX_Mode() {
    
    unsigned char enaa = 0x00;
    
    NRF_CE = 0;	  
    

    NRF_Write_Buf(NRF_W_REGISTER + NRF_RX_ADDR_P0, RX_ADDRESS_P0, 5);       // Use the same address on the RX device as the TX device
                                                                                // Select same RX payload width as TX Payload width
    NRF_Write_Buf(NRF_W_REGISTER + NRF_RX_ADDR_P1, RX_ADDRESS_P1, 5);       
    NRF_Write_Buf(NRF_W_REGISTER + NRF_RX_ADDR_P2, RX_ADDRESS_P2, 1);
    NRF_Write_Buf(NRF_W_REGISTER + NRF_RX_ADDR_P3, RX_ADDRESS_P3, 1);
    NRF_Write_Buf(NRF_W_REGISTER + NRF_RX_ADDR_P4, RX_ADDRESS_P4, 1);
    NRF_Write_Buf(NRF_W_REGISTER + NRF_RX_ADDR_P5, RX_ADDRESS_P5, 1);

  
    NRF_Write_Reg(NRF_W_REGISTER + NRF_EN_AA, 0x3F);                            // Enable Auto.Ack:Pipe0 使能所有通道自动应答
    NRF_Write_Reg(NRF_W_REGISTER + NRF_EN_RXADDR, 0x3F);                        // Enable Pipe0 使能所有通道接收数据
    NRF_Write_Reg(NRF_W_REGISTER + NRF_RF_CH, 40);                              // Select RF channel 40
     
    NRF_Write_Reg(NRF_W_REGISTER + NRF_RX_PW_P0, RX_PLOAD_WIDTH); 
    NRF_Write_Reg(NRF_W_REGISTER + NRF_RX_PW_P1, RX_PLOAD_WIDTH); 
    NRF_Write_Reg(NRF_W_REGISTER + NRF_RX_PW_P2, RX_PLOAD_WIDTH); 
    NRF_Write_Reg(NRF_W_REGISTER + NRF_RX_PW_P3, RX_PLOAD_WIDTH); 
    NRF_Write_Reg(NRF_W_REGISTER + NRF_RX_PW_P4, RX_PLOAD_WIDTH); 
    NRF_Write_Reg(NRF_W_REGISTER + NRF_RX_PW_P5, RX_PLOAD_WIDTH); 
      
      
    
    NRF_Write_Reg(NRF_W_REGISTER + NRF_RF_SETUP, 0x0f);                         // TX_PWR:0dBm, Datarate:2Mbps, LNA:HCURR
    NRF_Write_Reg(NRF_W_REGISTER + NRF_CONFIG, 0x0f);                           // Set PWR_UP bit, enable CRC(2 bytes) & Prim:RX. RX_DR enabled..   

    NRF_CE = 1;                                                                 // CE为高,进入接收模式   
    
    __delay_us(200);
}

/*
 * 接收数据函数
 * txbuf:待发送数据首地址
 * 返回值:0，接收完成；其他，错误代码
 */
unsigned char NRF_RxPacket(unsigned char *rxbuf)
{
	unsigned char status = 0; 

    if(NRF_Get_Status() & RX_DR) {                                        // 接收到数据     ;;;
        status = NRF_Read_RX_Pload(rxbuf);  
        NRF_Write_Reg(NRF_W_REGISTER + NRF_STATUS, 0xff);           // 清除TX_DS或MAX_RT中断标志
        NRF_Flush_RX();                                         // 清除RX FIFO寄存器 
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
unsigned char NRF_Get_Current_Pipenum(void) {
  return ((NRF_Get_Status() & RX_P_NO) >> 1);
}

/**
 * 读取接受到的数据
 * @param pBuf
 * @return 
 */
unsigned char NRF_Read_RX_Pload(unsigned char *pBuf) {
    unsigned char pipe, width;
    pipe = NRF_Get_Current_Pipenum();
    width = NRF_Read_Reg(NRF_RX_PW_P0 + pipe);
    NRF_Read_Buf(NRF_R_RX_PAYLOAD, pBuf, width);   
    return pipe;
}

/*
 * 获取状态寄存器
 */
unsigned char NRF_Get_Status(void) {
    return NRF_Read_Reg(NRF_STATUS);
}

/*
 * 清除 TX FIFO 寄存器
 */
void NRF_Flush_TX(void) {
    NRF_Write_Reg(NRF_FLUSH_TX, 0);
}

/*
 * 清除 RX FIFO 寄存器
 */
void NRF_Flush_RX(void) {
    NRF_Write_Reg(NRF_FLUSH_RX, 0);
}

unsigned char NRF_Clear_IRQ(void) {
    
    unsigned char IRQ_FLAG;             // 中断状态标志位
    
    unsigned char temp, rx_pw;
    unsigned char* buf;
    
    temp = NRF_Write_Reg(NRF_W_REGISTER + NRF_STATUS, 0x70);    // Read STATUS byte and clear IRQ flag's(nRF24L01)

    if(temp & MAX_RT) IRQ_FLAG = MAX_RT;                      // Indicates max #of retransmit interrupt
    if(temp & TX_DS)  IRQ_FLAG = TX_DS;                       // Indicates TX data succsessfully sent

    if(temp & RX_DR)  // In RX mode, check for data received
    {
        // Data received, so find out which datapipe the data was received on:
//        temp = (0x07 & (temp > 1)); // Shift bits in status byte one bit to LSB and mask 'Data Pipe Number'
//        rx_pw = NRF_Read_Reg(NRF_RX_PW_P0 + temp); // Read current RX_PW_Pn register, where Pn is the pipe the data was received on..
//        NRF_Read_Buf(NRF_R_RX_PAYLOAD, buf, rx_pw); // Data from RX Payload register is now copied to SPI_Buffer[].

        IRQ_FLAG = RX_DR; // Indicates RX data received
    }    
    
    return IRQ_FLAG;
}

/*
 ********************************** 新方案 *************************************
 */

/*
 * 发送模式
 * NRF_Tx_Set 函数（发送地址，是否启动自动应答）
 * 1、设置发送给地址，启动自动应答，自动应答接收地址，启用应答通道0
 * 2、设置自动重发，发射频率，配置发射模式
 * 
 * NRF_Tx_Send 函数（发送数据，数据长度）
 * 1、CE 置高，发送数据
 * 2、验证自动应答接收信号
 */
void NRF_Tx_Set(unsigned char *addr, unsigned char ack) {
    NRF_CE = 0;
    NRF_Write_Buf(NRF_W_REGISTER + NRF_TX_ADDR, addr, 5);       // 写发送地址
    NRF_Write_Buf(NRF_W_REGISTER + NRF_RX_ADDR_P0, addr, 5);    // 自动应答时使用通道0为 接收地址，与发送地址要一致
    NRF_Write_Reg(NRF_W_REGISTER + NRF_EN_AA, ack);             // Enable Auto.Ack:Pipe0
    
    NRF_Write_Reg(NRF_W_REGISTER + NRF_EN_RXADDR, 0x01);        // Enable Pipe0
    NRF_Write_Reg(NRF_W_REGISTER + NRF_SETUP_RETR, 0x1a);       // 500?s + 86?s, 10 retrans... 延时自动重发
    NRF_Write_Reg(NRF_W_REGISTER + NRF_RF_CH, 40);              // Select RF channel 40
    NRF_Write_Reg(NRF_W_REGISTER + NRF_RF_SETUP, 0x0f);         // TX_PWR:0dBm, Datarate:2Mbps, LNA:HCURR
    NRF_Write_Reg(NRF_W_REGISTER + NRF_CONFIG, 0x0e);           // Set PWR_UP bit, enable CRC(2 bytes) & Prim:TX. MAX_RT & TX_DS enabled.. 
}

void NRF_Tx_Send(unsigned char *txbuf, unsigned char len) {
    NRF_CE = 0;
    NRF_Write_Buf(NRF_W_TX_PAYLOAD, txbuf, len);        // 写缓冲数据，数据长度，不能超过32个字节
    NRF_CE = 1;
    __delay_us(10);
    while(NRF_Tx_CheckACK());
}

unsigned char NRF_Tx_CheckACK(void) {  
    unsigned char sta = 0;

	sta = NRF_Read_Reg(NRF_STATUS);                 // 读取状态寄存器的值	 
    if(sta & MAX_RT || sta & TX_DS) {
        NRF_Write_Reg(NRF_W_REGISTER + NRF_STATUS, 0xff);    // 清除中断标志，否则不会重发数据
    }
	if(sta & TX_DS) {                               // 发送完毕中断,退出循环（是否清除了 TX_FIFO?）
	   return(0);
	}
	return(1);
}

/*
 * 接收模式
 * NRF_Rx_Set 函数（启用接收应答，启用接收数据通道）
 * 1、设置哪些通道自动应答，设置哪些通道被启用接收，
 * 2、如设置通道0-2为接收数据，则设置 ack 参数为 0x03
 *    然后调用 NRF_Rx_Set_P0(--2) 3个函数 
 * {
 * NRF_Rx_Set_P0 函数（通道0地址，接收数据长度）
 * NRF_Rx_Set_P1 函数（通道1地址，接收数据长度）
 * NRF_Rx_Set_P2 函数（通道2地址，接收数据长度）
 * NRF_Rx_Set_P3 函数（通道3地址，接收数据长度）
 * NRF_Rx_Set_P4 函数（通道4地址，接收数据长度）
 * NRF_Rx_Set_P5 函数（通道5地址，接收数据长度）
 * }
 * 
 * NRF_Rx_Receive 函数
 * 1、CE 置高 ，启用接收
 * 
 * NRF_RxPacket 函数（数据缓冲区）
 * 1、在循环众调用此函数，不断接收数据
 */
// 
void NRF_Rx_Set(unsigned char ack, unsigned char pipe) {
    NRF_CE = 0;
    NRF_Write_Reg(NRF_W_REGISTER + NRF_EN_AA, ack);                            // Enable Auto.Ack:Pipe0 使能所有通道自动应答
    NRF_Write_Reg(NRF_W_REGISTER + NRF_EN_RXADDR, pipe);                        // Enable Pipe0 使能所有通道接收数据
    
    NRF_Write_Reg(NRF_W_REGISTER + NRF_RF_CH, 40);                              // Select RF channel 40   
    NRF_Write_Reg(NRF_W_REGISTER + NRF_RF_SETUP, 0x0f);                         // TX_PWR:0dBm, Datarate:2Mbps, LNA:HCURR
}

void NRF_Rx_Set_P0(unsigned char *addr, unsigned char buf_len) {
    NRF_CE = 0;
    NRF_Write_Buf(NRF_W_REGISTER + NRF_RX_ADDR_P0, addr, 5);       // Use the same address on the RX device as the TX device
    NRF_Write_Reg(NRF_W_REGISTER + NRF_RX_PW_P0, buf_len);  
}

void NRF_Rx_Set_P1(unsigned char *addr, unsigned char buf_len) {
    NRF_CE = 0;
    NRF_Write_Buf(NRF_W_REGISTER + NRF_RX_ADDR_P1, addr, 5);       // Use the same address on the RX device as the TX device
    NRF_Write_Reg(NRF_W_REGISTER + NRF_RX_PW_P1, buf_len);  
}

void NRF_Rx_Set_P3(unsigned char *addr, unsigned char buf_len) {
    NRF_CE = 0;
    NRF_Write_Buf(NRF_W_REGISTER + NRF_RX_ADDR_P3, addr, 5);       // Use the same address on the RX device as the TX device
    NRF_Write_Reg(NRF_W_REGISTER + NRF_RX_PW_P3, buf_len);  
}

void NRF_Rx_Set_P4(unsigned char *addr, unsigned char buf_len) { 
    NRF_CE = 0;
    NRF_Write_Buf(NRF_W_REGISTER + NRF_RX_ADDR_P4, addr, 5);       // Use the same address on the RX device as the TX device
    NRF_Write_Reg(NRF_W_REGISTER + NRF_RX_PW_P4, buf_len);  
}

void NRF_Rx_Set_P5(unsigned char *addr, unsigned char buf_len) {
    NRF_CE = 0;
    NRF_Write_Buf(NRF_W_REGISTER + NRF_RX_ADDR_P5, addr, 5);       // Use the same address on the RX device as the TX device
    NRF_Write_Reg(NRF_W_REGISTER + NRF_RX_PW_P5, buf_len);  
}

void NRF_Rx_Receive(void) {
    NRF_CE = 0;	  
    NRF_Write_Reg(NRF_W_REGISTER + NRF_CONFIG, 0x0f);                           // Set PWR_UP bit, enable CRC(2 bytes) & Prim:RX. RX_DR enabled..   
    NRF_CE = 1;                                                                 // CE为高,进入接收模式    
    __delay_us(200);
}
