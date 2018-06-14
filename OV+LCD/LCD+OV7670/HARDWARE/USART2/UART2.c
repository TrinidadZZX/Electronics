#include <stdio.h>
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "misc.h"
#include "UART2.h"

static unsigned char TxBuffer[TxBuffer_Length];					//∑¢ÀÕª∫≥Â«¯
static unsigned char TxCounter=0;							//“—æ≠∑¢ÀÕ ˝æ›÷∏’Î
static unsigned char TxCount=0; 							//“—æ≠ ’¬º ˝æ›

static unsigned char RxBuffer[RxBuffer_Length];					//Ω” ‹ª∫≥Â«¯
static unsigned char RxCounter=0;							//“—æ≠Ω” ’ ˝æ›÷∏’Î
static unsigned char RxCount=0; 							//“—æ≠∂¡»°µƒŒª÷√

u8 UART2_Read_Flag = 0;												//»ÙŒ™1‘Úø…“‘∂¡»° ˝æ›

void Initial_UART2(unsigned long baudrate)
{
	GPIO_InitTypeDef GPIO_InitStructure;//I/O?˙3?Í??Ø?·11Ï?
    NVIC_InitTypeDef NVIC_InitStructure;//?D??3?Í??Ø?·11Ï?
    USART_InitTypeDef USART_InitStructure;//'??˙3?Í??Ø?·11Ï?  
    
    /* Enable the USART2 Pins Software Remapping */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2|RCC_APB2Periph_AFIO, ENABLE);
    
    /* Configure USART2 Rx (PA.03) as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /* Configure USART2 Tx (PA.02) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /* Enable the USART2 Interrupt */
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 		//1Œª«¿’º”≈œ»º∂ 3Œª∏±”≈œ»º∂
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);    
    
    USART_InitStructure.USART_BaudRate = baudrate;                
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;     
    USART_InitStructure.USART_Parity = USART_Parity_No;        
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;      
    
    USART_Init(USART2, &USART_InitStructure);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
		USART_ITConfig(USART2, USART_IT_TXE, DISABLE);  
		USART_ITConfig(USART2, USART_IT_TC, DISABLE);
    /* Enable USART2 */
    USART_Cmd(USART2, ENABLE);
	
	
	
// 	GPIO_InitTypeDef GPIO_InitStructure;
//	USART_InitTypeDef USART_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure; 
//	
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 | RCC_APB2Periph_GPIOA, ENABLE);
//	
//	USART_DeInit(USART2);
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);    
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	 
//	USART_InitStructure.USART_BaudRate = baudrate;
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;												// π”√¡À–£—ÈŒª
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;														//1Õ£÷πŒª
//	USART_InitStructure.USART_Parity = USART_Parity_No ;														//≈ºƒ£ Ω
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//	USART_Init(USART2, &USART_InitStructure); 
//	
//	
//	USART_ClearFlag(USART2,USART_FLAG_TC);			//∑¢ÀÕÕÍ≥…±Í÷æŒª		TC£∫«πÃ≈	TXE£∫µØÃ≈
//	
//	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 		//1Œª«¿’º”≈œ»º∂ 3Œª∏±”≈œ»º∂
//	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//	
//	USART_ITConfig(USART2, USART_IT_TXE, DISABLE);  
//	//USART_ITConfig(USART2, USART_IT_TC, DISABLE);
//	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
//	USART_Cmd(USART2, ENABLE);
}
void USART2_IRQHandler(void)
{
  if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET)					//–¥¥Ê¥¢∆˜
  {   
		while(TxCounter < TxCount){
			USART_SendData(USART2, TxBuffer[TxCounter++]); 
			while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET);
		}
		TxCounter = 0;
		TxCount = 0;
    USART_ClearITPendingBit(USART2, USART_IT_TXE);
		USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
  }
	else if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)		//∂¡¥Ê¥¢∆˜
  {
		if(TxCount < RxBuffer_Length){
			UART2_Read_Flag = 1;
			RxBuffer[RxCounter++] = USART_ReceiveData(USART2);
		}
		else
			USART_ReceiveData(USART2);																//»Áπ˚“—¬˙‘Ú÷±Ω”≈◊∆˙
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
  }
	
	USART_ClearITPendingBit(USART2,USART_IT_ORE);									//“Á≥ˆ¥ÌŒÛ±Í÷æŒª
}


void UART2_Put_Char(unsigned char DataToSend)
{
	if(TxCount < TxBuffer_Length)
		TxBuffer[TxCount++] = DataToSend;  
  USART_ITConfig(USART2, USART_IT_TXE, ENABLE);  
}

void UART2_Put_String(unsigned char *Str)
{
	while(*Str)
	{
		if(*Str=='\r')UART2_Put_Char(0x0d);
			else if(*Str=='\n')UART2_Put_Char(0x0a);
				else UART2_Put_Char(*Str);
		Str++;
	}
}

u8 UART_Read_Char(void)
{
	u8 feedBack;
	if(RxCount < RxCounter)
		feedBack = RxBuffer[RxCount++];
	else
		feedBack = 0;
	if(RxCount == RxCounter){
		RxCount = 0;
		RxCounter = 0;
		UART2_Read_Flag = 0;
	}
	return feedBack;
}

