/* 
 * File:   spi.h
 * Author: tianlunvip
 *
 * Created on 2017��12��14��, ����12:29
 */
#ifndef SPI_H
#define	SPI_H

#ifdef	__cplusplus
extern "C" {
#endif
    
void SPI_Port_Init(void);
void SPI_Init(void);
unsigned char SPI_RW(unsigned char data);

#ifdef	__cplusplus
}
#endif

#endif	/* SPI_H */

