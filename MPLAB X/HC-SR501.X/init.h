#ifndef INIT_TEMPLATE_H
#define	INIT_TEMPLATE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pic.h>
//#include <eeprom_routines.h>            // EEPROM 读写函数使用有问题

/*
 * 注意：不能定义 ON  OFF 否则程序出错 中断无法执行（不知为何？）
 */    
//#define     ON      0xFF
//#define     OFF     0x00

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
    
#define     OSCCON_125K             0X28
#define     OSCCON_250K             0X30
#define     OSCCON_500K             0X38
#define     OSCCON_1M               0X58
#define     OSCCON_2M               0X60
#define     OSCCON_4M               0X68
#define     OSCCON_8M               0X70
#define     OSCCON_16M              0X78

#define     _XTAL_FREQ              _XTAL_FREQ_4M       // 当前工程使用频率定义


/*
 * 函数定义
 */

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

void Pic_Init(void);            // 在主程序众调用，初始化所有设置
void FOSC_Init(void);           // 初始化内部时钟源
void PORTA_Init(void);
void PORTC_Init(void);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

