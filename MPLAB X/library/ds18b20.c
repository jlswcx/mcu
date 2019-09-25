#include "ds18b20.h"
#include "fosc.h"

// 定义端口
#define     DQ_DIR      TRISC4
#define     DQ_IO       RC4

/*
 * DQ_DIR = 1  DQ 高电平
 * DQ_DIR = 0  DQ_IO = 0   DQ 低电平
 * DQ_DIR = 0  DQ_IO = 1   DQ 高电平
 */

/*
 * 注意；防止中断程序 影响时序
 * 时钟频率不能低于 1M, 指令周期不应大于 1us 以免影响最小延时
 */

void DS_Rest(void) {
    unsigned char flag = 1;
    
    GIE = 0;
    while(flag) {    
        DQ_DIR = DQ_IO = 0; 
        __delay_us(480);        // 发送复位信号，下拉500us
        DQ_DIR = 1;
        __delay_us(60);         // 等待16??60us
        if(!DQ_IO) {            // 接收60??240us的低脉冲
            flag = 0;
        }
    }
    GIE = 1;
    __delay_us(240);
}

void DS_Write(char command) {  
    unsigned char i;
    
    GIE = 0;
    for(i = 0; i < 8; i++) {
        DQ_DIR = DQ_IO = 0;         // 拉低电平
        __delay_us(5);
        DQ_DIR = command & 0x01;
        __delay_us(55);
        DQ_DIR = 1;
        command >>= 1;
    }
    GIE = 1;
    __delay_us(65);
}

unsigned char DS_Read(void) {
    unsigned char i, value, temp;                   //读出温度
    
    GIE = 0;
    for(i = 0; i < 8; i++) {
        DQ_DIR = DQ_IO = 0;                   
        __delay_us(10);
        DQ_DIR = 1;                                 // 释放总线
        __delay_us(10);
        temp = DQ_IO;
        __delay_us(50);
        value = (value>>1) | (temp<<7);  
    }
    GIE = 1;
    return value;
}

void DS_Convert(void) {
    // 温度转换
    DS_Rest();
    DS_Write(DS_ROM_SKIP);
    DS_Write(DS_RAM_CONVERT);
    __delay_ms(750);
}

unsigned int DS_GetTemp(void) {
    
    unsigned int ret = 0;
    unsigned char temp, ds_low, ds_hight, ds_th, ds_tl, ds_rs0, ds_rs1, ds_rs2, ds_rs3, ds_crc;
    temp = ds_low = ds_hight = ds_th = ds_tl = 0;
    ds_rs0 = ds_rs1 = ds_rs2 = ds_rs3 = ds_crc = 0;
     
    // 读寄存器
    DS_Rest();
    DS_Write(DS_ROM_SKIP);
    DS_Write(DS_RAM_READ); 

    // 读暂存器
    ds_low  = DS_Read();
    ds_hight = DS_Read();
    ds_th = DS_Read();        // 
    ds_tl = DS_Read();
    ds_rs0 = DS_Read();       // 结构寄存器易失性拷贝
    ds_rs1 = DS_Read();
    ds_rs2 = DS_Read();
    ds_rs3 = DS_Read();
    ds_crc = DS_Read();

    DS_Rest();
    
    /*
     * 分辨率 9-12 位设置表 R1 R0 的值 位于 ds_rs0 字节的第 7、6 位
     * 0 0 - 9位 0.5
     * 0 1 - 10位 0.25
     * 1 0 - 11位 0.125
     * 1 1 - 12位 0.0625
     * 
     */ 
    
    temp = ds_hight >> 3;       // 取前5位
    if(temp & 0x1F) {           // 判断前5位是 11111 还是 00000   1为负 0为正数
        ret |= ds_hight;
        ret = (ret << 8) | ds_low;
        ret = ~ret;                 // 取反 再 +1
        ret += 1;
        ret = (unsigned long)ret * 625 / 100;
    }else {
        ret |= ds_hight;
        ret = (ret << 8) | ds_low;
        ret = (unsigned long)ret * 625 / 100;         // 这里一定要转换 long 类型，不然数据溢出
    }
    
    return ret;
}

void DS_RAM_Write(char th, char tl, char acc) {
 
    DS_Rest();
    DS_Write(DS_ROM_SKIP);
    DS_Write(DS_RAM_WRITE); 
    
    DS_Write(th);       // 写TH值
    DS_Write(tl);       // 写TL值
    DS_Write(acc);      // 写温度计精确度
    
    DS_Rest();
    DS_Write(DS_ROM_SKIP);      // skip rom
    DS_Write(DS_RAM_COPY);      // 复制暂存器    
    
    __delay_ms(200);
}