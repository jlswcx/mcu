/*
 * File:   init.c
 * Author: WANGE
 *
 * 初始化各种选项配置
 * 
 * Created on 2019年8月19日, 下午8:57
 */

#include "init.h"

void Pic_Init() {
    FOSC_Init();
    PORTA_Init();
    PORTC_Init();
}

void FOSC_Init(){ OSCCON = OSCCON_4M; }

void PORTA_Init() {
    TRISA = 0;
    PORTA = 0;
    LATA = 0;
    ANSELA = 0;
}

void PORTC_Init() {
    TRISC = 0;
    PORTC = 0;
    LATC = 0;
    ANSELC = 0;
}