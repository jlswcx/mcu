#include "adc.h"
#include <pic.h>





void ADC_Init(void) {
    FVRCON = 0B01010001;        // FVREN,ADCFVR=01(1.024V)
    ADCON1 = 0B10000000;        // 右对齐，AD时钟为Fosc/32,参考电压为电源电压，
    ADCON0 = 0B01111100;        // 选择通道AN0 
}

unsigned int ADC_Read(void) {       //转换一次
    unsigned int value = 0;
 //   ADCON0bits.CHS =0;            // 选择通道AN0
    GO = 1;                         // 开始转换
    while(GO == 1);                 // 等待转换结束,转换完成后会被清零
    value = (unsigned int)ADRESH;   // 强制类型转换，因为ADRESH是字符型的只能表示8位二进制。所以必须转换成可以容纳10位二进制的整型。
    value = value<<8;               // 将高两位左移8位
    value += ADRESL;                // 低八位加入ADRESL的值。
    return ADRES;
}

unsigned int ADC_Read_N(void){

    unsigned char num = 8;          // 读取次数
    unsigned short total = 0;
    unsigned short average = 0;
    unsigned short adv[8];
    unsigned short i = 0;
    
    for(i = 0;i < num; i++) {       //进行多次AD转换
        adv[i] = ADC_Read();
    }

    for(i = 0;i < num; i++) {       //计算多次AD转换的平均值
      total += adv[i];
    }
    
    average = total / num;
    return average;                 //得到结果返回
}
