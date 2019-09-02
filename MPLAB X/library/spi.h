/* 
 * File:   spi.h
 * Author: tianlunvip
 *
 * Created on 2017��12��18��, ����12:29
 */

#ifndef SPI_H
#define	SPI_H

#ifdef	__cplusplus
extern "C" {
#endif

void SPI_Port_Init();
void SPI_Init();

unsigned char SPI_RW(unsigned char dat);              // Ӳ��ͨ��

unsigned char SPI_RW_SW(unsigned char dat);          // IO�˿�ģʽSPIͨ��

#ifdef	__cplusplus
}
#endif

#endif	/* SPI_H */

