/*
 * led.h
 *
 * Created: 2018/1/28 11:27:00
 *  Author: TianlunVip
 */ 

#ifndef WP_LED_H_
#define WP_LED_H_

#ifdef	__cplusplus
extern "C" {
#endif

void LED_Init(void);
void LED_Aux(char i);
void LED_Chair(char i);
void LED_Nurse(char i);
void LED_Potty(char i);
void LED_Bed(char i);
void LED_Red(char i);
void LED_Green(char i);
void LED_All(char i);
void LED_Control(char i);
#ifdef	__cplusplus
}
#endif

#endif /* WP_LED_H_ */