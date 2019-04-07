#include "bsp_delay.h"


void delay_us( int i)
 {     
    while( i--)
       {	

        }
 }		 


void delay_ms(unsigned int time)
	 {
	  while(time--)
	  	  {	
		   delay_us(1000);
		   
		  }
	 }
	 


