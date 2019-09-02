/*
 * temp.c
 *
 * Created: 2018/1/28 11:29:32
 *  Author: HELLO
 */ 

#include <asf.h>

#include "temp.h"
#include "stdlib.h"

#include <avr/io.h>
#include "io_168pa.h"
#include <util/delay.h>

void led24(void) {
	
	unsigned char i = 0;
	
	DDRB = 0xFF;
	DDRC = 0x3F;
	DDRD = 0xFF;
	
	PORTB = 0xFF;
	PORTC = 0x3F;
	PORTD = 0xFF;
	
	DDRC6 = 0;		// °´¼ü
	
	char k;


	while(1) {
		//if(PINC6 == 1) {
		//_delay_ms(20);
		//if(PINC6 == 1) {
		//i++;
		//if(i > 3) i = 0;
		//}
		//}
		
		if(i == 1) {
			PORTB = 0xFF;
			PORTC = 0x3F;
			PORTD = 0xFF;
		}
		
		if(i == 1) {
			PORTB = 0x00;
			PORTC = 0x00;
			PORTD = 0x00;
			_delay_ms(100);
			PORTB = 0xFF;
			PORTC = 0x3F;
			PORTD = 0xFF;
			_delay_ms(100);
		}
		if(i == 0){
			
			while(PORTB) {
				PORTB = PORTB << 1;
				_delay_ms(30);
			}
			while(PORTD) {
				PORTD = PORTD << 1;
				_delay_ms(30);
			}
			while(PORTC) {
				PORTC = PORTC << 1 & 0x3F;
				_delay_ms(30);
			}
			k = 5;
			while(k >= 0) {
				PORTC |= 1 << k--;
				_delay_ms(30);
			}
			k = 7;
			while(k >= 0) {
				PORTD |= 1 << k--;
				_delay_ms(30);
			}
			k = 7;
			while(k >= 0) {
				PORTB |= 1 << k--;
				_delay_ms(30);
			}


			while(PORTC) {
				PORTC = PORTC >> 1;
				_delay_ms(30);
			}
			while(PORTD) {
				PORTD = PORTD >> 1;
				_delay_ms(30);
			}
			while(PORTB) {
				PORTB = PORTB >> 1;
				_delay_ms(30);
			}
			k = 0;
			while(k < 8) {
				PORTB |= 1 << k++;
				_delay_ms(30);
			}
			k = 0;
			while(k < 8) {
				PORTD |= 1 << k++;
				_delay_ms(30);
			}
			k = 0;
			while(k < 6) {
				PORTC |= 1 << k++;
				_delay_ms(30);
			}
		}
		
		if (i == 3) {
			PORTB = 0x00;
			PORTC = 0x00;
			PORTD = 0x00;
		}
	}
	

	while(1) {
		if(PINC5 == 1){
			i++;
			if(i > 2) {
				i = 0;
			}
		}
		if(i == 0) {
			PORTB = 0xFF;
			PORTD = 0xFF;
		}
		if(i == 1) {

			PORTB = 0x00;
			PORTD = 0x00;
			_delay_ms(100);
			PORTB = 0xFF;
			PORTD = 0xFF;
			_delay_ms(100);
			
		}
		if(i == 2) {
			while(PORTB) {
				PORTB = PORTB << 1;
				_delay_ms(50);
			}
			while(PORTD) {
				PORTD = PORTD << 1;
				_delay_ms(50);
			}
			k = 7;
			while(PORTD != 0xFF) {
				PORTD |= 1 << k--;
				_delay_ms(50);
			}
			k = 7;
			while(PORTB != 0xFF) {
				PORTB |= 1 << k--;
				_delay_ms(50);
			}
		}


	}
	
	while(1) {
		nop();
	}
	
	
	
	
	
}
