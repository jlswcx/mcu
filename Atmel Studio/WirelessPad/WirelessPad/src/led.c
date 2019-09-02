#include "led.h"

#include <avr/io.h>
#include "io_168pa.h"
#include <util/delay.h>

// PIN7->PB6
// PIN8->PB7
// PIN9->PD5
// PIN10->PD6

#define LED_CONTROL_DIR		DDRD3
#define LED_CONTROL			PORTD3

#define LPIN7_DIR	DDRB6
#define LPIN8_DIR	DDRB7
#define LPIN9_DIR	DDRD5
#define LPIN10_DIR	DDRD6

#define LPIN7	PORTB6
#define LPIN8	PORTB7
#define LPIN9	PORTD5
#define LPIN10	PORTD6



#define	LED_TWO_ON	LED_CONTROL=1
#define	LED_TWO_OFF	LED_CONTROL=0

void LED_Init(void) {
	LED_CONTROL_DIR = 0;
	
	LPIN7_DIR	= 0;
	LPIN8_DIR	= 0;
	LPIN9_DIR	= 0;
	LPIN10_DIR	= 0;	
	
	LPIN7	= 0;
	LPIN8	= 0;
	LPIN9	= 0;
	LPIN10	= 0;
}


void LED_white() {
	
}


void LED_Aux(char i) {
	if(i) {
				LPIN7_DIR = 1;
				LPIN8_DIR = 1;	
				LPIN9_DIR = 0;
				LPIN7 = 0;
				LPIN8 = 1;
		while(1) {
	
			LPIN9 = 0;

			_delay_us(1);
			LPIN9 = 1;
			_delay_ms(500);
		}

		

		
		LED_CONTROL = 1;
		
	}else {
		LPIN7_DIR = 0;
		LPIN8_DIR = 0;
		LPIN9_DIR = 0;
	}

}

void LED_Chair(char i) {
	if(i) {
		LPIN7_DIR = 1;
		LPIN8_DIR = 1;
		LPIN7 = 1;
		LPIN8 = 0;
	}else {
		LPIN7_DIR = 0;
		LPIN8_DIR = 0;
	}
}

void LED_Nurse(char i) {
	if(i) {
		LPIN7_DIR = 1;
		LPIN10_DIR = 1;
		LPIN7 = 0;
		LPIN10 = 1;
	}else {
		PORTB6 = 1;
		PORTD6 = 0;
	}

}

void LED_Potty(char i) {
	if(i) {
		LPIN7_DIR = 1;
		LPIN10_DIR = 1;
		LPIN7 = 1;
		LPIN10 = 0;
	}else {
		LPIN7_DIR = 0;
		LPIN10_DIR = 0;
	}

}

void LED_Bed(char i) {
	if(i) {
		LPIN8_DIR = 1;
		LPIN9_DIR = 1;
		LPIN8 = 1;
		LPIN9 = 0;
	}else {
		PORTB7 = 0;
		PORTD5 = 1;
	}
}

void LED_Red(char i) {
	if(i == 1) {
		PORTB6 = 1;
		PORTD5 = 0;
	}else {
		PORTB6 = 0;
		PORTD5 = 1;
	}
}

void LED_Green(char i) {
	if(i == 1) {
		LPIN7_DIR = 1;
		LPIN9_DIR = 1;
		LPIN7 = 0;
		LPIN9 = 1;
		}else {
		LPIN7_DIR = 0;
		LPIN9_DIR = 0;
		LPIN7 = 0;
		LPIN9 = 0;
	}
}

void LED_Control(char i) {
	if(i)
		LED_CONTROL = 1;
	else
		LED_CONTROL = 0;
}

void LED_All(char i) {
	if(i) {
		
		LPIN7_DIR = 1;
		LPIN8_DIR = 1;
		LPIN9_DIR = 1;
		LPIN10_DIR = 1;
		
		for(int i = 0; i < 250; i++) {
			
			LPIN7 = 1;	// potty chair red
			LPIN8 = 0;
			LPIN9 = 0;
			LPIN10 = 0;
			_delay_ms(5);		
			LPIN7 = 0;	// nurse aux green
			LPIN8 = 1;
			LPIN9 = 1;
			LPIN10 = 1;
			_delay_ms(5);	
			LPIN7 = 0;	// bed
			LPIN8 = 1;
			LPIN9 = 0;	
			_delay_ms(5);		
			
		}

		
	}else {
		LED_Init();
	}
}