#include "library.h"
#include "macro.h"


void write_ee(char addr, char data) {
    do{
    }while(WR == 1);    // �ȴ�д���
    EEADR = addr;       // д���ַ��Ϣ
    EEDATA = data;      // д��������Ϣ
    EEPGD = 0;          // ����EEPROM
    WREN = 1;           // дEEPROM����
    EECON2 = 0x55;      // д���ض�ʱ��
    EECON2 = 0xaa;
    WR = 1;             // ִ��д����
    do{
    }while(WR == 1);//�ȴ�д���
    WREN = 0;           // ��ֹд��EEPROM 
}

char read_ee(char addr) {
    char num;
    do{
    }while(RD == 1);    // �ȴ������
    EEADR = addr;       // д��Ҫ����ַַ
    EEPGD = 0;          // ����EEPROM
    RD = 1;             // ִ�ж�����
    do{
    }while(RD == 1);    // �ȴ������
    num = EEDATA;
    return num;         // ���ض�ȡ������
}
