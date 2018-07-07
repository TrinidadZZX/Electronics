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
	
typedef struct{
	u16 x;
	u16 y;
}Locat;
//С������λ��
Locat ballLocat;
//��������λ��
Locat rayLocat;

void WakeUpUSART(void);

u8 wakeUp = 0;		//wakeUp����Ƿ���ϵ��Transmitter����wakeUpΪ1�����ٷ���wakeUp��Ϣ

//usart.c�궨��
extern u8 USART2_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
extern u16 USART2_RX_STA;       //����״̬���	  
int main(void)
{		
	u8 temp=0;
	
 	u16 t;  
	u16 len;	

	
	
	delay_init();	    	 //��ʱ������ʼ��	  
	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2: 2λ��ռ���ȼ���2λ��Ӧ���ȼ�
																					//TIM2 1,1		USART 1,0


 	USART2_INIT(38400);	 //���ڳ�ʼ��Ϊ115200


 	while(1)
	{
		if(!wakeUp){
			WakeUpUSART();
		}
		else{
			if(USART2_RX_STA&0x8000)		//���ݽ������
			{					   
				len=USART2_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
				
				//��������
				
				
				USART2_RX_STA=0;				//Reset���ջ�����
			}
		}
		
		
		delay_ms(1000);
	}	 
}

void WakeUpUSART(void){
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);////ѭ������,ֱ���������   
		USART_SendData(USART2, 0x30);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);//ѭ������,ֱ���������   
		USART_SendData(USART2, 0x0d);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);//ѭ������,ֱ���������   
		USART_SendData(USART2, 0x0a);
}
