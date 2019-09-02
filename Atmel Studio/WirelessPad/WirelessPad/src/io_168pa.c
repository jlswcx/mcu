/*
 * avr_io.c
 *
 * Created: 2018/1/13 0:41:04
 *  Author: HELLO
 */ 

#include "io_168pa.h"

volatile PINbits_t	*PINBbit = (PINbits_t*)(PINB_ADDRESS + __SFR_OFFSET);
volatile PINbits_t	*PINCbit = (PINbits_t*)(PINC_ADDRESS + __SFR_OFFSET);
volatile PINbits_t	*PINDbit = (PINbits_t*)(PIND_ADDRESS + __SFR_OFFSET);

volatile PORTbits_t *PORTBbit = (PORTbits_t*)(PORTB_ADDRESS + __SFR_OFFSET);
volatile PORTbits_t *PORTCbit = (PORTbits_t*)(PORTC_ADDRESS + __SFR_OFFSET);
volatile PORTbits_t *PORTDbit = (PORTbits_t*)(PORTD_ADDRESS + __SFR_OFFSET);

volatile DDRbits_t *DDRBbit = (DDRbits_t*)(DDRB_ADDRESS + __SFR_OFFSET);
volatile DDRbits_t *DDRCbit = (DDRbits_t*)(DDRC_ADDRESS + __SFR_OFFSET);
volatile DDRbits_t *DDRDbit = (DDRbits_t*)(DDRD_ADDRESS + __SFR_OFFSET);

volatile SPCRbits_t *SPCRbit = (SPCRbits_t*)(SPCR_ADDRESS + __SFR_OFFSET);
volatile SPSRbits_t *SPSRbit = (SPSRbits_t*)(SPSR_ADDRESS + __SFR_OFFSET);