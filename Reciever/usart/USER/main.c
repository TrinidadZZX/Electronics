#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
 
 
	/*
	u8 USART2_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
	//����״̬
	//bit15��	������ɱ�־
	//bit14��	���յ�0x0d			(���յ������ݱ�����0x0d 0x0a��β)
	//bit13~0��	���յ�����Ч�ֽ���Ŀ
	u16 USART2_RX_STA=0;       //����״̬���	 
	*/


int main(void)
{		
	delay_init();	    	 //��ʱ������ʼ��	  
	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�

	USART2_INIT(115200);	 //���ڳ�ʼ��Ϊ115200

 	while(1)
	{
		printf("Gay Lou.");
		delay_ms(1000);
	}	 
}

