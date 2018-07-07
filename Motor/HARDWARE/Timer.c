#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "math.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "Timer.h"

void RCC_Configuration(void)				 //ʹ���κ�һ������ʱ����ؿ�������Ӧ��ʱ��
{
  
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);	  //ʹ��APB2���������ʱ�ӣ�����GPIOC, ���ܸ���ʱ��AFIO�ȣ�
                                                                              //�������������裬����̼����ֲ������
  
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʹ��APB1���������ʱ�ӣ���ʱ��4��������������ֲ�

                
}

void GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
 
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;           
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;	
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
}



void TIM4_Config(void)  
{  
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;  //�����ʼ���ṹ��

     TIM_OCInitTypeDef  TIM_OCInitStructure;   //����ṹ��
     
    u16 CCR1= 1000;
    u16 CCR2= 1100;
    u16 CCR3= 1200;
		u16 CCR4= 1300;
	                                           
    TIM_TimeBaseStructure.TIM_Period =1999;         //�Զ���װ�ؼĴ�����ֵ   Ƶ��Ϊ��72*10^6/(9999+1)/(143+1)=50Hz    2000-1
    TIM_TimeBaseStructure.TIM_Prescaler = 119;       //TIMX��Ƶ��ֵ                130-1
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;   //ʱ�ӷָ�
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ��
		TIM_TimeBaseStructure.TIM_RepetitionCounter=0;  
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);   //�������Ϲ��ܶԶ�ʱ����ʼ��
   

   
	/* PWM1 Mode configuration: Channel1 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;    	//ѡ��ʱ��ģʽ��TIM��ȵ���  
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;       //�Ƚ������ʹ��         
    TIM_OCInitStructure.TIM_Pulse = CCR1;                                       //?????,???????????,??????  
    TIM_OCInitStructure.TIM_OCPolarity =TIM_OCPolarity_High;       
		TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;		//����Ƚϼ���
    
		TIM_OC1Init(TIM4, &TIM_OCInitStructure);                                    //���ݽṹ����Ϣ���г�ʼ��   
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);                           //���ܶ�ʱ��TIM4��CCR1�ϵ�Ԥװ��ֵ
		TIM_OC2Init(TIM4, &TIM_OCInitStructure);  
		TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);	
		TIM_OC3Init(TIM4, &TIM_OCInitStructure);  
		TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable); 
		TIM_OC4Init(TIM4, &TIM_OCInitStructure);  
		TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);	
		
		TIM_ARRPreloadConfig(TIM4, ENABLE);                                         //
		
    /* TIM4 enable counter */  
    TIM_Cmd(TIM4, ENABLE); 	                                                    //ʹ�ܶ�ʱ��TIM4
} 

