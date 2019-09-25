#include "ledtube.h"
#include <pic.h>


// 位选IO
#define     SEG_A_DIR       TRISC4
#define     SEG_A_IO        LATC4
#define     SEG_B_DIR       TRISA1
#define     SEG_B_IO        LATA1
#define     SEG_C_DIR       TRISA2
#define     SEG_C_IO        LATA2
#define     SEG_D_DIR       TRISC0
#define     SEG_D_IO        LATC0
#define     SEG_E_DIR       TRISC1
#define     SEG_E_IO        LATC1 
#define     SEG_F_DIR       TRISC2
#define     SEG_F_IO        LATC2
#define     SEG_G_DIR       TRISA5
#define     SEG_G_IO        LATA5    
#define     SEG_P_DIR       TRISA4
#define     SEG_P_IO        LATA4

// 段选IO
//#define     SEG_1_DIR       TRISC5
//#define     SEG_1_IO        LATC5
//#define     SEG_2_DIR       TRISC0
//#define     SEG_2_IO        LATC0
//#define     SEG_3_DIR       TRISA3
//#define     SEG_3_IO        RA3
#define     SEG_4_DIR       TRISA2
#define     SEG_4_IO        LATA2

// 74HC138 译码器IO
#define     SEG_1_DIR       TRISC5
#define     SEG_1_IO        LATC5
#define     SEG_2_DIR       TRISC3
#define     SEG_2_IO        LATC3
#define     SEG_3_DIR       TRISA3
#define     SEG_3_IO        RA3


// 位选
inline void display_place(unsigned char num) {
    
    // 段选
    switch(num) {
        case 0: 
                SEG_A_DIR = SEG_B_DIR = SEG_C_DIR = SEG_D_DIR = SEG_E_DIR = SEG_F_DIR = 0; 
                SEG_A_IO = SEG_B_IO = SEG_C_IO = SEG_D_IO = SEG_E_IO = SEG_F_IO = 0; 
                SEG_G_DIR = SEG_P_DIR = 1; 
                SEG_G_IO = SEG_P_IO = 0;
                break;
        case 1: 
                SEG_B_DIR = SEG_C_DIR = 0;
                SEG_B_IO = SEG_C_IO = 0;
                SEG_A_DIR = SEG_D_DIR = SEG_E_DIR = SEG_F_DIR = SEG_G_DIR = SEG_P_DIR = 1; 
                SEG_A_IO = SEG_D_IO = SEG_E_IO = SEG_F_IO = SEG_G_IO = SEG_P_IO = 0; 
                break;
        case 2: 
                SEG_A_DIR = SEG_B_DIR = SEG_G_DIR = SEG_E_DIR = SEG_D_DIR = 0; 
                SEG_A_IO = SEG_B_IO = SEG_G_IO = SEG_E_IO = SEG_D_IO = 0; 
                SEG_C_DIR = SEG_F_DIR = SEG_P_DIR = 1;
                SEG_C_IO = SEG_F_IO = SEG_P_IO = 0; 
                break;
        case 3: 
                SEG_A_DIR = SEG_B_DIR = SEG_C_DIR = SEG_D_DIR = SEG_G_DIR = 0; 
                SEG_A_IO = SEG_B_IO = SEG_C_IO = SEG_D_IO = SEG_G_IO = 0; 
                SEG_E_DIR = SEG_F_DIR = SEG_P_DIR = 1; 
                SEG_E_IO = SEG_F_IO = SEG_P_IO = 0; 
                break;   
        case 4: 
                SEG_B_DIR = SEG_C_DIR = SEG_F_DIR = SEG_G_DIR = 0; 
                SEG_B_IO = SEG_C_IO = SEG_F_IO = SEG_G_IO = 0; 
                SEG_A_DIR = SEG_D_DIR = SEG_E_DIR = SEG_P_DIR = 1; 
                SEG_A_IO = SEG_D_IO = SEG_E_IO = SEG_P_IO = 0;
                break;
        case 5: 
                SEG_A_DIR = SEG_C_DIR = SEG_D_DIR = SEG_F_DIR = SEG_G_DIR = 0; 
                SEG_A_IO = SEG_C_IO = SEG_D_IO = SEG_F_IO = SEG_G_IO = 0; 
                SEG_B_DIR = SEG_E_DIR = SEG_P_DIR = 1; 
                SEG_B_IO = SEG_E_IO = SEG_P_IO = 0; 
                break;    
        case 6: 
                SEG_A_DIR = SEG_C_DIR = SEG_D_DIR = SEG_E_DIR = SEG_F_DIR = SEG_G_DIR = 0; 
                SEG_A_IO = SEG_C_IO = SEG_D_IO = SEG_E_IO = SEG_F_IO = SEG_G_IO = 0; 
                SEG_B_DIR = SEG_P_DIR = 1; 
                SEG_B_IO = SEG_P_IO = 0;
                break;
        case 7: 
                SEG_A_DIR = SEG_B_DIR = SEG_C_DIR = 0; 
                SEG_A_IO = SEG_B_IO = SEG_C_IO = 0; 
                SEG_D_DIR = SEG_E_DIR = SEG_F_DIR = SEG_G_DIR = SEG_P_DIR = 1;
                SEG_D_IO = SEG_E_IO = SEG_F_IO = SEG_G_IO = SEG_P_IO = 0;
                break; 
        case 8: 
                SEG_A_DIR = SEG_B_DIR = SEG_C_DIR = SEG_D_DIR = SEG_E_DIR = SEG_F_DIR = SEG_G_DIR = 0; 
                SEG_A_IO = SEG_B_IO = SEG_C_IO = SEG_D_IO = SEG_E_IO = SEG_F_IO = SEG_G_IO = 0; 
                break;
        case 9: 
                SEG_A_DIR = SEG_B_DIR = SEG_C_DIR = SEG_D_DIR = SEG_F_DIR = SEG_G_DIR = 0; 
                SEG_A_IO = SEG_B_IO = SEG_C_IO = SEG_D_IO = SEG_F_IO = SEG_G_IO = 0; 
                SEG_E_DIR = SEG_P_DIR = 1; 
                SEG_E_IO = SEG_P_IO = 0;
                break;
        default: ;
        case 'E': 
                SEG_A_DIR = SEG_D_DIR = SEG_E_DIR = SEG_F_DIR = SEG_G_DIR = 0; 
                SEG_A_IO = SEG_D_IO = SEG_E_IO = SEG_F_IO = SEG_G_IO = 0; 
                SEG_B_DIR = SEG_C_DIR = SEG_P_DIR = 1; 
                SEG_B_IO = SEG_C_IO = SEG_P_IO =  0; 
                break;
    }    
}


