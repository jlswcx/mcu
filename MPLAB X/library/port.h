/* 
 * File:   port.h
 * Author: tianlunvip
 *
 * Created on 2017年12月13日, 下午12:40
 */

#ifndef PORT_H
#define	PORT_H


#define     TRA0    TRISA0
#define     TRA1    TRISA1
#define     TRA2    TRISA2
#define     TRA3    TRISA3
#define     TRA4    TRISA4
#define     TRA5    TRISA5

#define     TRC0    TRISC0
#define     TRC1    TRISC1
#define     TRC2    TRISC2
#define     TRC3    TRISC3
#define     TRC4    TRISC4
#define     TRC5    TRISC5


#ifdef	__cplusplus
extern "C" {
#endif

void PORT_Init(void);
void PORTA_Init(void);
void PORTC_Init(void);

#ifdef	__cplusplus
}
#endif

#endif	/* PORT_H */

