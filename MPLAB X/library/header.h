/* 
 * File:   header.h
 * Author: �����ǳ�
 *
 * Created on 2018��6��27��, ����6:54
 */

#ifndef HEADER_H
#define	HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pic.h>
#include <eeprom_routines.h>

// BYTE type definition
#ifndef _BYTE_DEF_
#define _BYTE_DEF_
typedef unsigned char BYTE;
#endif   /* _BYTE_DEF_ */

#ifdef	__cplusplus
extern "C" {
#endif

 /*
  * ����Ƶ�� ʹ����ʱ�����趨�� _XTAL_FREQ
  * ͨ�������� FOSC ѡ���ڲ����� INTOSC �趨�� OSCCON ȷ������Ƶ��
*/    
#define     _XTAL_FREQ_125K         125000              // 125KHZ    
#define     _XTAL_FREQ_250K         250000              // 250KHZ
#define     _XTAL_FREQ_500K         500000              // 500KHZ
#define     _XTAL_FREQ_1M           1000000             // 1MHZ
#define     _XTAL_FREQ_2M           2000000             // 2MHZ
#define     _XTAL_FREQ_4M           4000000             // 4MHZ
#define     _XTAL_FREQ_8M           8000000             // 8MHZ
#define     _XTAL_FREQ_16M          16000000            // 16MHZ 
    
#define     OSCCON_125K             0X28
#define     OSCCON_250K             0X30
#define     OSCCON_500K             0X38
#define     OSCCON_1M               0X58
#define     OSCCON_2M               0X60
#define     OSCCON_4M               0X68
#define     OSCCON_8M               0X70
#define     OSCCON_16M              0X78

#define     _XTAL_FREQ      _XTAL_FREQ_125K
    
#ifdef	__cplusplus
}
#endif

#endif	/* HEADER_H */

