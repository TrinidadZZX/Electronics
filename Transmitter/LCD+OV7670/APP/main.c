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
extern u8 ov_sta;	//��exit.c���涨��		0����ʾFIFO�洢�������Ѿ�����ȡ��	1��Ŀǰ������FIFOд����	   2�����Կ�ʼ������


u8 readyToSend=0;
extern u8 UART2_Read_Flag;												//��Ϊ1����Զ�ȡ����
u16 readHundrend(char *s);
void SendHundred(int num);

/*��������
	
*/
u16 r_threshold = 25 << 11;
u16 g_threshold = 55 << 5;
u16 b_threshold = 25;

void assert_failed(uint8_t* file, uint32_t line)
{
    printf("Wrong parameters value: file %s on line %d\r\n", file, line);
    while(1);
}


//������ʾ
//�趨��Ҫ���ֵ���ɫ���Լ���ֵ	R:0, G:1, B:2

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
				LCD_SetWindows(0,0,320-xOfOV7670-1,320-yOfOV7670-1);				//LCD xOf ��߲�Ҫ������
        //LCD_SetWindows(0,0,320-1,240-1);				//LCD
				#endif
			
        for(j=0; j<76800; j++)
        {
            GPIOB->CRL=0X88888888;
            OV7670_RCK=0;
            color=OV7670_DATA;		//?����y?Y
            OV7670_RCK=1;
            color<<=8;
            OV7670_RCK=0;
            color|=OV7670_DATA;		//?����y?Y
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
				
				if(readyToSend){		//����"x=```Sy=```"
					printf("x=");
					SendHundred(x);
					printf("Sy=");
					SendHundred(y);
					printf("\r\n");//���뻻��
				}
				
//				if(readyToSend){		//����"x=```\0y=```"
//					printf("x=");
//					SendHundred(x);
//					printf("\0y=");
//					SendHundred(y);
//					printf("\r\n");//���뻻��
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


//usart.c�궨��
extern u8 USART2_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
extern u16 USART2_RX_STA;       //����״̬���	 

int main(void)
{
	
	#ifdef USE_BLUETOOTH	
 	u16 t;  
	u16 len;	
	#endif
//		u8 numBit=0;		//ÿ�η�����λ����	1��0��
//		u8 u8temp;
//		u8 mode=0;			//mode=1 RGBѡ��ģʽ		mode=2��ֵ�趨ģʽ
//		u8 RGB=0;				//������
		//u16 threshold=10;		//��ֵ 32��64��32
    //u16 i=0;
		//u8 readChar;

	SystemInit();       //��ʼ��RCC ����ϵͳ��ƵΪ72MHZ
    delay_init(72);     //��ʱ��ʼ��
		
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	
		//Һ��
    LCD_Init();          //Һ������ʼ��
	
		//����ͷ
    OV7670_Init();    //����ͷ��ʼ��
    delay_ms(500);
    EXTI15_Init();						                    //ʹ�ܶ�ʱ������
    OV7670_Window_Set(10,174,240,320);	//���ô���
    OV7670_CS=0;

    delay_ms(500);

		//��������
	#ifdef USE_BLUETOOTH	
 	USART2_INIT(38400);	 //���ڳ�ʼ��Ϊ38400
	#endif

	//delay_ms(5000);
  while(1)
  {
		#ifdef USE_BLUETOOTH
		camera_refresh();
		
		
		
//�������Σ���ֵ��
//		if(USART2_RX_STA&0x8000)		//���ݽ������
//		{					   
//				len=USART2_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
//				
//				//��������
//				if(USART2_RX_BUF[0] == 'r' &&USART2_RX_BUF[6] == 'g' && USART2_RX_BUF[12] == 'b'){//x������
//					r_threshold = readHundrend(USART2_RX_BUF+2) << 11;
//					g_threshold = readHundrend(USART2_RX_BUF+8) << 5;
//					b_threshold = readHundrend(USART2_RX_BUF+14);
//				}
//				
//				USART2_RX_STA=0;				//Reset���ջ�����
//		}
		
		
		
//		double temp=100.123;
//		if(readyToSend){
//			printf("%f",temp);
//			printf("\r\n");//���뻻��
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


