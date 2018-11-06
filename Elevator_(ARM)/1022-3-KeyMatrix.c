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

#define KM_INT_1 0b10001000
#define KM_INT_2 0b10000100
#define KM_INT_3 0b10000010
#define KM_INT_4 0b10001001
#define KM_INT_5 0b01001000

void matrix_init()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_Initstruct;
	GPIO_Initstruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_Initstruct.GPIO_OType = GPIO_OType_PP;
	GPIO_Initstruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Initstruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Initstruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_Initstruct);
	GPIO_Initstruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Initstruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Initstruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &GPIO_Initstruct);
}

void simple_delay(int count)
{
	int i;
	for(i=0; i<count; i++);
}

int GetKey()
{
	int i;
	for(i=0; i<4; i++)
	{
		GPIO_ResetBits(GPIOA, 0b1111 << 8);
		GPIO_SetBits(GPIOA, 1 << i << 8);
		simple_delay(5);
		int key = (GPIO_ReadInputData(GPIOA) >> 4) & 0b1111;
		if (key != 0)
		{
			return 1 << 4 << i | key;
		}
	}
	return 0;
}

int main(void)
{
	matrix_init();
	for(;;)
	{
		int key = GetKey();

		if (key == KM_INT_1)
		{
			// 내부 1층이 입력되었습니다.
		}
	}
}



