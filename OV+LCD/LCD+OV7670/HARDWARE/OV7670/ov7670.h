#ifndef _OV7670_H
#define _OV7670_H
#include "sys.h"
#include "sccb.h"
//////////////////////////////////////////////////////////////////////////////////
//������ο�������guanfu_wang���롣
//ALIENTEK MiniSTM32������
//OV7670 ��������
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/10/31
//�汾��V1.0
//////////////////////////////////////////////////////////////////////////////////
#define OV7670_WRST		PAout(8)		//дָ�븴λ
#define OV7670_RCK		    PAout(9)		//������ʱ��
#define OV7670_RRST		PAout(4)  		//��ָ�븴λ
#define OV7670_CS		    PAout(11)  	//Ƭѡ�ź�(OE)
#define OV7670_WREN		PAout(12)		//д��FIFOʹ��
#define OV7670_VSYNC  	PAin(15)		//ͬ���źż��IO

#define OV7670_DATA   GPIOF->IDR&0x00FF  					//��������˿�
/////////////////////////////////////////

u8   OV7670_Init(void);					//��ʼ��
void OV7670_Light_Mode(u8 mode);				//����ģʽ
void OV7670_Color_Saturation(u8 sat);				//ɫ�ʱ��Ͷ�
void OV7670_Brightness(u8 bright);					//����
void OV7670_Contrast(u8 contrast);					//�Աȶ�
void OV7670_Special_Effects(u8 eft);				//�˾�
void OV7670_Window_Set(u16 sx,u16 sy,u16 width,u16 height);			//��Ĭ��

#endif
