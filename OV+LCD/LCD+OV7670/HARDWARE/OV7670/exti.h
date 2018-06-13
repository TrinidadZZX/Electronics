#ifndef __EXTI_H
#define __EXIT_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK Mini STM32开发板
//外部中断 驱动代码
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/3/27
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//********************************************************************************
//V1.1 20140327
//增加EXTI15_Init函数，并修改EXTI15_10_IRQHandler函数，以支持OV7670的使用
//////////////////////////////////////////////////////////////////////////////////


void exti_Init(void);	//外部中断初始化
void EXTI15_Init(void);
#endif

























