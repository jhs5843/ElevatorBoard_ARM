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
#include "stm32f4xx_adc.h"

#define FND0 0b0011111100000000
#define FND1 0b0000011000000000
#define FND2 0b0101101100000000
#define FND3 0b0100111100000000
#define FND4 0b0110011000000000
#define FND5 0b0110110100000000
#define FND6 0b0111110100000000
#define FND7 0b0010011100000000
#define FND8 0b0111111100000000
#define FND9 0b0110111100000000

#define LED1 0b0000000000000001
#define LED2 0b0000000000000010
#define LED3 0b0000000000000100
#define LED4 0b0000000000001000
#define LED5 0b0000000000010000

#define KEY_Down 0b10001000 // Down
#define KEY_Up 0b10000100 // Up
#define KEY_OUT_5 0b10000010 // 외부 5층
#define KEY_OUT_4 0b10000001 // 외부 4층
#define KEY_OUT_3 0b01001000 // 외부 3층
#define KEY_OUT_2 0b01000100 // 외부 2층
#define KEY_OUT_1 0b01000010 // 외부 1층
#define KEY_TitleSet 0b01000001 // Title Set
#define KEY_Emergency 0b00101000 // 비상
#define KEY_Open 0b00100100 // 열림
#define KEY_Close 0b00100010 // 닫힘
#define KEY_IN_5 0b00100001 // 내부 5층
#define KEY_IN_4 0b00011000 // 내부 4층
#define KEY_IN_3 0b00010100 // 내부 3층
#define KEY_IN_2 0b00010010 // 내부 2층
#define KEY_IN_1 0b00010001 // 내부 1층

#define buzzer_pin GPIO_Pin_1
#define buzzer_port GPIOB

#define photo_port GPIOD
#define photo_pin GPIO_Pin_2

