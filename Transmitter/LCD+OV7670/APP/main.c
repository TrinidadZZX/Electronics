#include <stdio.h>
#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "gui.h"
#include "test.h"
#include "exti.h"
#include "ov7670.h"
#include "usart.h"


#define xOfOV7670	70
#define yOfOV7670 30
#define USE_BLUETOOTH

//#define USE_LCD_PICTURE
#define USE_LCD_POINT
extern u8 ov_sta;	//在exit.c里面定义		0：表示FIFO存储的数据已经被读取了	1：目前正在往FIFO写数据	   2：可以开始读数据


u8 readyToSend=0;
extern u8 UART2_Read_Flag;												//若为1则可以读取数据
u16 readHundrend(char *s);
void SendHundred(int num);

/*蓝牙调参
	
*/
u16 r_threshold = 25 << 11;
u16 g_threshold = 55 << 5;
u16 b_threshold = 25;

void assert_failed(uint8_t* file, uint32_t line)
{
    printf("Wrong parameters value: file %s on line %d\r\n", file, line);
    while(1);
}


//更新显示
//设定所要区分的颜色，以及阈值	R:0, G:1, B:2

#ifdef USE_LCD_POINT
void camera_refresh(void)
#endif
#ifdef USE_LCD_PICTURE
void camera_refresh(void)
#endif
{
    u16 color=0;
    u32 j=0;
		u8 temp;
		
	
	
		int collector=0;
		u16 r_threshold = 25 << 11;
		u16 g_threshold = 55 << 5;
		u16 b_threshold = 25;
	
		u8 continious_flag = 0;		//
		u16 c_count = 0;					//collect 10 points to bingo
		u16 x;
		u16 y;
	
	
	
		

    if(ov_sta==2)
    {
        OV7670_CS=0;
        OV7670_RRST=0;
        OV7670_RCK=0;
        OV7670_RCK=1;
        OV7670_RCK=0;
        OV7670_RRST=1;
        OV7670_RCK=1;

				#ifdef USE_LCD_PICTURE
				LCD_SetWindows(0,0,320-xOfOV7670-1,320-yOfOV7670-1);				//LCD xOf 左边不要的猎术
        //LCD_SetWindows(0,0,320-1,240-1);				//LCD
				#endif
			
        for(j=0; j<76800; j++)
        {
            GPIOB->CRL=0X88888888;
            OV7670_RCK=0;
            color=OV7670_DATA;		//?áêy?Y
            OV7670_RCK=1;
            color<<=8;
            OV7670_RCK=0;
            color|=OV7670_DATA;		//?áêy?Y
            OV7670_RCK=1;
            GPIOB->CRL=0X33333333;
					
						#ifdef USE_LCD_PICTURE
						if((j%320)>=xOfOV7670 && (j/320)>=yOfOV7670)
            LCD_DrawPoint_16Bit(color);					//LCD
						#endif
					
						if((j%320)>=xOfOV7670 && (j/320)>=yOfOV7670){
							if(((unsigned int)(color&0xF800)>r_threshold)&&((unsigned int)(color&0x07E0)>g_threshold)&&((unsigned int)(color&0x001F)>b_threshold))
							{
								collector++;
								if(continious_flag){
									c_count++;
									if(c_count==3){
										x = j%320;
										y = j/320;
									}
								}
								else{
									continious_flag=1;
									c_count=0;
								}
								
							}
							else
								continious_flag=0;
						}
        }
        
				#ifdef USE_LCD_PICTURE
        LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);			//LCD
				
				#endif
				
				temp=(collector%10);
					LCD_ShowNum(0,240,temp,5,12);
				temp=(collector%100)/10;
					LCD_ShowNum(0,240,temp,4,12);
				temp=(collector%1000)/100;
					LCD_ShowNum(0,240,temp,3,12);
				temp=(collector%10000)/1000;
					LCD_ShowNum(0,240,temp,2,12);
				temp=(collector%100000)/10000;
					LCD_ShowNum(0,240,temp,1,12);
				
				temp=(x%10);
					LCD_ShowNum(0,270,temp,3,12);
				temp=(x%100)/10;
					LCD_ShowNum(0,270,temp,2,12);
				temp=(x%1000)/100;
					LCD_ShowNum(0,270,temp,1,12);
				
				
				temp=(y%10);
					LCD_ShowNum(0,300,temp,3,12);
				temp=(y%100)/10;
					LCD_ShowNum(0,300,temp,2,12);
				temp=(y%1000)/100;
					LCD_ShowNum(0,300,temp,1,12);
				
				if(readyToSend){		//发送"x=```Sy=```"
					printf("x=");
					SendHundred(x);
					printf("Sy=");
					SendHundred(y);
					printf("\r\n");//插入换行
				}
				
//				if(readyToSend){		//发送"x=```\0y=```"
//					printf("x=");
//					SendHundred(x);
//					printf("\0y=");
//					SendHundred(y);
//					printf("\r\n");//插入换行
//				}
				
        OV7670_CS=1;
        OV7670_RCK=0;
        OV7670_RCK=1;
        EXTI->PR=1<<15;
        ov_sta=0;
				
    }
}

