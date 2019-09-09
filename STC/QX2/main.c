#include "stc8fxx.h"
#include "intrins.h"
#incldue "delay.h"

sbit	LED_White   = P1^1;
sbit	LED_Red     = P1^2;

sbit	LED_Key     = P3^2;


#define     LED_PWM_LIMIT_MAX   49
#define     LED_PWM_LIMIT_MIN   0

static unsigned int s_u8TimeCounter     = 0;        // �жϼ���
static unsigned int s_u8LedDirection   = 0;         // 0������ 1������

static signed int s_s8LedPWMCounter  = 0;           // LEDռ�ձ�

static unsigned char g_th0 = 0xff;                  // 
static unsigned char g_tl0 = 0x00;                  // 

static unsigned int s_s8PWMCounter2 = 2000;


static unsigned char g_sec = 0;      // 


#define     CYCLE   0x1000      //����PWM����(���ֵΪ32767)
#define     DUTY    10L         //����ռ�ձ�Ϊ10%


void Init(void) {
    
    P1NCS = 0;
    
    LED_White   = 0;
    LED_Red     = 0;
    
}


void Delay2500us()		//@24.000MHz
{
	unsigned char i, j;

	_nop_();
	_nop_();
	i = 78;
	j = 233;
	do
	{
		while (--j);
	} while (--i);
}





void test() {
    
    while(1){    
        LED_White = 1;
        Delay1us();
        LED_White = 0;
        Delay2000us();
    }
    
}

void PWM_Init() {
    
    P0M0 = 0x00;                //��PWMģ�����IO�ڳ�ʼ״̬Ϊ���裬��Ҫ��IO������Ϊ׼˫������������������������Σ�
    P0M1 = 0x00;
    P1M0 = 0x00;
    P1M1 = 0x00;
    P2M0 = 0x00;
    P2M1 = 0x00;
    P3M0 = 0x00;
    P3M1 = 0x00;
    P4M0 = 0x00;
    P4M1 = 0x00;
    
    
    P_SW2 |= 0x80;              //ʹ�ܷ���XSFR�������޷�������������Ĵ���
    PWMCFG = 0x00;              //����PWM�������ʼ��ƽΪ�͵�ƽ��Ҳ���ǵ�һ�η�תǰ����ĵ�ƽ
    PWMCKS = 0x00;              //ѡ��PWM��ʱ��ΪFosc/(0+1)������FOSCΪ�ⲿ���ڲ�ϵͳʱ�Ӿ���Ƶ�����Ƭ���Ĺ���ʱ��
    
    PWMC = CYCLE;               //����PWM����(���ֵΪ32767)���üĴ���Ϊ15λ��ʵ��ʹ��ʱ��ö��������β�Ϊunsigned int
    PWM0T1 = 0x0000;            //����PWM2��1�η�ת��PWM����,Ҳ���ǵ�ƽ��һ�η�����ת�ļ���ֵ                                                            
                                //�����ж���PWM2T1Ϊ0����ʼ��ƽҲΪ0��������һ��ʼ��Ҳ���Ǽ���Ϊ0ʱ��ֱ�ӷ�תΪ�ߣ������������ռ�ձ�
    PWM0T2 = 0x0001;            //����PWM2��2�η�ת��PWM��������ʵ�������Ĵ��������Ⱥ�û�е�1��2֮�֣�ֻ�����������㣬�����ƽ��ת
                                //ռ�ձ�Ϊ(PWM2T2-PWM2T1)/PWMC
    PWM0CR = 0x80;              //ѡ��PWM2�����P3.7,��ʹ��PWM2�жϣ�Ҳ����ͨ���üĴ����л�Ҫ���PWM��IO��

    P_SW2 = 0x00;
    
    PWMCR = 0Xc0;               //ʹ��PWM�ź����
    
    EA = 1;  
}

void PWM_Isr() interrupt 22 
{
    static bit dir = 1;
    static int val = 0;
    
    if(PWMCFG & 0x80) {
        
        PWMCFG &= ~0x80;        // ���жϱ�־
        if(dir) {
            val++;
            if(val >= CYCLE) 
                dir = 0;
        }else {
            val--;
            if(val <= 1) 
                dir = 1;
        }
        
        _push_(P_SW2);
        P_SW2 |= 0x80;
        PWM0T2 = val;
        _pop_(P_SW2);

    } 
}