volatile int mstime = 0;
void SysTick_Handler(void) {
	mstime++;
}
void delay(int ms) {
	volatile int future = ms + mstime;
	while (future > mstime)
		;
}
void buzzer() {
	GPIO_SetBits(GPIOB, GPIO_Pin_1);
	delay(500);
	GPIO_ResetBits(GPIOB, GPIO_Pin_1);
	delay(500);
}
void FND_display(int num) {

	GPIO_SetBits(GPIOC,
			GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12
					| GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
	switch (num) {
	case 0:
		GPIO_ResetBits(GPIOC, (0b1111111100000000 & FND0));
		break;
	case 1:
		GPIO_ResetBits(GPIOC, (0b1111111100000000 & FND1));
		break;
	case 2:
		GPIO_ResetBits(GPIOC, (0b1111111100000000 & FND2));
		break;
	case 3:
		GPIO_ResetBits(GPIOC, (0b1111111100000000 & FND3));
		break;
	case 4:
		GPIO_ResetBits(GPIOC, 0b1111111100000000 & FND4);
		break;
	case 5:
		GPIO_ResetBits(GPIOC, 0b1111111100000000 & FND5);
		break;
	case 6:
		GPIO_ResetBits(GPIOC, 0b1111111100000000 & FND6);
		break;
	case 7:
		GPIO_ResetBits(GPIOC, 0b1111111100000000 & FND7);
		break;
	case 8:
		GPIO_ResetBits(GPIOC, 0b1111111100000000 & FND8);
		break;
	case 9:
		GPIO_ResetBits(GPIOC, 0b1111111100000000 & FND9);
		break;
	}
}
void LED(int num) {
	GPIO_SetBits(GPIOC,
	GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4);
	switch (num) {
	case 1:
		GPIO_ResetBits(GPIOC, 0b0000000000011111 & LED1);
		break;
	case 2:
		GPIO_ResetBits(GPIOC, 0b0000000000011111 & LED2);
		break;
	case 3:
		GPIO_ResetBits(GPIOC, 0b0000000000011111 & LED3);
		break;
	case 4:
		GPIO_ResetBits(GPIOC, 0b0000000000011111 & LED4);
		break;
	case 5:
		GPIO_ResetBits(GPIOC, 0b0000000000011111 & LED5);
		break;
	}
}
void getKey_init() {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(
			RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC
					| RCC_AHB1Periph_GPIOD, ENABLE); // GPIO 클럭 인가
	// Key_Row Output set
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10
			| GPIO_Pin_11);
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// KEY_Col Input set
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6
			| GPIO_Pin_7);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; // push pull
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
char getKey() {
	char key = 0xff;
	GPIO_SetBits(GPIOA, GPIO_Pin_8);
	delay(5);
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 1)
		key = '0';
	else if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 1)
		key = '1';
	else if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == 1)
		key = '2';
	else if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7) == 1)
		key = '3';
	GPIO_ResetBits(GPIOA, GPIO_Pin_8);
	GPIO_SetBits(GPIOA, GPIO_Pin_9);
	delay(5);
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 1)
		key = '4';
	else if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 1)
		key = '5';
	else if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == 1)
		key = '6';
	else if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7) == 1)
		key = '7';
	GPIO_ResetBits(GPIOA, GPIO_Pin_9);
	GPIO_SetBits(GPIOA, GPIO_Pin_10);
	delay(5);
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 1)
		key = '8';
	else if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 1)
		key = '9';
	else if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == 1)
		key = 'A';
	else if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7) == 1)
		key = 'B';
	GPIO_ResetBits(GPIOA, GPIO_Pin_10);
	GPIO_SetBits(GPIOA, GPIO_Pin_11);
	delay(5);
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 1)
		key = 'C';
	else if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 1)
		key = 'D';
	else if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == 1)
		key = 'E';
	else if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7) == 1)
		key = 'F';
	GPIO_ResetBits(GPIOA, GPIO_Pin_11);
	delay(5);
	return key;
}
void ADC_init() {

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	ADC_InitTypeDef ADC_InitStruct;
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStruct.ADC_ExternalTrigConv = DISABLE;
	ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStruct.ADC_NbrOfConversion = 1;
	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;
	ADC_Init(ADC1, &ADC_InitStruct);

	ADC_Cmd(ADC1, ENABLE);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_84Cycles);

	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(
			RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC
					| RCC_AHB1Periph_GPIOD, ENABLE); // GPIO 클럭 인가

	GPIO_InitStructure.GPIO_Pin =
	// LED Output set
			(GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4
					|
					// FND Output set
					GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11
					| GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; // push pull
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

}
int ADC_Read() {
	ADC_SoftwareStartConv(ADC1);
	while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC))
		;
	return ADC_GetConversionValue(ADC1);
}
void init() {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(
			RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC
					| RCC_AHB1Periph_GPIOD, ENABLE); // GPIO 클럭 인가

	GPIO_InitStructure.GPIO_Pin =
	// LED Output set
			(GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4
					|
					// FND Output set
					GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11
					| GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; // push pull
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	// Buzzer Output set
	GPIO_InitStructure.GPIO_Pin = buzzer_pin;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// LED default set
	GPIO_SetBits(GPIOC,
	GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4);
	// FND default set
	GPIO_SetBits(GPIOC,
			GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12
					| GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);

	// PD2 Photo Interrupt set
	GPIO_InitStructure.GPIO_Pin = photo_pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; // push pull
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void matrix_init() {
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_Initstruct;
	GPIO_Initstruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10
			| GPIO_Pin_11;
	GPIO_Initstruct.GPIO_OType = GPIO_OType_PP;
	GPIO_Initstruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Initstruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Initstruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_Initstruct);
	GPIO_Initstruct.GPIO_Pin =
	GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Initstruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Initstruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &GPIO_Initstruct);
}

void simple_delay(int count) {
	int i;
	for (i = 0; i < count; i++)
		;
}

