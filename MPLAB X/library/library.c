#include "library.h"
#include <pic.h>


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

// 位转换
char bit_swnp(char s) {
	char t, l, f;
	t = 0B00000001;
	f = 0B00000000;
	for(signed char i = 7; i >= 0; i--){
		l = s & t;				// 取最右位
		l = l << i;				// 左移7位
		s = s >> 1;
		f = f | l;
	};
	return f;
}
