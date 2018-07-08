#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "math.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "Timer.h"





void TIM4_Config(void)  
{  
	
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;  //�����ʼ���ṹ��

  TIM_OCInitTypeDef  TIM_OCInitStructure;   //����ṹ��
  GPIO_InitTypeDef GPIO_InitStructure; 
 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);	  //ʹ��APB2���������ʱ�ӣ�����GPIOC, ���ܸ���ʱ��AFIO�ȣ�
                                                                              //�������������裬����̼����ֲ������
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʹ��APB1���������ʱ�ӣ���ʱ��4��������������ֲ�
	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;           
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8|GPIO_Pin_9;	
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
	
	                                           
    TIM_TimeBaseStructure.TIM_Period =1999;         //�Զ���װ�ؼĴ�����ֵ   Ƶ��Ϊ��72*10^6/(9999+1)/(143+1)=50Hz    2000-1
    TIM_TimeBaseStructure.TIM_Prescaler = 119;       //TIMX��Ƶ��ֵ                130-1
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;   //ʱ�ӷָ�
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ��
		TIM_TimeBaseStructure.TIM_RepetitionCounter=0;  
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);   //�������Ϲ��ܶԶ�ʱ����ʼ��
   

   
	/* PWM1 Mode configuration: Channel1 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;    	//ѡ��ʱ��ģʽ��TIM��ȵ���  
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;       //�Ƚ������ʹ��         
    TIM_OCInitStructure.TIM_Pulse = 990;                                       //?????,???????????,??????  
    TIM_OCInitStructure.TIM_OCPolarity =TIM_OCPolarity_High;       
		TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;		//����Ƚϼ���
    
		TIM_OC3Init(TIM4, &TIM_OCInitStructure);  
		TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable); 
		TIM_OC4Init(TIM4, &TIM_OCInitStructure);  
		TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);	
		
		TIM_ARRPreloadConfig(TIM4, ENABLE);                                         //
		
    /* TIM4 enable counter */  
    TIM_Cmd(TIM4, ENABLE); 	                                                    //ʹ�ܶ�ʱ��TIM4
} 

