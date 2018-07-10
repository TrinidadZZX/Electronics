#include "led.h"
#include "delay.h"
#include "Timer.h"
#include "pid.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "stdlib.h"
#include "tim3.h"
#include "Motor.h"
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


u16 readHundrend(char *s);

int read_count=0;
int pid_count=0;
int recieve_count=0;
//TIM4 CH3:PB8
//			CH4:PB9


u8	pic_fre = 0;
u8	pic_count=0;
int main(void)
{		

	u8 readyForPID=0;
	u8 notStartPID=1;
	u16 len;	

	
	
	delay_init();	    	 //��ʱ������ʼ��	  
	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2: 2λ��ռ���ȼ���2λ��Ӧ���ȼ�
																					//TIM2 1,1		USART 1,0


 	USART2_INIT(9600);	 //���ڳ�ʼ��Ϊ115200
	
	TIM4_Config();

	//���������
	rayLocat.x = 120;
	rayLocat.y = 120;
	
	TIM3_Config();
	delay_ms(1000);
 	while(1)
	{
		
		
			if(USART2_RX_STA&0x8000)		//���ݽ������
			{					   
				len=USART2_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
				
				//��������
				if(USART2_RX_BUF[0] == 'x' && USART2_RX_BUF[1] == '=' && USART2_RX_BUF[5] == 'y' && USART2_RX_BUF[6] == '='){//x������
					ballLocat.x = readHundrend(USART2_RX_BUF+2);
					ballLocat.y = readHundrend(USART2_RX_BUF+7);
					readyForPID=1;
					read_count++;
					pic_count++;
				}
				
				USART2_RX_STA=0;				//Reset���ջ�����
			}
		
		if(readyForPID && notStartPID){
			TIM2_PID_Init();
			notStartPID=0;
		}
		delay_ms(1);
			
		
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


u16 readHundrend(char *s){
	u16 temp=0;
	temp=((*s -0x30) * 100)+((*(s+1)-0x30) * 10)+(*(s+2)-0x30);
	return temp;
}
