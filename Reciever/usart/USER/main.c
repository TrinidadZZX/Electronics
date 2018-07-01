#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
 
 
	/*
	u8 USART2_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
	//接收状态
	//bit15，	接收完成标志
	//bit14，	接收到0x0d			(接收到的数据必须是0x0d 0x0a结尾)
	//bit13~0，	接收到的有效字节数目
	u16 USART2_RX_STA=0;       //接收状态标记	 
	*/


int main(void)
{		
	delay_init();	    	 //延时函数初始化	  
	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级

	USART2_INIT(115200);	 //串口初始化为115200

 	while(1)
	{
		printf("Gay Lou.");
		delay_ms(1000);
	}	 
}

