/* 
 * File:   adc.h
 * Author: tianlunvip
 *
 * Created on 2019年9月10日, 下午10:25
 */

#ifndef ADC_H
#define	ADC_H

/*
 */


#ifdef	__cplusplus
extern "C" {
#endif

void ADC_Init(void);
unsigned int ADC_Read(void);
unsigned int ADC_Read_N(void);

#ifdef	__cplusplus
}
#endif

#endif	/* ADC_H */

