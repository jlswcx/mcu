/* 
 * File:   fosc.h
 * Author: tianlunvip
 *
 * Created on 2019年9月20日, 下午19:33
 */

#ifndef FOSC_H
#define	FOSC_H

#include <pic.h>
 /*
  * 晶振频率 使用延时函数需定义 _XTAL_FREQ
  * 通过配置字 FOSC 选择内部振荡器 INTOSC 需定义 OSCCON 确定振荡器频率
*/    

#define     _XTAL_FREQ_125K         125000              // 125KHZ    
#define     _XTAL_FREQ_250K         250000              // 250KHZ
#define     _XTAL_FREQ_500K         500000              // 500KHZ
#define     _XTAL_FREQ_1M           1000000             // 1MHZ
#define     _XTAL_FREQ_2M           2000000             // 2MHZ
#define     _XTAL_FREQ_4M           4000000             // 4MHZ
#define     _XTAL_FREQ_8M           8000000             // 8MHZ
#define     _XTAL_FREQ_16M          16000000            // 16MHZ 
#define     _XTAL_FREQ_32M          32000000            // 32MHZ  

#define     OSCCON_125K             0x28
#define     OSCCON_250K             0x30
#define     OSCCON_500K             0x38
#define     OSCCON_1M               0x58
#define     OSCCON_2M               0x60
#define     OSCCON_4M               0x68
#define     OSCCON_8M               0x70
#define     OSCCON_16M              0x78
#define     OSCCON_32M              0xF0

// 振荡频率定义
#define     _XTAL_FREQ              _XTAL_FREQ_4M
#define     _OSCCON                 OSCCON_4M


#ifdef	__cplusplus
extern "C" {
#endif
    
    
void FOSC_Init(void);

#ifdef	__cplusplus
}
#endif

#endif	/* FOSC_H */