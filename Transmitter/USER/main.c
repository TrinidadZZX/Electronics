#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "string.h"
#include "ov7670.h"
#include "exti.h"
#include "lcd.h"
#include "gui.h"
#include "tim2.h"
#include "usart2.h"

//#define USE_LCD_PICTURE
/************************************************
 ALIENTEKս��STM32������ʵ��35
 ����ͷOV7670 ʵ��
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/


//const u8*LMODE_TBL[5]={"Auto","Sunny","Cloudy","Office","Home"};							//5�ֹ���ģʽ
//const u8*EFFECTS_TBL[7]={"Normal","Negative","B&W","Redish","Greenish","Bluish","Antique"};	//7����Ч
extern u8 ov_sta;	//��exit.c�� �涨��
//����LCD��ʾ

//����

u16 readHundrend(char *s);
void SendHundred(int num);


//֡��
u8 pic_count=0;
u8 pic_fre=0;				//ͼ��Ƶ��

//��ֵ
u16 r_threshold = 25 << 11;
u16 g_threshold = 55 << 5;
u16 b_threshold = 25;

void camera_refresh(void);

int main(void)
{
    u8 lightmode=0,saturation=2,brightness=2,contrast=2;
    u8 effect=0;

    delay_init();	    	 //��ʱ������ʼ��

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
																																								//TIM2 1,1			EXTI8	0,0		
    EXTI8_Init();
    //֡��������
    TIM2_Config();
    //Һ��

    OV7670_Init();
    LCD_Init();          //Һ������ʼ��
		//����
		USART2_INIT(9600);
    //����
    OV7670_Light_Mode(lightmode);
    OV7670_Color_Saturation(saturation);
    OV7670_Brightness(brightness);
    OV7670_Contrast(contrast);
    OV7670_Special_Effects(effect);

    OV7670_Window_Set(12,176,240,320);	//���ô���
    OV7670_CS=0;
		delay_ms(100);

    while(1)
    {
        camera_refresh();//������ʾ
    }

}






void camera_refresh(void)
{
    u16 color;
    u8 temp;
    u16 x_count, y_count;				//x �� y�������

    //ͳ��ֵ
    int collector=0;
    int x_sum=0, y_sum=0;
    short x=0, y=0;


    if(ov_sta)//��֡�жϸ��£�
    {
        pic_count++;

        OV7670_RRST=0;				//��ʼ��λ��ָ��
        OV7670_RCK_L;
        OV7670_RCK_H;
        OV7670_RCK_L;
        OV7670_RRST=1;				//��λ��ָ�����
        OV7670_RCK_H;

#ifdef USE_LCD_PICTURE
        //LCD_SetWindows(0,0,320-xRes-1,320-yRes-1);				//LCD xOf ��߲�Ҫ������
        LCD_SetWindows(0,0,240-1,240-1);				//LCD
#endif
        for(y_count=0; y_count<240; y_count++) {
            for(x_count=0; x_count<320; x_count++) {
                if(x_count<240) {
                    OV7670_RCK_L;
                    color=GPIOE->IDR&0XFF00;	//������
                    OV7670_RCK_H;
                    //color<<=8;
                    OV7670_RCK_L;
                    color|=(GPIOE->IDR&0XFF00)>>8;	//������
                    OV7670_RCK_H;
#ifdef USE_LCD_PICTURE
                    //if((j%320)>=xRes && (j/320)>=yRes)
                    LCD_DrawPoint_16Bit(color);					//LCD
                    //LCD_DrawPoint_16Bit(0x0F0F);					//LCD
#endif

                    //�ҵ�
                    //if((color&0x0001F)>b_threshold && (color&0x07D0)>g_threshold && (color&0xF800) >r_threshold)
                    if((color&0x0001F)>b_threshold	)
                    {
                        x_sum += x_count;
                        y_sum += y_count;
                        collector++;
                    }
                }
                else {
                    OV7670_RCK_L;
                    color=GPIOA->IDR&0XFF00;	//������
                    OV7670_RCK_H;
                    OV7670_RCK_L;
                    color=GPIOA->IDR&0XFF00;	//������
                    OV7670_RCK_H;
                }
            }
        }

        ov_sta=0;					//����֡�жϱ��

        if(collector) {				//ͳ��ֵ��Ϊ0��ȡƽ��ֵ
            x = x_sum/collector;
            y = y_sum/collector;
        }
        else {
            x = 0;
            y = 0;
        }
#ifdef USE_LCD_PICTURE
        LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);			//LCD
#endif
        //��ʾλ��
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


        //��ʾ֡��
        temp=(pic_fre%10);
        LCD_ShowNum(60,300,temp,3,12);
        temp=(pic_fre%100)/10;
        LCD_ShowNum(60,300,temp,2,12);
        temp=(pic_fre%1000)/100;
        LCD_ShowNum(60,300,temp,1,12);

        //��������
								printf("x=");
								SendHundred(x);
								printf("y=");
								SendHundred(y);
								printf("\r\n");//���뻻��
    }
}







u16 readHundrend(char *s) {
    u16 temp=0;
    temp=((*s -0x30) * 100)+((*(s+1)-0x30) * 10)+(*(s+2)-0x30);
    return temp;
}
void SendHundred(int num) {
    while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
    USART_SendData(USART2, (num%1000)/100+0x30);

    while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
    USART_SendData(USART2, (num%100)/10+0x30);

    while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
    USART_SendData(USART2, (num%10)+0x30);
}
