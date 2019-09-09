/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>

#include "stdlib.h"

#include <avr/io.h>
//#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>


#include "io_168pa.h"
#include "isd1700.h"
#include "nrf24l01.h"
#include "spi.h"

#include "led.h"
#include "temp.h"

//#pragma interrupt_handler adc_isr:22
//#pragma interrupt_handler TIMER0_Interrupt:17
//#pragma interrupt_handler PCINT0_Interrupt:4			// 电平变化中断引脚0-7使用该向量
//#pragma interrupt_handler PCINT1_Interrupt:5			// 电平变化中断引脚8-14使用该向量
//#pragma interrupt_handler PCINT2_Interrupt:6			// 电平变化中断引脚16-23使用该向量

void LEDx(char num);
void WirelessPad(void);
void Isd1700(void);


#define		RESET_KEY_DIR	DDRC3
#define		STATUS_KEY_DIR	DDRC2
#define		HOLD_KEY_DIR	DDRC1
#define		REC_KEY_DIR		DDRC0

#define		RESET_KEY		PORTC3
#define		STATUS_KEY		PORTC2
#define		HOLD_KEY		PORTC1
#define		REC_KEY			PORTC0

int main(void) {
	/* Insert system clock initialization code here (sysclk_init()). */

	board_init();
	
	/* Insert application code here, after the board has been initialized. */
	RESET_KEY = 1;
	STATUS_KEY = 1;
	HOLD_KEY = 1;
	REC_KEY = 1;
		
	RESET_KEY_DIR = 0;
	STATUS_KEY_DIR = 0;
	HOLD_KEY_DIR = 0;
	REC_KEY_DIR = 0;
	

	
	SREG = 0X80;
	PCICR = 1<<PCIE1;
	PCMSK1 = 1<<PCINT8 | 1<<PCINT9 | 1<<PCINT10 |1<<PCINT11;
	
	/********* 逻辑代码区 Start **********/
	
	LED_Init();
	
	ISD_Port_Init();
	ISD_SPI_Init();
	ISD_Pu();
	ISD_Rest();
	ISD_Play();


	LED_All(1);

	LED_All(0);
			
	WirelessPad();
	
	/********* 逻辑代码区 End ************/
	while(1) {
		LED_Aux(1);
	}
}


ISR(PCINT1_vect) {
	
//	_NOP();
	
	if(RESET_KEY) {
		ISD_Init();
		ISD_Rd_RecPtr();
		ISD_set_rec(Status[VOCADDL], Status[VOCADDH], 0xFF,0xFF);
	

		LED_Chair(1);
		
	}else if(REC_KEY) {
		ISD_Init();
		ISD_rec();
		LED_Green(1);
	}else  {
		ISD_Init();
		ISD_Stop();
		LED_Green(0);
		LED_Chair(0);
	}
	
	
//	
}

void Timer0_Init(void) {
//	TCCR0 = 0x02;		// 控制寄存器
	
//	TCNT0 = 0x00;		// 计数寄存器
	
//	TIMSK = 0x01;		// 中断溢出允许
}

/************************************************************************/
/* ADC 中断函数                                                                     */
/************************************************************************/
ISR(ADC_vect) {
	 
}

//SIGNAL(SIG_USART_RECV) {
	//
//}

ISR(TIMER0_OVF_vect) {
	
}




void WirelessPad(void) {
	
	unsigned char ledx;
	unsigned char rxbuf[2] = {0, 0};
	    
	NRF_Port_Init();
	NRF_SPI_Init();
		
	if(NRF_Check(1)) {
		while(1) {
			LED_Red(1);
			_delay_ms(100);
			LED_Red(0);
			_delay_ms(100);
		}
	}
		
	NRF_RX_Mode(1);
	LED_Control(0);
	
	unsigned char down_one = 0;
	while(1) {
		
		NRF_SPI_Init();
		    
		ledx = NRF_RxPacket(1, rxbuf);
		    
//		LEDx(ledx);
		
		if(rxbuf[0] == 0x02) {
			LED_Control(0);
			
			if(down_one == 0) {
				down_one = 1;
				Isd1700();
				
				NRF_SPI_Init();			
			}
			

		} else if(rxbuf[0] == 0x03) {
			down_one = 0;
			LED_Control(1);
			Isd1700();
			
			NRF_SPI_Init();
		}else {
			LED_Control(0);
		}

	}	
}


void Isd1700(void) {
	
	unsigned char i = 0;
	
	ISD_Port_Init();
	ISD_SPI_Init();
	ISD_Pu();	
	ISD_Play();
	ISD_Fwd();

	//i = ISD_Devid();
	//if(i) {
		////		LEDx(i);
		//
		//ISD_Play();
		//
		//_delay_ms(10000);
		//ISD_Fwd();
		//_delay_ms(50);
		////		ISD_Play();
	//}	
}



void LEDx(char num) {
	while(num--) {
		LED_Green(1);
		_delay_ms(200);
		LED_Green(0);
		_delay_ms(200);
	}
	_delay_ms(1000);
}