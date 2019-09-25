/* 
 * File:   spi.h
 * Author: tianlunvip
 *
 * Created on 2017年12月18日, 下午12:29
 */

#ifndef SPI_H
#define	SPI_H

#ifdef	__cplusplus
extern "C" {
#endif

void SPI_Port_Init();
void SPI_Init();

unsigned char SPI_RW(unsigned char dat);              // 硬件通信

unsigned char SPI_RW_SW(unsigned char dat);          // IO端口模式SPI通信

#ifdef	__cplusplus
}
#endif

#endif	/* SPI_H */

