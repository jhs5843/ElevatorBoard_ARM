/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include "stm32f4xx.h"
/* Include my libraries here */
//#include "defines.h"
#include "tm_stm32f4_usart.h"

char inputbuf[255];
int bufsize = 0;

char* ReadUartStr()
{
	int c;
	do {
		c = TM_USART_Getc(USART2);
		if (c) {
			if (c=='\n')
			{
				inputbuf[bufsize] = 0;
				bufsize=0;
				return inputbuf;
			}
			inputbuf[bufsize++] = c;
		}
	} while (c);
	return 0;
}

int main(void) {
	uint8_t c;

	TM_USART_Init(USART2, TM_USART_PinsPack_1, 9600);
	TM_USART_Puts(USART2, "Hello world\n");

	int input;
	while (1) {
		char* str = ReadUartStr();
		if(str)
		{
			sscanf(str, "%d", &input);
			if (input == 1)
			{
				TM_USART_Puts(USART2, "hi");
			}
			if (input == 2)
			{
				TM_USART_Puts(USART2, "low");
			}

			//TM_USART_Puts(USART2, str);
		}
	}
}



