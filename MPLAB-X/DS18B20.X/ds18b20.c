#include "ds18b20.h"
#include "header.h"


// ����˿�
#define     DQ_DIR      TRISA2
#define     DQ          RA2


void DS_Rest() {
    char flag = 1;
    while(flag) {    
        DQ_DIR = 0; 
        DQ = 0;
        __delay_us(800);
        DQ_DIR = 1;
        __delay_us(80);
        if(!DQ) {
            flag = 0;
        }
        __delay_us(200);
    }
}

void DS_Write(char command) {   
    for(signed char i = 8; i > 0; i--) {
        DQ_DIR = 0; 
        DQ = 0;
        __delay_us(5);
        DQ_DIR = command & 0x01;
        __delay_us(60);
        DQ_DIR = 1;
        __delay_us(5);
        command >>= 1;
    }
}

char DS_Read() {
    char value, temp;                              //�����¶�
    for(signed char i = 8; i > 0; i--) {
        DQ_DIR = 0;
        DQ = 0;
        __delay_us(5);
        DQ_DIR = 1;                                 //�����ߵ�ƽ
        __delay_us(20);
        temp = DQ;
        value = (value>>1) | (temp<<7);
        __delay_us(60);
    }
    return value;
}

void DS_Convert() {
    // �¶�ת��
    DS_Rest();
    DS_Write(DS_ROM_SKIP);
    DS_Write(DS_RAM_CONVERT);
}

char DS_GetTemp() {
    
    int lowtemp, hightemp;
    char ds_low, ds_hight;
    char ds_th, ds_tl, ds_rs0, ds_rs1, ds_rs2, ds_rs3, ds_crc;
//    float temp;
    
//    DS_RAM_Write(99, 1, 0x7F);
    
    // ת������
//    DS_Convert();
    
    // ���Ĵ���
    DS_Rest();
    DS_Write(DS_ROM_SKIP);
    DS_Write(DS_RAM_READ); 

    // ���ݴ���
    ds_low  = DS_Read();
    ds_hight = DS_Read();
//    ds_th = DS_Read();
//    ds_tl = DS_Read();
//    ds_rs0 = DS_Read();
//    ds_rs1 = DS_Read();
//    ds_rs2 = DS_Read();
//    ds_rs3 = DS_Read();
//    ds_crc = DS_Read();
//    DS_Rest();
    
//    lowtemp = ds_low;
//    hightemp = ds_hight;
//    hightemp = hightemp<<8;
//    temp = (hightemp | lowtemp) * 0.0625;

    ds_th = (ds_hight << 4) | (ds_low >> 4);
    
    return ds_th;
}

void DS_RAM_Write(char th, char tl, char acc) {
 
    DS_Rest();
    DS_Write(DS_ROM_SKIP);
    DS_Write(DS_RAM_WRITE); 
    
    DS_Write(th);       // дTHֵ
    DS_Write(tl);       // дTLֵ
    DS_Write(acc);      // д�¶ȼƾ�ȷ��
    
    DS_Rest();
    DS_Write(DS_ROM_SKIP);      // skip rom
    DS_Write(DS_RAM_COPY);      // �����ݴ���    
    
    __delay_ms(200);
}