/* 
 * File:   header.h
 * Author: tianlunvip
 *
 * Created on 2018年6月27日, 下午6:54
 */

#ifndef HEADER_H
#define	HEADER_H

#include <stdio.h>
#include <stdlib.h>

#include <pic.h>
#include <eeprom_routines.h>

#ifdef	__cplusplus
extern "C" {
#endif

/*
 * 晶振频率  
 * 125KHZ   OSCCON = 0X28;
 * 250KHZ   OSCCON = 0X30;
 * 500KHZ   OSCCON = 0X38;
 * 1MHZ     OSCCON = 0X58;
 * 2MHZ     OSCCON = 0X60;
 * 4MHZ     OSCCON = 0X68;
 * 8MHZ     OSCCON = 0X70;
 * 16MHZ    OSCCON = 0X78;
*/ 
#define     _XTAL_FREQ      16000000      

#ifdef	__cplusplus
}
#endif

#endif	/* HEADER_H */