int GetKey(int prevKey) {
	int i;
	for (i = 0; i < 4; i++) {
		GPIO_ResetBits(GPIOA, 0b1111 << 8);
		GPIO_SetBits(GPIOA, 1 << i << 8);
		simple_delay(40);
		int key = (GPIO_ReadInputData(GPIOA) >> 4) & 0b1111;
		if (key != 0) {

			int retKey= 1 << 4 << i | key;
			if(retKey != prevKey)
				return retKey;
		}
	}
	return 0;
}

int main(void) {
	init();
	SysTick_Config(53676000 / 1000);
	matrix_init();
	int j = 0;
	for (;;) {
		int key = GetKey(key);
		GPIO_ResetBits(GPIOB, GPIO_Pin_1);
		if (key == KEY_IN_5){
			FND_display(1);delay(2);}
		if (key == KEY_OUT_5){
			FND_display(2);delay(2);}
		if (key == KEY_IN_4){
			FND_display(3);delay(2);}
		if (key == KEY_OUT_4){
			FND_display(4);delay(2);}
		if (key == KEY_IN_3){
			FND_display(5);delay(2);}
		if (key == KEY_OUT_3){
			FND_display(6);delay(2);}
		if (key == KEY_IN_2){
			FND_display(7);delay(2);}
		if (key == KEY_OUT_2){
			FND_display(8);delay(2);}
		if (key == KEY_IN_1){
			FND_display(9);delay(2);}
		if (key == KEY_OUT_1){
			FND_display(0);delay(2);}
		if (key==KEY_Open){
			GPIO_SetBits(GPIOB, GPIO_Pin_1);
			simple_delay(100000);
		}
		if(key == KEY_Close){
			GPIO_SetBits(GPIOB, GPIO_Pin_1);
			simple_delay(100000);
		}
		/* ADC Read

		 j = ADC_Read();
		 //LED High
		 GPIO_SetBits(GPIOC,
		 GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4);
		 if ((j >= 0) & (j < 800))
		 GPIO_ResetBits(GPIOC, 0b0000000000011111 & LED1);
		 if ((j >= 800) & (j < 1600))
		 GPIO_ResetBits(GPIOC, 0b0000000000011111 & (LED1 | LED2));
		 if ((j >= 1600) & (j < 2400))
		 GPIO_ResetBits(GPIOC, 0b0000000000011111 & (LED1 | LED2 | LED3));
		 if ((j >= 2400) & (j < 3200))
		 GPIO_ResetBits(GPIOC,
		 0b0000000000011111 & (LED1 | LED2 | LED3 | LED4));
		 if (j >= 3200)
		 GPIO_ResetBits(GPIOC,
		 0b0000000000011111 & (LED1 | LED2 | LED3 | LED4 | LED5));
		 //0~4095
		 if ((j >= 0) & (j < 400))
		 FND_display(0);
		 if ((j >= 400) & (j < 800))
		 FND_display(1);
		 if ((j >= 800) & (j < 1200))
		 FND_display(2);
		 if ((j >= 1200) & (j < 1600))
		 FND_display(3);
		 if ((j >= 1600) & (j < 2000))
		 FND_display(4);
		 if ((j >= 2000) & (j < 2400))
		 FND_display(5);
		 if ((j >= 2400) & (j < 2800))
		 FND_display(6);
		 if ((j >= 2800) & (j < 3200))
		 FND_display(7);
		 if ((j >= 3200) & (j < 3600))
		 FND_display(8);
		 if (j >= 3600)
		 FND_display(9);
		 */
		/* photo interrupt
		 if (GPIO_ReadInputDataBit(photo_port, photo_pin) == 1) {
		 GPIO_SetBits(buzzer_port, buzzer_pin);
		 }
		 else {
		 GPIO_ResetBits(buzzer_port, buzzer_pin);
		 }
		 GPIO_ResetBits(GPIOB, GPIO_Pin_1);
		 GPIO_SetBits(GPIOB,GPIO_Pin_1*GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2));
		 */
	}
}
