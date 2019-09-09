#include "stc8fxx.h"
#include "intrins.h"
#incldue "delay.h"

sbit	LED_White   = P1^1;
sbit	LED_Red     = P1^2;

sbit	LED_Key     = P3^2;


#define     LED_PWM_LIMIT_MAX   49
#define     LED_PWM_LIMIT_MIN   0

static unsigned int s_u8TimeCounter     = 0;        // 中断计数
static unsigned int s_u8LedDirection   = 0;         // 0：渐亮 1：渐灭

static signed int s_s8LedPWMCounter  = 0;           // LED占空比

static unsigned char g_th0 = 0xff;                  // 
static unsigned char g_tl0 = 0x00;                  // 

static unsigned int s_s8PWMCounter2 = 2000;


static unsigned char g_sec = 0;      // 


#define     CYCLE   0x1000      //定义PWM周期(最大值为32767)
#define     DUTY    10L         //定义占空比为10%


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
    
    P0M0 = 0x00;                //因PWM模块相关IO口初始状态为高阻，需要将IO口设置为准双向或推挽输出才能正常输出波形；
    P0M1 = 0x00;
    P1M0 = 0x00;
    P1M1 = 0x00;
    P2M0 = 0x00;
    P2M1 = 0x00;
    P3M0 = 0x00;
    P3M1 = 0x00;
    P4M0 = 0x00;
    P4M1 = 0x00;
    
    
    P_SW2 |= 0x80;              //使能访问XSFR，否则无法访问以下特殊寄存器
    PWMCFG = 0x00;              //配置PWM的输出初始电平为低电平，也就是第一次翻转前输出的电平
    PWMCKS = 0x00;              //选择PWM的时钟为Fosc/(0+1)，其中FOSC为外部或内部系统时钟经分频后给单片机的工作时钟
    
    PWMC = CYCLE;               //设置PWM周期(最大值为32767)，该寄存器为15位，实际使用时最好定义周期形参为unsigned int
    PWM0T1 = 0x0000;            //设置PWM2第1次反转的PWM计数,也就是电平第一次发生翻转的计数值                                                            
                                //此例中定义PWM2T1为0，初始电平也为0，所以在一开始，也就是计数为0时，直接翻转为高，这样方便计算占空比
    PWM0T2 = 0x0001;            //设置PWM2第2次反转的PWM计数，其实这两个寄存器不分先后，没有第1第2之分，只是设置两个点，到点电平翻转
                                //占空比为(PWM2T2-PWM2T1)/PWMC
    PWM0CR = 0x80;              //选择PWM2输出到P3.7,不使能PWM2中断，也可以通过该寄存器切换要输出PWM的IO口

    P_SW2 = 0x00;
    
    PWMCR = 0Xc0;               //使能PWM信号输出
    
    EA = 1;  
}

void PWM_Isr() interrupt 22 
{
    static bit dir = 1;
    static int val = 0;
    
    if(PWMCFG & 0x80) {
        
        PWMCFG &= ~0x80;        // 清中断标志
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
    

	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
//	TL0 = 0x40;		//设置定时初值
//	TH0 = 0xED;		//设置定时初值
    
    	TL0 = 0x88;		//设置定时初值
	TH0 = 0xFF;		//设置定时初值
    
	TF0 = 0;		//清除TF0标志
	TR0 = 0;		//定时器0开始计时
    
    
    ET0 = 0;
    
    EA = 1;
}

void TM0_Rountine(void) interrupt  1 {
    
    static signed int s_s8PWMCounter = 0;
    
    TL0 = 0x88;		//设置定时初值
	TH0 = 0xFF;		//设置定时初值

    
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
//	TL0 = 0x40;		//设置定时初值
//	TH0 = 0xED;		//设置定时初值
//   
//        
//        if(++s_u8TimeCounter >= 200) {          // 每20MS调整LED的占空比
//            
//            s_u8TimeCounter = 0;                // 中断计数
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
