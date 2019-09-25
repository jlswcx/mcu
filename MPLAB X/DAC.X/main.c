#include "fosc.h"
#include "port.h"
#include "ledtube.h"

#include "config.h"

void main(void) {
    
    FOSC_Init();
    
    PORT_Init();
    
    WPUA5 = 0;
    OPTION_REGbits.nWPUEN = 1; 
    
    while(1) {
        
 //       TRA0 = 0;
  //      RA0 = 1;
        
  //      TRA5 = 1;
  //      RA5 = 0;
        
        display_2seg_hc(6, 0, 3);
    }
}