void Timer0_Init(void) {
    
    unsigned short g_us = 200;
    g_th0 = (65536-g_us)/256;
    g_tl0 = (65536-g_us)%256;
    

	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
//	TL0 = 0x40;		//���ö�ʱ��ֵ
//	TH0 = 0xED;		//���ö�ʱ��ֵ
    
    	TL0 = 0x88;		//���ö�ʱ��ֵ
	TH0 = 0xFF;		//���ö�ʱ��ֵ
    
	TF0 = 0;		//���TF0��־
	TR0 = 0;		//��ʱ��0��ʼ��ʱ
    
    
    ET0 = 0;
    
    EA = 1;
}

void TM0_Rountine(void) interrupt  1 {
    
    static signed int s_s8PWMCounter = 0;
    
    TL0 = 0x88;		//���ö�ʱ��ֵ
	TH0 = 0xFF;		//���ö�ʱ��ֵ

    
    if(++s_u8TimeCounter >= s_s8PWMCounter2)
    {
        s_u8TimeCounter = 0;
    
        if(0 == s_u8LedDirection) {
            s_s8LedPWMCounter++;
            s_s8PWMCounter2 = s_s8PWMCounter2 - 5;
            
            if(s_s8PWMCounter2 <= 0){
                s_u8LedDirection = 1;
                TR0 = 0;
                ET0 = 0;
            }   
        }
        
        if(1 == s_u8LedDirection) {
            s_s8LedPWMCounter--;
            s_s8PWMCounter2 = s_s8PWMCounter2 + 5;
            
            if(s_s8PWMCounter2 >= 2000){
                s_u8LedDirection = 0;
                TR0 = 0;
                ET0 = 0;
            }
        }
        
        s_s8PWMCounter = s_s8LedPWMCounter;
    }
    
    if(s_s8PWMCounter > 0)
    {
        LED_White = 1;
        s_s8PWMCounter--;
    }else {
        LED_White = 0;
    }  
    
}


//void TM0_Rountine(void) interrupt  1 {
//    
//    static signed int s_s8PWMCounter = 0;
//    
//	TL0 = 0x40;		//���ö�ʱ��ֵ
//	TH0 = 0xED;		//���ö�ʱ��ֵ
//   
//        
//        if(++s_u8TimeCounter >= 200) {          // ÿ20MS����LED��ռ�ձ�
//            
//            s_u8TimeCounter = 0;                // �жϼ���
//            
//            if((s_s8LedPWMCounter <= LED_PWM_LIMIT_MAX) && (0 == s_u8LedDirection)) {
//                
//                s_s8LedPWMCounter++;
//                
//                if(s_s8LedPWMCounter > LED_PWM_LIMIT_MAX) {

//                    s_u8LedDirection = 1;
//                    s_s8LedPWMCounter = LED_PWM_LIMIT_MAX;  
//                    
//                    TR0 = 0;
//                    ET0 = 0;
//                }
//                

//                    s_s8PWMCounter = s_s8LedPWMCounter;

//            }
//            
//            
//            if((s_s8LedPWMCounter >= LED_PWM_LIMIT_MIN) && (1 == s_u8LedDirection)) {
//                
//                s_s8LedPWMCounter--;
//                
//                if(s_s8LedPWMCounter <  LED_PWM_LIMIT_MIN) {
//                    
//                    s_u8LedDirection = 0;
//                    s_s8LedPWMCounter = LED_PWM_LIMIT_MIN;
//                    
//                    TR0 = 0;
//                    ET0 = 0;
//                }  
//                
//                s_s8PWMCounter = s_s8LedPWMCounter;
//            }
//            
//            
//        }
//        
//        if(s_s8PWMCounter > 0)
//        {
//            LED_White = 1;
//            s_s8PWMCounter--;
//        }else {
//            LED_White = 0;
//        }  

//}

void INT0_Init(void) {
    IT0 = 1;
    EX0 = 1;
    EA = 1;
}

void INT0_Isr() interrupt 0
{
    static char t = 0;
    TR0 = 0;
    ET0 = 0;
    if(t == 0) {
        g_sec = 0;
    }

    TR0 = 1;
    ET0 = 1;
}


void main() {

//    test();
    
    Init();
    
    INT0_Init();
    Timer0_Init();
    
    while(1);
}
