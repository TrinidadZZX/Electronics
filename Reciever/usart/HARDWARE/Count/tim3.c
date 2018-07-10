#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "math.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "tim3.h"



extern u8 pic_count;				
extern u8 pic_fre;				//图像频率
void TIM3_Config(void)  
{  
	
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    NVIC_InitTypeDef NVIC_InitStructure;		 //	  命名一优先级变量

    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    TIM_TimeBaseStructure. TIM_Period = 7200;		     //配置计数阈值，超过时，自动清零，并触发中断
    TIM_TimeBaseStructure.TIM_Prescaler = 10000;	   //时钟预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	 // 时钟分频倍数
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	 // 计数方式为向上计数

    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);		 //初始化tim2
    TIM_ClearITPendingBit(TIM3,TIM_IT_Update);             //清除TIM2溢出中断标志
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);               //使能TIM2的溢出更新中断
    TIM_Cmd(TIM3,ENABLE);			     //使能TIM2

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; //TIM2	溢出更新中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //打断优先级为1，与上一个相同，不希望中断相互打断对方
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 	//响应优先级1，低于上一个，当两个中断同时来时，上一个先执行
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
                                          //使能定时器TIM4
} 
void TIM3_IRQHandler(void){

	
	
  TIM_ClearITPendingBit(TIM3,TIM_IT_Update);		 //清空TIM2溢出中断响应函数标志位
	
	pic_fre = pic_count;
	pic_count=0;
}