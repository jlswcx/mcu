/* 
 * File:   ledtube.h
 * Author: tianlunvip
 *
 * Created on 2019年9月18日, 下午19:28
 */

#ifndef LED_TUBE_H
#define	LED_TUBE_H



#ifdef	__cplusplus
extern "C" {
#endif

    
inline void display_place(unsigned char num);
void display_2seg(unsigned char num, unsigned char dp, unsigned char seg);
void display_4seg(unsigned char num, unsigned char dp, unsigned char seg);

void display_2seg_hc(unsigned char num, unsigned char dp, unsigned char seg);


#ifdef	__cplusplus
}
#endif

#endif	/* LED_TUBE_H */

