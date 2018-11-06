// 스탑워치 만들기
// PD0 버튼 : 카운팅 시작
// PD1 버튼 : 카운팅 종료
// PD2 버튼 : 리셋
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

volatile int mstime = 0;
volatile int sec =0, min =0, hour =0;
void SysTick_Handler(void) {
	mstime++;
	if (mstime >=1000){
		sec++;
		mstime=0;
		if (sec>=60){
			min++;
			sec=0;
			if(min>=60){
				min=0;
			}
		}
	}
}

void delay(int ms) {
	volatile int future = ms + mstime;
	while (future > mstime);
}


unsigned int fnd[10] = { 0b0011111100000000, 0b0000011000000000,
		0b0101101100000000, 0b0100111100000000, 0b0110011000000000,
		0b0110110100000000, 0b0111110100000000, 0b0010011100000000,
		0b0111111100000000, 0b0110011100000000 };

unsigned int fnd_sel[4] = { 0b1000000000000000, 0b0100000000000000,
		0b0010000000000000, 0b0001000000000000 };

void display_fnd(int num, int num_2) {

	int num1 = 0, num2 = 0, num3 = 0, num4 = 0;

	num1 = num % 10;
	num2 = (num / 10) % 10;
	num3 = num_2%10;
	num4 = (num_2/10)%10;
	/*
	num3 = (num / 100) % 10;
	num4 = (num / 1000) % 10;
	 */
	GPIO_ResetBits(GPIOB, fnd_sel[3]);
	GPIO_SetBits(GPIOD, fnd[num1]);
	GPIO_SetBits(GPIOB, fnd_sel[3]);
	GPIO_ResetBits(GPIOD, fnd[num1]);

	GPIO_ResetBits(GPIOB, fnd_sel[2]);
	GPIO_SetBits(GPIOD, fnd[num2]);
	GPIO_SetBits(GPIOB, fnd_sel[2]);
	GPIO_ResetBits(GPIOD, fnd[num2]);

	GPIO_ResetBits(GPIOB, fnd_sel[1]);
	GPIO_SetBits(GPIOD, fnd[num3]);
	GPIO_SetBits(GPIOB, fnd_sel[1]);
	GPIO_ResetBits(GPIOD, fnd[num3]);

	GPIO_ResetBits(GPIOB, fnd_sel[0]);
	GPIO_SetBits(GPIOD, fnd[num4]);
	GPIO_SetBits(GPIOB, fnd_sel[0]);
	GPIO_ResetBits(GPIOD, fnd[num4]);
}
void init() {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOD, ENABLE); // GPIO 클럭 인가

	// GPIOB set
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7
			| GPIO_Pin_8 |
			GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; // push pull
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//GPIOD_OUT set
	GPIO_InitStructure.GPIO_Pin = 0b1111111100000000;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	//GPIOD_IN set
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2
			| GPIO_Pin_3);
	// equal to 0b00001111
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; // push pull
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_Init(GPIOD, &GPIO_InitStructure);

}
int main(void) {
	init();
	int fnd_min=0,fnd_sec=0;
	int flag = 0, flag2=0, flag3=0;
	SysTick_Config(53676000 / 1000);

	int mode =0;
	for (;;) {
		if(mode ==0)
		display_fnd(fnd_sec,fnd_min); //FND display(num)
		else if(mode ==1)
		display_fnd(sec,min);
		// GPIOD1 입력시 GPIOB bit set, num++;
		 if((GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_0)==0) && flag ==0){
		 mstime =0; mode =1; sec =0; min=0;
		 flag=1;
		 delay(10);
		 }
		 else if((GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_0)==1) && flag==1){
		 flag=0;
		 }

		 // GPIOD2 이력시 GPIOB bit set, num--;
		 if((GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1)==0) && flag2 ==0){
		 fnd_min = min; fnd_sec = sec;
		 mode =0;
		 flag2=1;
		 delay(10);
		 }
		 else if ((GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1)==1)&& flag2 ==1){
		 flag2=0;
		 }
		 // GPIOD3
		 if ((GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2)==0)&& flag3 ==0){
		 fnd_min =0; fnd_sec=0; mstime =0; sec=0;min=0;
		 flag3=1;
		 delay(10);
		 }
		 else if((GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2)==1)&&flag ==1){
		 flag3=0;
		 }
	}
}
