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


int main(void)
{		
	#ifdef USE_BLUETOOTH	
 	u16 t;  
	u16 len;	
	#endif
	
	
	delay_init();	    	 //延时函数初始化	  
	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级

	#ifdef USE_BLUETOOTH	
 	USART2_INIT(115200);	 //串口初始化为115200
	#endif

 	while(1)
	{
		#ifdef USE_BLUETOOTH	
		if(USART2_RX_STA&0x8000)//接收完成
		{					   
			len=USART2_RX_STA&0x3fff;//得到此次接收到的数据长度
			for(t=0;t<len;t++)
			{
				
				//LCD_ShowChar(0,len*5,BLUE, WHITE, USART2_RX_BUF[t],12,1);
				//USART_SendData(USART2, USART2_RX_BUF[t]);//向串口1发送数据
				//while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
			}
			//printf("\r\n");//插入换行
			USART2_RX_STA=0;
		}
		#endif
		
		delay_ms(1000);
	}	 
}

