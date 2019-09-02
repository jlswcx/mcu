
#include <stdio.h>
#include <stdlib.h>

#include <pic.h>

// PIC16F1823 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select (MCLR/VPP pin function is digital input)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable (Brown-out Reset disabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = OFF       // Internal/External Switchover (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is disabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN = OFF      // PLL Enable (4x PLL disabled)
#pragma config STVREN = OFF     // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will not cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = OFF         // Low-Voltage Programming Enable (Low-voltage programming enabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

//#define     DO      262
//#define     DO      277
//#define     DO      294
//#define     DO      311
//#define     DO      330
//#define     DO      349
//#define     DO      370
//#define     DO      392
//#define     DO      415
//#define     DO      440
//#define     DO      466
//#define     DO      494
//
//#define     DO      523
//#define     DO      554
//#define     DO      587
//#define     DO      622
//#define     DO      659
//#define     DO      698
//#define     DO      740
//#define     DO      784
//#define     DO      831
//#define     DO      880
//#define     DO      932
//#define     DO      988
//
//#define     DO      1046
//#define     DO      1109
//#define     DO      1175
//#define     DO      1245
//#define     DO      1318
//#define     DO      1397
//#define     DO      1480
//#define     DO      1568
//#define     DO      1661
//#define     DO      1760
//#define     DO      1865
//#define     DO      1976

#define     SOUND_SPACE 	4/5 		//������ͨ��������ĳ��ȷ���,//ÿ4���������
#define     BeepIO          RC5         //��������ܽ�

//ͬһ�׸�

//ͬһ�׸�



unsigned char Sound_Temp_TH0,Sound_Temp_TL0, Sound_Temp_0;	//������ʱ����ֵ�ݴ� 
unsigned char Sound_Temp_TH1,Sound_Temp_TL1;	//������ʱ����ֵ�ݴ�

/*˵��**************************************************************************
 ���״�����ʽ unsigned char Music_Encode{���ߣ����������ߣ�����...., 0,0};	ĩβ:0,0 ��ʾ����(Important)

 ��������λ������ɣ�
 		��λ�Ǳ�ʾ 1~7 ���߸����� 
 		ʮλ�Ǳ�ʾ�������ڵ�����:1-������2-������3-����;
 		��λ��ʾ��������Ƿ�Ҫ������: 0-������1-��������
 
 �����������λ������ɣ� 
 		��λ��ʾ������ʱֵ�����Ӧ��ϵ�ǣ� 
 			|��ֵ(n):  |0 |1 |2 |3 | 4 | 5 | 6 
 			|��������: |1 |2 |4 |8 |16 |32 |64 		����=2^n
 		ʮλ��ʾ����������Ч��(0-2):  0-��ͨ��1-������2-����
 		��λ�Ƿ���λ: 0-�޷��㣬1-�з���
 * 
 * 
 */
void main() {

    while(1) {
        NOP();
    }
}


void TIME1_Init() {
    
    	Sound_Temp_TH1 = (65535-(1/1200)*_XTAL_FREQ)/256;	// ����TL1Ӧװ��ĳ�ֵ 	(10ms�ĳ�װֵ)
	Sound_Temp_TL1 = (65535-(1/1200)*_XTAL_FREQ)%256;	// ����TH1Ӧװ��ĳ�ֵ 
	TMR1H = Sound_Temp_TH1;
	TMR1L = Sound_Temp_TL1;
    
    
    GIE = 1;
    PEIE = 1;
    
    TMR0IE = 1;             // ��ʱ��0 ʹ��λ
//    PSA = 0;                // ���ö�ʱ��0 ��Ƶ��
    OPTION_REG = 0x88;      //Ԥ��ƵΪ8
    
    TMR1IE = 1;             // ��ʱ��1 ʹ��λ
    TMR1ON = 1;             // ��ʱ��1 ʹ��λ

    TMR0IF = 0;             // ��ʱ��0 �жϱ�־λ
    TMR1IF = 0;             // ��ʱ��1 �жϱ�־λ
    
}


