/* 
 * File:   delay.h
 * Author: 晓梦星辰
 *
 * Created on 2018年07月15日, 下午15:40
 */

#ifndef DELAY_H
#define	DELAY_H


#ifdef	__cplusplus
extern "C" {
#endif

    
void Delay1us();		//@24.000MHz

void Delay1us();		//@24.000MHz
void Delay20us();		//@24.000MHz
    void Delay50us();		//@24.000MHz
void Delay200us();      //@24.000MHz
    void Delay300us();		//@24.000MHz
void Delay500us();		//@24.000MHz
void Delay1000us();		//@24.000MHz
    void Delay2000us();		//@24.000MHz
void Delay19800us();		//@24.000MHz     


    
void Delay5ms();		//@24.000MHz
    void Delay8ms();		//@24.000MHz
    void Delay9ms();		//@24.000MHz
    void Delay10ms();		//@24.000MHz
    void Delay20ms();		//@24.000MHz
    void Delay30ms();		//@24.000MHz
    void Delay50ms();		//@24.000MHz
    void Delay100ms();		//@24.000MHz
void Delay200ms();		//@24.000MHz


void Delay1000ms();		//@24.000MHz

    
    

   
#ifdef	__cplusplus
}
#endif

#endif	/* DELAY_H */