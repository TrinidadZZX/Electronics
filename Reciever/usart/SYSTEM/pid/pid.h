#ifndef __PID_H
#define __PID_H

#include "sys.h"
#include "stm32f10x_tim.h"
#include "Motor.h"
void TIM2_PID_Init(void);
void NewExpt(void);
void TIM2_IRQHandler(void);

#endif
