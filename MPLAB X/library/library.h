/* 
 * File:   library.h
 * Author: tianlunvip
 *
 * Created on 2018年8月9日, 下午10:26
 */

#ifndef LIBRARY_H
#define	LIBRARY_H

#ifdef	__cplusplus
extern "C" {
#endif

void write_ee(char addr, char data);
char read_ee(char addr);


#ifdef	__cplusplus
}
#endif

#endif	/* LIBRARY_H */

