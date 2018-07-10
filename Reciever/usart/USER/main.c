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
	u8 USART2_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
	//接收状态
	//bit15，	接收完成标志
	//bit14，	接收到0x0d			(接收到的数据必须是0x0d 0x0a结尾)
	//bit13~0，	接收到的有效字节数目
	u16 USART2_RX_STA=0;       //接收状态标记	 
	*/

typedef struct{
	u16 x;
	u16 y;
}Locat;
//小球所在位置
Locat ballLocat;
//激光所在位置
Locat rayLocat;

void WakeUpUSART(void);

u8 wakeUp = 0;		//wakeUp标记是否联系上Transmitter，当wakeUp为1后不用再发送wakeUp信息

//usart.c宏定义
extern u8 USART2_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
extern u16 USART2_RX_STA;       //接收状态标记	  


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

	
	
	delay_init();	    	 //延时函数初始化	  
	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2: 2位抢占优先级，2位响应优先级
																					//TIM2 1,1		USART 1,0


 	USART2_INIT(9600);	 //串口初始化为115200
	
	TIM4_Config();

	//激光笔坐标
	rayLocat.x = 120;
	rayLocat.y = 120;
	
	TIM3_Config();
	delay_ms(1000);
 	while(1)
	{
		
		
			if(USART2_RX_STA&0x8000)		//数据接收完毕
			{					   
				len=USART2_RX_STA&0x3fff;//得到此次接收到的数据长度
				
				//处理数据
				if(USART2_RX_BUF[0] == 'x' && USART2_RX_BUF[1] == '=' && USART2_RX_BUF[5] == 'y' && USART2_RX_BUF[6] == '='){//x轴坐标
					ballLocat.x = readHundrend(USART2_RX_BUF+2);
					ballLocat.y = readHundrend(USART2_RX_BUF+7);
					readyForPID=1;
					read_count++;
					pic_count++;
				}
				
				USART2_RX_STA=0;				//Reset接收缓冲区
			}
		
		if(readyForPID && notStartPID){
			TIM2_PID_Init();
			notStartPID=0;
		}
		delay_ms(1);
			
		
	}	 
}

void WakeUpUSART(void){
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);////循环发送,直到发送完毕   
		USART_SendData(USART2, 0x30);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);//循环发送,直到发送完毕   
		USART_SendData(USART2, 0x0d);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);//循环发送,直到发送完毕   
		USART_SendData(USART2, 0x0a);
}


u16 readHundrend(char *s){
	u16 temp=0;
	temp=((*s -0x30) * 100)+((*(s+1)-0x30) * 10)+(*(s+2)-0x30);
	return temp;
}
