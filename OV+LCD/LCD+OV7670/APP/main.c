#include <stdio.h>
#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "gui.h"
#include "test.h"
#include "exti.h"
#include "ov7670.h"
#include "UART2.h"
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
int camera_refresh(u8 RGB, u8 threshold)
#endif
#ifdef USE_LCD_PICTURE
void camera_refresh(void)
#endif
{
    u16 color=0;
    u32 j=0;
	
		#ifdef USE_LCD_POINT
		int collector=0;
		u16 temp;
		switch(RGB){
			case 0 : 	temp = threshold<<11;break;
			case 1 :	temp = threshold<<5;break;
			case 2 :	temp = threshold;break;
			default:	return 0;break;
		}
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
						switch(RGB){
							case 0: color = color & 0xF800;
									if(color>temp) collector++;break;
							case 1: color = color & 0x07E0;
									if(color>temp) collector++;break;
							case 2: color = color & 0x001F;
									if(color>temp) collector++;break;
							default: return 0;break;
						}
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
				#endif
				
        OV7670_CS=1;
        OV7670_RCK=0;
        OV7670_RCK=1;
        EXTI->PR=1<<15;
        ov_sta=0;
				
				#ifdef USE_LCD_POINT
				return collector;
				#endif
    }
}


int main(void)
{
		u8 numBit=0;		//每次发送两位数据	1高0低
		u8 u8temp;
		u8 mode=0;			//mode=1 RGB选择模式		mode=2阈值设定模式
		u8 RGB=0;				//红绿蓝
		u16 threshold=10;		//阈值 32，64，32
    //u16 i=0;
		u8 readChar;
    SystemInit();       //初始化RCC 设置系统主频为72MHZ
    delay_init(72);     //延时初始化
	
		//液晶
    LCD_Init();          //液晶屏初始化
	
		//摄像头
    OV7670_Init();    //摄像头初始化
    delay_ms(500);
    EXTI15_Init();						                    //使能定时器捕获
    OV7670_Window_Set(10,174,240,320);	//设置窗口
    OV7670_CS=0;

    delay_ms(500);
//	
		//蓝牙串口
		Initial_UART2(38400);
		delay_ms(500);

    while(1)
    {
        //i++;
				//UART2_Put_String("Another Try\r\n");
        //camera_refresh(RGB,threshold);	//更新显示
				camera_refresh();
				delay_ms(100);
				if(UART2_Read_Flag){
					readChar = UART_Read_Char();
					switch(readChar){
						case 'C': mode=1;break;			//color模式
						case 'T': mode=2;break;			//阈值模式
						case 'R': if(mode==1)RGB=1;break;			//红
						case 'G': if(mode==1)RGB=2;break;			//绿
						case 'B': if(mode==1)RGB=3;break;			//蓝
						case 'M': numBit=1;break;
						case 'L': numBit=0;break;
						default:
							if(mode == 2){
								if(readChar>0x30 && readChar<0x39){
									if(numBit==0)
										threshold = threshold/10*10 + readChar-0x30;
									else if(numBit==1)
										threshold = threshold%10 +(readChar-0x30)*10;
								}
							}break;
						}
					}
		}
}
										
								
//							if(mode == 2){
//								if(readChar>0x30 && readChar<0x39){
//									threshold = readChar-0x30;
//									while(UART2_Read_Flag){
//										u8temp = UART_Read_Char();
//										if(readChar>0x30 && readChar<0x39){
//											u8temp-=0x30;
//											threshold = threshold*10 + u8temp;
//										}
//										else{
//											threshold = 0;
//											break;
//										}
//									}
//								}
//								else{
//									threshold = 0;
//									break;
//								}
//							}
//							break;

