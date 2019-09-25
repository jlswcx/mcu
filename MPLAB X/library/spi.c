#include "spi.h"
#include <pic.h>

/*
 *      SSP1CON1        控制寄存器
 *          WCOL        写冲突检测位置1，清零才能使以后对SSP1BUF的写入成功完成
 *          SSPOV
 *          SSPEN       使能SPI，复位或重新配置时，清零，重新初始化SSPICONx寄存器，再置1
 *          CKP         SPI时钟极性控制 0：SCLK空闲为低电平   1：SCLK空闲为高电平
 *          SSP1M       时钟频率
 *      SSP1STAT        状态寄存器
 *          SMP         数据输入采样位，0：前时钟沿采样 1：后时钟沿采样
 *          CKE         时钟边沿选择位，0：时钟前沿驱动，1：时钟空闲状态时驱动（理解有点绕脑）
 *          BF          SSP1BUF缓冲寄存器满检测位置1，SSP1BUF被读取后会被清零
 *          
 *      SSP1BUF         数据缓冲寄存器，用于操作SSP1SR寄存器，进行数据发送/接收
 *      
 *      SSP1IF          发送/接收中断标志置，
 
 *      正确初始化I/O端口数据方向
 *      SDI 必须将相应的TRIS 位置1  SDI是MISO,指主器件数据输入,从器件数据输出
 *      SDO 必须将相应的TRIS 位清零 SDO是MOSI,指主器件数据输出,从器件数据输入
 *      SCK （主模式）必须将相应的TRIS 位清零
 *      SCK （从模式）必须将相应的TRIS 位置1
 *      SS 必须将相应的TRIS 位置1（从器件）
 * 
 * 
 *      注意：必须对SPI端口的 ANSEL 模拟端口初始化清零，否则无法使用 SPI
 * 
 *      关于 SDI、SDO、MOSI、MISO 的理解
 *      SDI 为数据输入，即接收数据，所以需要链接的从器件为输出，即 MISO
 *      SDO 为数据输出，与 SDI 刚好相反
 */

#define     SPI_SCK_DIR     TRISC0          // SCK
#define     SPI_SCK         RC0
#define     SPI_MOSI_DIR    TRISC1          // MOSI   
#define     SPI_MOSI        RC1    
#define     SPI_MISO_DIR    TRISC2          // MISO
#define     SPI_MISO        RC2

#define     SPI_CSN_DIR     TRISA2          // SS
#define     SPI_CSN         RA2


void SPI_Port_Init() {
    SPI_MOSI_DIR = 1;
    SPI_MISO_DIR = 0;
    SPI_SCK_DIR = 0;
    SPI_CSN_DIR = 0;
    
    SPI_CSN = 1;                    // 片选处于高电平     
}


void SPI_Init() {  
    SPI_Port_Init();
    
    SSP1STAT = 0x00;
    SSP1STATbits.CKE = 1;           // 时钟状态从空闲转换到有效时发送

    SSP1BUF = 0;                    // 清除缓存
    SSP1IF = 0;                     // 清除中断标志位          
    SSP1CON1 = 0x20;                // 使能SPI串口通信      
}


unsigned char SPI_RW(unsigned char dat) {
    SSP1IF = 0;                     // 
    SSP1BUF = dat;
    while(!SSP1IF);                 // 写完成后BF置1，读取SSP1BUF后BF会被清零
    
    return(SSP1BUF);
}  


unsigned char SPI_RW_SW(unsigned char dat) {
    unsigned char bit_ctr;
    
    for(bit_ctr = 0; bit_ctr < 8; bit_ctr++) {
        SPI_MOSI = (dat & 0x80);                      // output 'byte', MSB to MOSI
        dat = (unsigned char)(dat << 1);                             // shift next bit into MSB..
        SPI_SCK = 1;                                  // Set SCK high..
        dat |= (unsigned char)SPI_MISO;                              // capture current MISO bit
        SPI_SCK = 0;                                  // ..then set SCK low again
    }
    SPI_MOSI = 0;                                   // MOSI pin low before return

    return(dat);
}