#include "library.h"
#include "macro.h"


void write_ee(char addr, char data) {
    do{
    }while(WR == 1);    // 等待写完成
    EEADR = addr;       // 写入地址信息
    EEDATA = data;      // 写入数据信息
    EEPGD = 0;          // 操作EEPROM
    WREN = 1;           // 写EEPROM允许
    EECON2 = 0x55;      // 写入特定时序
    EECON2 = 0xaa;
    WR = 1;             // 执行写操作
    do{
    }while(WR == 1);//等待写完成
    WREN = 0;           // 禁止写入EEPROM 
}

char read_ee(char addr) {
    char num;
    do{
    }while(RD == 1);    // 等待读完成
    EEADR = addr;       // 写入要读的址址
    EEPGD = 0;          // 操作EEPROM
    RD = 1;             // 执行读操作
    do{
    }while(RD == 1);    // 等待读完成
    num = EEDATA;
    return num;         // 返回读取的数据
}
