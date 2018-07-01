#include <stdio.h>
#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "gui.h"
#include "test.h"
#include "exti.h"
#include "ov7670.h"
#include "usart.h"

#define USE_BLUETOOTH

#define USE_LCD_PICTURE
//#define USE_LCD_POINT
extern u8 ov_sta;	//在exit.c里面定义		0：表示FIFO存储的数据已经被读取了	1：目前正在往FIFO写数据	   2：可以开始读数据

extern u8 UART2_Read_Flag;												//若为1则可以读取数据

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
	
		
	
	
		#ifdef USE_LCD_POINT
		int collector=0;
		u16 r_threshold = 15 << 11;
		u16 g_threshold = 30 << 5;
		u16 b_threshold = 15;
	
		u8 continious_flag = 0;		//
		u16 c_count = 0;					//collect 10 points to bingo
		u16 x;
		u16 y;
	
	
		#endif
	
		

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
        LCD_SetWindows(0,0,320-1,240-1);				//LCD
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
            LCD_DrawPoint_16Bit(color);					//LCD
						#endif
					
						#ifdef USE_LCD_POINT
						if(((unsigned int)(color&0xF800)>r_threshold)&&((unsigned int)(color&0x07E0)>g_threshold)&&((unsigned int)(color&0x001F)>b_threshold))
						{
							collector++;
							if(continious_flag){
								c_count++;
								if(c_count==10){
									x = j%320;
									y = j/320;
								}
								else{
									continious_flag=1;
									c_count=0;
								}
							}
						}
						else
							continious_flag=0;
						#endif
        }
        
				#ifdef USE_LCD_PICTURE
        LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);			//LCD
				#endif
				
				#ifdef USE_LCD_POINT
				if(collector/10000)
					LCD_ShowNum(0,0,collector,5,12);
				else if(collector/1000)
					LCD_ShowNum(0,0,collector,4,12);
				else if(collector/100)
					LCD_ShowNum(0,0,collector,3,12);
				else if(collector/10)
					LCD_ShowNum(0,0,collector,2,12);
				else
					LCD_ShowNum(0,0,collector,1,12);
				
				if(x/100)
					LCD_ShowNum(0,30,x,3,12);
				else if(x/10)
					LCD_ShowNum(0,30,x,2,12);
				else
					LCD_ShowNum(0,30,x,1,12);
				
				if(y/100)
					LCD_ShowNum(0,60,y,3,12);
				else if(y/10)
					LCD_ShowNum(0,60,y,2,12);
				else
					LCD_ShowNum(0,60,y,1,12);
				#endif
				
        OV7670_CS=1;
        OV7670_RCK=0;
        OV7670_RCK=1;
        EXTI->PR=1<<15;
        ov_sta=0;
				
    }
}


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
 	USART2_INIT(115200);	 //串口初始化为115200
	#endif

  while(1)
  {
		//camera_refresh();
			
		if(USART2_RX_STA&0x8000)//接收完成
		{					   
			len=USART2_RX_STA&0x3fff;//得到此次接收到的数据长度
			for(t=0;t<len;t++)
			{
				
				LCD_ShowChar(0,len*5,BLUE, WHITE, USART2_RX_BUF[t],12,1);
				//USART_SendData(USART2, USART2_RX_BUF[t]);//向串口1发送数据
				//while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
			}
			//printf("\r\n\r\n");//插入换行
			USART2_RX_STA=0;
		}

	}
}
										
								

