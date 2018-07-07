#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
 
#define USE_BLUETOOTH
	/*
	u8 USART2_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
	//接收状态
	//bit15，	接收完成标志
	//bit14，	接收到0x0d			(接收到的数据必须是0x0d 0x0a结尾)
	//bit13~0，	接收到的有效字节数目
	u16 USART2_RX_STA=0;       //接收状态标记	 
	*/

void WakeUpUSART(void);

u8 wakeUp = 0;		//wakeUp标记是否联系上Transmitter，当wakeUp为1后不用再发送wakeUp信息

//usart.c宏定义
extern u8 USART2_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
extern u16 USART2_RX_STA;       //接收状态标记	  
int main(void)
{		
	u8 temp=0;
	#ifdef USE_BLUETOOTH	
 	u16 t;  
	u16 len;	
	#endif
	
	
	delay_init();	    	 //延时函数初始化	  
	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级

	#ifdef USE_BLUETOOTH	
 	USART2_INIT(38400);	 //串口初始化为115200
	#endif

 	while(1)
	{
		#ifdef USE_BLUETOOTH	
		if(!wakeUp){
			WakeUpUSART();
		}
		else{
			if(USART2_RX_STA&0x8000)
			{					   
				len=USART2_RX_STA&0x3fff;//得到此次接收到的数据长度
				
				USART2_RX_STA=0;
			}
		}
		
		#endif
		
		delay_ms(1000);
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
