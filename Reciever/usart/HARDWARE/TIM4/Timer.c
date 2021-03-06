#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "math.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "Timer.h"





void TIM4_Config(void)  
{  
	
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;  //定义初始化结构体

  TIM_OCInitTypeDef  TIM_OCInitStructure;   //定义结构体
  GPIO_InitTypeDef GPIO_InitStructure; 
 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);	  //使能APB2控制外设的时钟，包括GPIOC, 功能复用时钟AFIO等，
                                                                              //其他包括的外设，详见固件库手册等资料
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //使能APB1控制外设的时钟，定时器4，其他外设详见手册
	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;           
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8|GPIO_Pin_9;	
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
	
	                                           
    TIM_TimeBaseStructure.TIM_Period =1999;         //自动重装载寄存器的值   频率为：72*10^6/(9999+1)/(143+1)=50Hz    2000-1
    TIM_TimeBaseStructure.TIM_Prescaler = 119;       //TIMX分频的值                130-1
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;   //时钟分割
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计�
		TIM_TimeBaseStructure.TIM_RepetitionCounter=0;  
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);   //根据以上功能对定时器初始化
   

   
	/* PWM1 Mode configuration: Channel1 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;    	//选择定时器模式，TIM宽度调制  
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;       //比较输出是使能         
    TIM_OCInitStructure.TIM_Pulse = 990;                                       //?????,???????????,??????  
    TIM_OCInitStructure.TIM_OCPolarity =TIM_OCPolarity_High;       
		TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;		//输出比较极高
    
		TIM_OC3Init(TIM4, &TIM_OCInitStructure);  
		TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable); 
		TIM_OC4Init(TIM4, &TIM_OCInitStructure);  
		TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);	
		
		TIM_ARRPreloadConfig(TIM4, ENABLE);                                         //
		
    /* TIM4 enable counter */  
    TIM_Cmd(TIM4, ENABLE); 	                                                    //使能定时器TIM4
} 

