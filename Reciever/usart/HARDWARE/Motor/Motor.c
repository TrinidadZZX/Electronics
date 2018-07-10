#include "Motor.h"
#include "delay.h"
#define restriction 30
void Motor1(int angle1,int angle2){
		int x1, x2;
	int i;

	 x1=11*angle1+990                 ;                     // y=11x+990      -90-90   0---2000 
		x2=11*angle2+990                 ;
	if(angle1<angle2){
		for(i=x1; i<x2; i=i+11)
		{
			TIM_SetCompare3(TIM4,i);        
			delay_ms(5);
		}
	}
	else{
		for(i=x1; i>x2; i=i-11)
		{
			TIM_SetCompare3(TIM4,i);        
	    delay_ms(5);
		}
	}
}
void Motor2(int angle1,int angle2){
	
	int x1, x2;
	int i;
	
		
	 x1=11*angle1+990                 ;                     // y=11x+990      -90-90   0---2000 
		x2=11*angle2+990                 ;
	if(angle1<angle2){
		for(i=x1; i<x2; i=i+11)
		{
			TIM_SetCompare4(TIM4,i);        
			delay_ms(5);
		}
	}
	else{
		for(i=x1; i>x2; i=i-11)
		{
			TIM_SetCompare4(TIM4,i);        
	    delay_ms(5);
		}
	}
}