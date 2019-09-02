/*
 * io_168pa.h
 *
 * Created: 2018/1/13 0:40:53
 *  Author: tianlunvip
 */ 


#ifndef AVR_IO_168PA_H_
#define AVR_IO_168PA_H_

#include <avr/sfr_defs.h>

#ifdef	__cplusplus
extern "C" {
#endif
	

/************************************************************************/
/* IO端口 相关寄存器地址													*/
/************************************************************************/

/*
 *	PINxn	数据寄存器
 *	DDxn	数据方向寄存器 1： 输出 0： 输入
 *	PINxn	输入引脚地址
*/
#define PINB_ADDRESS    0x03
#define DDRB_ADDRESS    0x04
#define PORTB_ADDRESS   0x05

#define PINC_ADDRESS    0x06
#define DDRC_ADDRESS    0x07
#define PORTC_ADDRESS   0x08

#define PIND_ADDRESS    0x09
#define DDRD_ADDRESS    0x0A
#define PORTD_ADDRESS   0x0B

//#define BIT1(P, N)		P |= (1 << N)
//#define BIT0(P, N)		P &= ~(1 << N)

// 输出口操作：
// PORTA |= (1 << PORTAx)           // 位置高
// PORTA &= ~(1 << PORTAx)       // 位置低
// PORTA ^= (1 << PORTAx)          // 位取反
// 输入口操作：
// PINA & (1 << PORTAx)               // 位读取

typedef union {
	struct {
		unsigned N0                 :1;
		unsigned N1                 :1;
		unsigned N2                 :1;
		unsigned N3                 :1;
		unsigned N4                 :1;
		unsigned N5                 :1;
		unsigned N6                 :1;
		unsigned N7                 :1;
	};
} PORTbits_t, DDRbits_t, PINbits_t;

extern volatile PINbits_t	*PINBbit;
extern volatile PINbits_t	*PINCbit;
extern volatile PINbits_t	*PINDbit;

extern volatile PORTbits_t	*PORTBbit;
extern volatile PORTbits_t	*PORTCbit;
extern volatile PORTbits_t	*PORTDbit;

extern volatile DDRbits_t	*DDRBbit;
extern volatile DDRbits_t	*DDRCbit;
extern volatile DDRbits_t	*DDRDbit;

#ifdef PORTB0
	#undef PORTB7  
	#undef PORTB6  
	#undef PORTB5  
	#undef PORTB4  
	#undef PORTB3  
	#undef PORTB2  
	#undef PORTB1  
	#undef PORTB0  
#endif

#ifdef PORTC0
	#undef PORTC7  
	#undef PORTC6  
	#undef PORTC5  
	#undef PORTC4  
	#undef PORTC3  
	#undef PORTC2  
	#undef PORTC1  
	#undef PORTC0  
#endif

#ifdef PORTD0
	#undef PORTD7  
	#undef PORTD6  
	#undef PORTD5  
	#undef PORTD4  
	#undef PORTD3  
	#undef PORTD2  
	#undef PORTD1  
	#undef PORTD0  
#endif

#ifdef DDRB0
	#undef DDRB0
	#undef DDRB1  
	#undef DDRB2  
	#undef DDRB3  
	#undef DDRB4  
	#undef DDRB5  
	#undef DDRB6  
	#undef DDRB7  
#endif

#ifdef DDRC0
	#undef DDRC0
	#undef DDRC1
	#undef DDRC2
	#undef DDRC3
	#undef DDRC4
	#undef DDRC5
	#undef DDRC6
	#undef DDRC7
#endif

#ifdef DDRD0
	#undef DDRD0
	#undef DDRD1
	#undef DDRD2
	#undef DDRD3
	#undef DDRD4
	#undef DDRD5
	#undef DDRD6
	#undef DDRD7
#endif

#ifdef PINB0
	#undef PINB0
	#undef PINB1
	#undef PINB2
	#undef PINB3
	#undef PINB4
	#undef PINB5
	#undef PINB6
	#undef PINB7
#endif

#ifdef PINC0
	#undef PINC0
	#undef PINC1
	#undef PINC2
	#undef PINC3
	#undef PINC4
	#undef PINC5
	#undef PINC6
	#undef PINC7
#endif

#ifdef PIND0
	#undef PIND0
	#undef PIND1
	#undef PIND2
	#undef PIND3
	#undef PIND4
	#undef PIND5
	#undef PIND6
	#undef PIND7
#endif

#define		PORTB0		PORTBbit->N0
#define		PORTB1		PORTBbit->N1
#define		PORTB2		PORTBbit->N2
#define		PORTB3		PORTBbit->N3
#define		PORTB4		PORTBbit->N4
#define		PORTB5		PORTBbit->N5
#define		PORTB6		PORTBbit->N6
#define		PORTB7		PORTBbit->N7

#define		PORTC0		PORTCbit->N0
#define		PORTC1		PORTCbit->N1
#define		PORTC2		PORTCbit->N2
#define		PORTC3		PORTCbit->N3
#define		PORTC4		PORTCbit->N4
#define		PORTC5		PORTCbit->N5
#define		PORTC6		PORTCbit->N6
#define		PORTC7		PORTCbit->N7

#define		PORTD0		PORTDbit->N0
#define		PORTD1		PORTDbit->N1
#define		PORTD2		PORTDbit->N2
#define		PORTD3		PORTDbit->N3
#define		PORTD4		PORTDbit->N4
#define		PORTD5		PORTDbit->N5
#define		PORTD6		PORTDbit->N6
#define		PORTD7		PORTDbit->N7

#define		DDRB0		DDRBbit->N0
#define		DDRB1		DDRBbit->N1
#define		DDRB2		DDRBbit->N2
#define		DDRB3		DDRBbit->N3
#define		DDRB4		DDRBbit->N4
#define		DDRB5		DDRBbit->N5
#define		DDRB6		DDRBbit->N6
#define		DDRB7		DDRBbit->N7

#define		DDRC0		DDRCbit->N0
#define		DDRC1		DDRCbit->N1
#define		DDRC2		DDRCbit->N2
#define		DDRC3		DDRCbit->N3
#define		DDRC4		DDRCbit->N4
#define		DDRC5		DDRCbit->N5
#define		DDRC6		DDRCbit->N6
#define		DDRC7		DDRCbit->N7

#define		DDRD0		DDRDbit->N0
#define		DDRD1		DDRDbit->N1
#define		DDRD2		DDRDbit->N2
#define		DDRD3		DDRDbit->N3
#define		DDRD4		DDRDbit->N4
#define		DDRD5		DDRDbit->N5
#define		DDRD6		DDRDbit->N6
#define		DDRD7		DDRDbit->N7

#define		PINB0		PINBbit->N0
#define		PINB1		PINBbit->N1
#define		PINB2		PINBbit->N2
#define		PINB3		PINBbit->N3
#define		PINB4		PINBbit->N4
#define		PINB5		PINBbit->N5
#define		PINB6		PINBbit->N6
#define		PINB7		PINBbit->N7

#define		PINC0		PINCbit->N0
#define		PINC1		PINCbit->N1
#define		PINC2		PINCbit->N2
#define		PINC3		PINCbit->N3
#define		PINC4		PINCbit->N4
#define		PINC5		PINCbit->N5
#define		PINC6		PINCbit->N6
#define		PINC7		PINCbit->N7

#define		PIND0		PINDbit->N0
#define		PIND1		PINDbit->N1
#define		PIND2		PINDbit->N2
#define		PIND3		PINDbit->N3
#define		PIND4		PINDbit->N4
#define		PIND5		PINDbit->N5
#define		PIND6		PINDbit->N6
#define		PIND7		PINDbit->N7


/************************************************************************/
/* SPI 相关寄存器地址														*/
/************************************************************************/

// 控制寄存器
#define SPCR_ADDRESS	0x2C

#undef SPR0
#undef SPR1
#undef CPHA
#undef CPOL
#undef MSTR
#undef DORD
#undef SPE
#undef SPIE

typedef union {
	struct {
		unsigned SPR0                 :1;
		unsigned SPR1                 :1;
		unsigned CPHA                 :1;
		unsigned CPOL                 :1;
		unsigned MSTR                 :1;
		unsigned DORD                 :1;
		unsigned SPE                  :1;
		unsigned SPIE                 :1;
	};
} SPCRbits_t;
extern volatile SPCRbits_t *SPCRbit;

// 状态寄存器
#define SPSR_ADDRESS	0x2D

#undef SPI2X
#undef WCOL
#undef SPIF

typedef union {
	struct {
		unsigned SPI2X					:1;
		unsigned						:5;
		unsigned WCOL					:1;
		unsigned SPIF					:1;
	};
} SPSRbits_t;
extern volatile SPSRbits_t *SPSRbit;



#ifdef	__cplusplus
}
#endif

#endif /* AVR_IO_168PA_H_ */