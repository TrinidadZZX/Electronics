/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */  

#include "include.h"
#include "Timer.h"
#include "delay.h"
int i;
void PB9set_angle1(int angle1,int angle2);
void PB9set_angle2(int angle1,int angle2);
void PB8set_angle1(int angle1,int angle2);
void PB8set_angle2(int angle1,int angle2);
void assert_failed(uint8_t* file, uint32_t line)
{
 printf("Wrong parameters value: file %s on line %d\r\n", file, line);
 while(1);
}
int x1,x2;                        //舵机设定值
void PB9set_angle1(int angle1,int angle2){
	  x1=11*angle1+990                 ;                     // y=11x+990      -90-90   0---2000 
	  x2=11*angle2+990                 ;
	for(i=x1; i<x2; i=i+11)
		{
			TIM_SetCompare4(TIM4,i);        
	    delay_ms(5);
		}
	
}
void PB9set_angle2(int angle1,int angle2){
	  x1=11*angle1+990                 ;                     // y=11x+990      -90-90   0---2000 
	  x2=11*angle2+990                 ;
	for(i=x1; i>x2; i=i-11)
		{
			TIM_SetCompare4(TIM4,i);        
	    delay_ms(5);
		}
	
}
void PB8set_angle1(int angle1,int angle2){
	  x1=11*angle1+990                 ;                     // y=11x+990      -90-90   0---2000 
	  x2=11*angle2+990                 ;
	for(i=x1; i<x2; i=i+11)
		{
			TIM_SetCompare3(TIM4,i);        
	    delay_ms(5);
		}
	
}
void PB8set_angle2(int angle1,int angle2){
	  x1=11*angle1+990                 ;                     // y=11x+990      -90-90   0---2000 
	  x2=11*angle2+990                 ;
	for(i=x1; i>x2; i=i-11)
		{
			TIM_SetCompare3(TIM4,i);        
	    delay_ms(5);
		}
	
}
int main(void)
{
	
	SystemInit();					//库函数，初始化RCC 设置系统主频为72MHZ
	delay_init(72);	     //延时初始化（自己编写的延时函数，详见APP的delay.c）

	RCC_Configuration();
	GPIO_Config();
	TIM4_Config();
  while (1)
  {
		PB9set_angle1(-45,45);
		PB9set_angle2(45,-45);
		PB8set_angle1(-90,90);
		PB8set_angle2(90,-90);
  }
}
