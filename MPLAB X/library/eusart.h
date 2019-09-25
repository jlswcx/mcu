/* 
 * File:   eusart.h
 * Author: tianlunvip
 *
 * Created on 2019年9月21日, 下午0:33
 */

#ifndef EUSART_H
#define	EUSART_H



#ifdef	__cplusplus
extern "C" {
#endif

void EUSART_Tx(void);
void EUSART_Rx(void);
void EUSART_Init(void);
void EUSART_Send(unsigned char dat);

#ifdef	__cplusplus
}
#endif

#endif	/* EUSART_H */

