/* 
 * File:   macro.h
 * Author: tianlunvip
 *
 * Created on 2018年8月9日, 下午10:25
 */

#ifndef MACRO_H
#define	MACRO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pic.h>
#include <eeprom_routines.h>                // EEPROM 读写函数使用有问题
                                            // PIC.H 文件已经包含此文件

/*
 * 注意：不能定义 ON  OFF 否则程序出错 中断无法执行（不知为何？或许是保留字吧）
 */    
//#define     ON      0xFF
//#define     OFF     0x00

// BYTE type definition
#ifndef _BYTE_DEF_
#define _BYTE_DEF_
typedef unsigned char byte;
#endif   /* _BYTE_DEF_ */

#ifndef _UINT_DEF_
#define _UINT_DEF_
typedef unsigned int uint;
#endif   /* _UINT_DEF_ */

#ifndef _ULONG_DEF_
#define _ULONG_DEF_
typedef unsigned int ulong;
#endif   /* _ULONG_DEF_ */


#endif	/* MACRO_H */

