/* 
 * File:   ds18b20.h
 * Author: tianlunvip
 *
 * Created on 2018年6月26日, 下午9:11
 */

#ifndef DS18B20_H
#define	DS18B20_H

#ifdef	__cplusplus
extern "C" {
#endif

    
#define     DS_ROM_READ         0x33        // 读ROM编码 即64位地址
#define     DS_ROM_MATH         0x55        // 匹配ROM
#define     DS_ROM_SEARCH       0xF0        // 搜索ROM
#define     DS_ROM_SKIP         0xCC        // 跳过ROM
#define     DS_ROM_ALARM        0xEC        // 警报搜索

#define     DS_RAM_CONVERT      0x44        // 启动温度转换
#define     DS_RAM_READ         0xBE        // 读RAM中9字节内容
#define     DS_RAM_WRITE        0x4E        // 写暂存器
#define     DS_RAM_COPY         0x48        // 赋值暂存器内容到EEPROM
#define     DS_RAM_RECALL       0xB8        // EEPROM中内容恢复到暂存器
#define     DS_RAM_POWER        0xB4        // 读供电方式，寄生供电为0，外接电源为1

    
void DS_Rest(void);                     // 初始化DS18B20
void DS_Write(char command);        // 写时序
unsigned char DS_Read(void);                     // 读时序
void DS_Convert(void);                  // 温度转换
unsigned int DS_GetTemp(void);
void DS_RAM_Write(char th, char tl, char acc);

#ifdef	__cplusplus
}
#endif

#endif	/* DS18B20_H */

