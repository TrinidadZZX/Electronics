#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "math.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "tim3.h"



extern u8 pic_count;				
extern u8 pic_fre;				//ͼ��Ƶ��
void TIM3_Config(void)  
{  
	
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    NVIC_InitTypeDef NVIC_InitStructure;		 //	  ����һ���ȼ�����

    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    TIM_TimeBaseStructure. TIM_Period = 7200;		     //���ü�����ֵ������ʱ���Զ����㣬�������ж�
    TIM_TimeBaseStructure.TIM_Prescaler = 10000;	   //ʱ��Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	 // ʱ�ӷ�Ƶ����
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	 // ������ʽΪ���ϼ���

    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);		 //��ʼ��tim2
    TIM_ClearITPendingBit(TIM3,TIM_IT_Update);             //���TIM2����жϱ�־
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);               //ʹ��TIM2����������ж�
    TIM_Cmd(TIM3,ENABLE);			     //ʹ��TIM2

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; //TIM2	��������ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //������ȼ�Ϊ1������һ����ͬ����ϣ���ж��໥��϶Է�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 	//��Ӧ���ȼ�1��������һ�����������ж�ͬʱ��ʱ����һ����ִ��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
                                          //ʹ�ܶ�ʱ��TIM4
} 
void TIM3_IRQHandler(void){

	
	
  TIM_ClearITPendingBit(TIM3,TIM_IT_Update);		 //���TIM2����ж���Ӧ������־λ
	
	pic_fre = pic_count;
	pic_count=0;
}