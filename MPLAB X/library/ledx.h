/* 
 * File:   ledx.h
 * Author: tianlunvip
 *
 * Created on 2019年9月20日, 下午19:33
 */

#ifndef LEDX_H
#define	LEDX_H


/* 控制LED灯 控制 IO */
#define     LED_DIR         TRISC3          // RC3 引脚控制LED
#define     LED_IO          LATC3
#define     LED_ON          0               // 低电平 亮
#define     LED_OFF         1               // 高电平 灭   


#ifdef	__cplusplus
extern "C" {
#endif

    
void LED_Init(void);
void LEDx(unsigned char num);
void LEDs(unsigned char sta);


#ifdef	__cplusplus
}
#endif

#endif	/* LEDX_H */

