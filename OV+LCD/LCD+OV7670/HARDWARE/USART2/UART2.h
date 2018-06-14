#ifndef __UART2_H
#define __UART2_H

#define TxBuffer_Length 256
#define RxBuffer_Length 256


void Initial_UART2(unsigned long baudrate);
void UART2_Put_Char(unsigned char DataToSend);
void UART2_Put_String(unsigned char *Str);

u8 UART_Read_Char(void);


#endif

//------------------End of File----------------------------