//**************************************************************************
void Play(unsigned char *Sound,unsigned char Signature,unsigned Octachord,unsigned int Speed)
{
	unsigned int NewFreTab[12];		//�µ�Ƶ�ʱ�
	unsigned char i,j;
	unsigned int Point,LDiv,LDiv0,LDiv1,LDiv2,LDiv4,CurrentFre,Temp_T,SoundLength;
	unsigned char Tone,Length,SL,SH,SM,SLen,XG,FD;
	for(i=0;i<12;i++) 				// ���ݵ��ż������˶��������µ�Ƶ�ʱ� 
	{
		j = i + Signature;
		if(j > 11)
		{
			j = j-12;
			NewFreTab[i] = FreTab[j]*2;
		}
		else
			NewFreTab[i] = FreTab[j];

		if(Octachord == 1)
			NewFreTab[i]>>=2;
		else if(Octachord == 3)
			NewFreTab[i]<<=2;
	}									
	
	SoundLength = 0;
	while(Sound[SoundLength] != 0x00)	//�����������
	{
		SoundLength+=2;
	}

	Point = 0;
	Tone   = Sound[Point];	
	Length = Sound[Point+1]; 			// ������һ����������ʱʱֵ
	
	LDiv0 = 12000/Speed;				// ���1�������ĳ���(����10ms) 	
	LDiv4 = LDiv0/4; 					// ���4�������ĳ��� 
	LDiv4 = LDiv4-LDiv4*SOUND_SPACE; 	// ��ͨ��������׼ 
	TMR0IE	  = 0;
	TMR1IE   = 1;
	while(Point < SoundLength)
	{
		SL=Tone%10; 								//��������� 
		SM=Tone/10%10; 								//������ߵ��� 
		SH=Tone/100; 								//������Ƿ����� 
		CurrentFre = NewFreTab[SignTab[SL-1]+SH]; 	//�����Ӧ������Ƶ�� 	
		if(SL!=0)
		{
			if (SM==1) CurrentFre >>= 2; 		//���� 
			if (SM==3) CurrentFre <<= 2; 		//����
			Temp_T = 256-(_XTAL_FREQ/(4*CurrentFre));//�����������ֵ
            Sound_Temp_0 = Temp_T;
			TMR0 = Sound_Temp_0;
		}
		SLen=LengthTab[Length%10]; 	//����Ǽ�������
		XG=Length/10%10; 			//�����������(0��ͨ1����2����) 
		FD=Length/100;
		LDiv=LDiv0/SLen; 			//���������������ĳ���(���ٸ�10ms)
		if (FD==1) 
			LDiv=LDiv+LDiv/2;
		if(XG!=1)	
			if(XG==0) 				//�����ͨ���������೤�� 
				if (SLen<=4)	
					LDiv1=LDiv-LDiv4;
				else
					LDiv1=LDiv*SOUND_SPACE;
			else
				LDiv1=LDiv/2; 		//������������೤�� 
		else
			LDiv1=LDiv;
		if(SL==0) LDiv1=0;
			LDiv2=LDiv-LDiv1; 		//����������ĳ��� 
	  	if (SL!=0)
		{
			TMR0IE=1;
			for(i=LDiv1;i>0;i--) 	//���涨���ȵ��� 
			{
				while(TMR1IF==0);
				TMR1H = Sound_Temp_TH1;
				TMR1L = Sound_Temp_TL1;
				TMR1IF=0;
			}
		}
		if(LDiv2!=0)
		{
			TMR0IE=0; BeepIO=1;
			for(i=LDiv2;i>0;i--) 	//������ļ��
			{
				while(TMR1IF==0);
				TMR1H = Sound_Temp_TH1;
				TMR1L = Sound_Temp_TL1;
				TMR1IF=0;
			}
		}
		Point+=2; 
		Tone=Sound[Point];
		Length=Sound[Point+1];
	}
	BeepIO = 1;
}



void PORTA_Init() {
    TRISA = 0;
    PORTA = 0;
    LATA = 0;
    ANSELA = 0;
}

void PORTC_Init() {
    TRISC = 0;
    PORTC = 0;
    LATC = 0;
    ANSELC = 0;
}

void PORT_Init() {
    PORTA_Init();
    PORTC_Init();
}

void TMR0_Init() {
    GIE = 1;
    PEIE = 1;
    TMR0IE = 1;             // ��ʱ��
    
    OPTION_REG = 0x88;      //Ԥ��ƵΪ8
    
    TMR0IF = 0;
    TMR0 = 0;
}

void main() {
    OSCCON = 0X28;          // ��ʼ������
    
    unsigned temp;
    
    PORT_Init();
    TMR0_Init();
//    TIME1_Init();
    
//    Play(Music_Encode,0,2,1);
 //    Beep();
    while(1) {
    

        for(int i = 0; i < 12; i++) {
            
            Sound_Temp_0 = 256-(_XTAL_FREQ/(4*(FreTab[i]*3)));//�����������ֵ
            TMR0 = Sound_Temp_0;
            __delay_ms(200);
        }
        
        NOP();
    }

}


void interrupt isr() {
    if(TMR0IF) {    
        TRISC5 = !TRISC5;
        TMR0IF = 0;
        TMR0 = Sound_Temp_0;
    }
}

void Beep() {
    
    TRISC5 = 0;
    RC5 = 1;
    
    
    while(1) {
        
        RC5 = 0;
        __delay_us(600);
        RC5 = 1;
        __delay_us(600);
        
    }
    
    
}