void display_2seg(unsigned char num, unsigned char dp, unsigned char seg) {
    
    // 消隐
    switch(seg) {
        case 2: 
                SEG_1_DIR = 1; 
                SEG_1_IO = 0; 
                break;
        case 1: 
                SEG_2_DIR = 1; 
                SEG_2_IO = 0;  
                break;
    }   
    
    display_place(num);
    
    // 小数点
    if(dp) {
        SEG_P_DIR = 0;
        SEG_P_IO = 0;  
    }else {
        SEG_P_DIR = 1;
        SEG_P_IO = 0; 
    }

    // 位选
    switch(seg) {
        case 2: 
                SEG_2_DIR = 0; 
                SEG_2_IO = 1; 
                break;
        case 1: 
                SEG_1_DIR = 0; 
                SEG_1_IO = 1; 
                break;
    }      
}

void display_2seg_hc(unsigned char num, unsigned char dp, unsigned char seg) {
    
    // 消隐
    switch(seg) {
        case 4: 
                SEG_1_DIR = SEG_2_DIR = 0; 
                SEG_1_IO = 1; SEG_2_IO = 1;  
                break;
        case 3: 
                SEG_1_DIR = SEG_2_DIR = 0; 
                SEG_1_IO = 0; SEG_2_IO = 1;  
                break;
        case 2: 
                SEG_1_DIR = SEG_2_DIR = 0; 
                SEG_1_IO = 1; SEG_2_IO = 0;  
                break;
        case 1: 
                SEG_1_DIR = SEG_2_DIR = 0; 
                SEG_1_IO = SEG_2_IO = 0;  
                break;
    }    
 
    display_place(num);
    
    // 小数点
    if(dp) {
        SEG_P_DIR = 0;
        SEG_P_IO = 0;  
    }else {
        SEG_P_DIR = 1;
        SEG_P_IO = 0; 
    }

//    // 位选
//    switch(seg) {
//        case 4: 
//                SEG_1_DIR = SEG_2_DIR = 0; 
//                SEG_1_IO = 1; SEG_2_IO = 1;  
//                break;
//        case 3: 
//                SEG_1_DIR = SEG_2_DIR = 0; 
//                SEG_1_IO = 0; SEG_2_IO = 1;  
//                break;
//        case 2: 
//                SEG_1_DIR = SEG_2_DIR = 0; 
//                SEG_1_IO = 1; SEG_2_IO = 0;  
//                break;
//        case 1: 
//                SEG_1_DIR = SEG_2_DIR = 0; 
//                SEG_1_IO = SEG_2_IO = 0;  
//                break;
//    }    
}

// 4 位数码管
void  display_4seg(unsigned char num, unsigned char dp, unsigned char seg) {

    // 消隐
    switch(seg) {
        case 4: 
                SEG_1_DIR = SEG_2_DIR = SEG_3_DIR = 1; 
                SEG_1_IO = SEG_2_IO = SEG_3_IO = 0; 
                break;
        case 3: 
                SEG_1_DIR = SEG_2_DIR = SEG_4_DIR = 1; 
                SEG_1_IO = SEG_2_IO = SEG_4_IO = 0; 
                break;
        case 2: 
                SEG_1_DIR = SEG_3_DIR = SEG_4_DIR = 1; 
                SEG_1_IO = SEG_3_IO = SEG_4_IO = 0; 
                break;
        case 1: 
                SEG_2_DIR = SEG_3_DIR = SEG_4_DIR = 1; 
                SEG_2_IO = SEG_3_IO = SEG_4_IO = 0;  
                break;
    }   
    
    display_place(num);
    
    // 小数点
    if(dp) {
        SEG_P_DIR = 0;
        SEG_P_IO = 0;  
    }else {
        SEG_P_DIR = 1;
        SEG_P_IO = 0; 
    }

    // 位选
    switch(seg) {
        case 4: 
                SEG_4_DIR = 0; 
                SEG_4_IO = 1; 
                break;
        case 3: 
                SEG_3_DIR = 0; 
                SEG_3_IO = 1; 
                break;
        case 2: 
                SEG_2_DIR = 0; 
                SEG_2_IO = 1; 
                break;
        case 1: 
                SEG_1_DIR = 0; 
                SEG_1_IO = 1; 
                break;
    }    
}