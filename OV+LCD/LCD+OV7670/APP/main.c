#include <stdio.h>
#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "gui.h"
#include "test.h"
#include "exti.h"
#include "ov7670.h"
#include "UART2.h"

extern u8 ov_sta;	//在exit.c里面定义		0：表示FIFO存储的数据已经被读取了	1：目前正在往FIFO写数据	   2：可以开始读数据

extern u8 UART2_Read_Flag;												//若为1则可以读取数据

void assert_failed(uint8_t* file, uint32_t line)
{
    printf("Wrong parameters value: file %s on line %d\r\n", file, line);
    while(1);
}


//更新显示
void camera_refresh(void)
{
    u16 color=0;

    u32 j=0;

    if(ov_sta==2)
    {
        OV7670_CS=0;
        OV7670_RRST=0;
        OV7670_RCK=0;
        OV7670_RCK=1;
        OV7670_RCK=0;
        OV7670_RRST=1;
        OV7670_RCK=1;

        LCD_SetWindows(0,0,320-1,240-1);
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
            LCD_DrawPoint_16Bit(color);
        }
        
        LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);

        OV7670_CS=1;
        OV7670_RCK=0;
        OV7670_RCK=1;
        EXTI->PR=1<<15;
        ov_sta=0;
    }
}


int main(void)
{
    u16 i=0;
		u8 readChar;
    SystemInit();       //初始化RCC 设置系统主频为72MHZ
    delay_init(72);     //延时初始化
	
		//蓝牙串口
		Initial_UART2(38400);
	
		//液晶
    LCD_Init();          //液晶屏初始化
	
		//摄像头
    OV7670_Init();    //摄像头初始化
    delay_ms(1500);
    EXTI15_Init();						                    //使能定时器捕获
    OV7670_Window_Set(10,174,240,320);	//设置窗口
    OV7670_CS=0;

    delay_ms(1000);

    while(1)
    {
        i++;
				UART2_Put_String("Another Try\r\n");
        camera_refresh();	//更新显示
				if(UART2_Read_Flag){
					readChar = UART_Read_Char();
					UART2_Put_String("OK, I get it!\r\nIt's ");
					UART2_Put_Char(readChar);
					UART2_Put_String("\r\n");
				}
    }
}