void SendHundred(int num){
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
		USART_SendData(USART2, (num%1000)/100+0x30);
	
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
		USART_SendData(USART2, (num%100)/10+0x30);
	
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
		USART_SendData(USART2, (num%10)+0x30);
}


//usart.c宏定义
extern u8 USART2_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
extern u16 USART2_RX_STA;       //接收状态标记	 

int main(void)
{
	
	#ifdef USE_BLUETOOTH	
 	u16 t;  
	u16 len;	
	#endif
//		u8 numBit=0;		//每次发送两位数据	1高0低
//		u8 u8temp;
//		u8 mode=0;			//mode=1 RGB选择模式		mode=2阈值设定模式
//		u8 RGB=0;				//红绿蓝
		//u16 threshold=10;		//阈值 32，64，32
    //u16 i=0;
		//u8 readChar;

	SystemInit();       //初始化RCC 设置系统主频为72MHZ
    delay_init(72);     //延时初始化
		
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	
		//液晶
    LCD_Init();          //液晶屏初始化
	
		//摄像头
    OV7670_Init();    //摄像头初始化
    delay_ms(500);
    EXTI15_Init();						                    //使能定时器捕获
    OV7670_Window_Set(10,174,240,320);	//设置窗口
    OV7670_CS=0;

    delay_ms(500);

		//蓝牙串口
	#ifdef USE_BLUETOOTH	
 	USART2_INIT(38400);	 //串口初始化为38400
	#endif

	//delay_ms(5000);
  while(1)
  {
		#ifdef USE_BLUETOOTH
		camera_refresh();
		
		
		
//蓝牙调参（阈值）
//		if(USART2_RX_STA&0x8000)		//数据接收完毕
//		{					   
//				len=USART2_RX_STA&0x3fff;//得到此次接收到的数据长度
//				
//				//处理数据
//				if(USART2_RX_BUF[0] == 'r' &&USART2_RX_BUF[6] == 'g' && USART2_RX_BUF[12] == 'b'){//x轴坐标
//					r_threshold = readHundrend(USART2_RX_BUF+2) << 11;
//					g_threshold = readHundrend(USART2_RX_BUF+8) << 5;
//					b_threshold = readHundrend(USART2_RX_BUF+14);
//				}
//				
//				USART2_RX_STA=0;				//Reset接收缓冲区
//		}
		
		
		
//		double temp=100.123;
//		if(readyToSend){
//			printf("%f",temp);
//			printf("\r\n");//插入换行
//		}
//		delay_ms(1000);
//		delay_ms(1000);
		#endif

	}
}

u16 readHundrend(char *s){
	u16 temp=0;
	temp=((*s -0x30) * 100)+((*(s+1)-0x30) * 10)+(*(s+2)-0x30);
	return temp;
}


