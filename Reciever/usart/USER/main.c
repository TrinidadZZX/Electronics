#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
 
#define USE_BLUETOOTH
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
	#ifdef USE_BLUETOOTH	
 	u16 t;  
	u16 len;	
	#endif
	
	
	delay_init();	    	 //��ʱ������ʼ��	  
	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�

	#ifdef USE_BLUETOOTH	
 	USART2_INIT(115200);	 //���ڳ�ʼ��Ϊ115200
	#endif

 	while(1)
	{
		#ifdef USE_BLUETOOTH	
		if(USART2_RX_STA&0x8000)//�������
		{					   
			len=USART2_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
			for(t=0;t<len;t++)
			{
				
				//LCD_ShowChar(0,len*5,BLUE, WHITE, USART2_RX_BUF[t],12,1);
				//USART_SendData(USART2, USART2_RX_BUF[t]);//�򴮿�1��������
				//while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
			}
			//printf("\r\n");//���뻻��
			USART2_RX_STA=0;
		}
		#endif
		
		delay_ms(1000);
	}	 
}

