/* 
 * File:   ds18b20.h
 * Author: tianlunvip
 *
 * Created on 2018��6��26��, ����9:11
 */

#ifndef DS18B20_H
#define	DS18B20_H

#ifdef	__cplusplus
extern "C" {
#endif

    
#define     DS_ROM_READ         0x33        // ��ROM���� ��64λ��ַ
#define     DS_ROM_MATH         0x55        // ƥ��ROM
#define     DS_ROM_SEARCH       0xF0        // ����ROM
#define     DS_ROM_SKIP         0xCC        // ����ROM
#define     DS_ROM_ALARM        0xEC        // ��������

#define     DS_RAM_CONVERT      0x44        // �����¶�ת��
#define     DS_RAM_READ         0xBE        // ��RAM��9�ֽ�����
#define     DS_RAM_WRITE        0x4E        // д�ݴ���
#define     DS_RAM_COPY         0x48        // ��ֵ�ݴ������ݵ�EEPROM
#define     DS_RAM_RECALL       0xB8        // EEPROM�����ݻָ����ݴ���
#define     DS_RAM_POWER        0xB4        // �����緽ʽ����������Ϊ0����ӵ�ԴΪ1

    
void DS_Rest();                     // ��ʼ��DS18B20
void DS_Write(char command);        // дʱ��
char DS_Read();                     // ��ʱ��
void DS_Convert();                  // �¶�ת��
char DS_GetTemp();
void DS_RAM_Write(char th, char tl, char acc);

#ifdef	__cplusplus
}
#endif

#endif	/* DS18B20_H */

