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

int main(void) {
	uint8_t c;

	TM_USART_Init(USART2, TM_USART_PinsPack_1, 9600);
	
	TM_USART_Puts(USART2, "Hello world\n");
	
	while (1) {

		/* Get character from internal buffer */
		c = TM_USART_Getc(USART2);
		if (c) {
			/* If anything received, put it back to terminal */
			TM_USART_Putc(USART2, c);
		}
	}
}
