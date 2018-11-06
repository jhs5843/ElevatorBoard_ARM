Arm trainning kit

1. 키와 몸무게를 이력받아 BMI를 계산하는 프로그램을 작성하시오

캐릭터 LCD에 메뉴를 표시한다.
상태1 
[1. INPUT WEIGHT]

상태2
[2. INPUT HEINGT]

상태3
[3. CALC BMI]

0번 버튼 : 상
1번 버튼 : 하
2번 버튼 : 선택
3번 버튼 : 취소
------------------------------------------------------------

1. 상/하 버튼을 누르면 상태가 바뀜. 상태1 > 상태2 > 상태3 

2. 1번 상태에서 버튼을 누르면 
 상태1
 [50] +-
 상/하 버튼을 입력하여 몸무게 증감. 
 [선택]을 누르면 몸무게가 셋팅 되고 1번 상태로 돌아감
 [취소]을 누르면 취소되고 1로 돌아감

 상태2
  상/하 버튼을 입력하여 키 증감. 
 [선택]을 누르면 키가 셋팅 되고 1번 상태로 돌아감
 [취소]을 누르면 취소되고 1로 돌아감

 상태3
 Your BMI is 
 BMI Calc = w/(t^2) (단위, t = m)
 >= 30 비만, 25>= 과체중
 < 18.5 저체중
 Over weight
 Nomal weight
 Under weignt
 [취소]을 누르면 1로 돌아감 // 해당 기능 구현

 #include "stm32f4xx.h"
#include "lcd.h"
#include "stdio.h"


int msTime = 0;
void SysTick_Handler() {
	msTime++;
}
void delay(int ms) {
	int future = ms + msTime;
	while (future > msTime)
		;
}
void init() {
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2
			| GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
}

int prev_pin_0 = 1;
int pin_0 = 1;
int prev_pin_1 = 1;
int pin_1 = 1;
int prev_pin_2 = 1;
int pin_2 = 1;
int prev_pin_3 = 1;
int pin_3 = 1;
char buf[255];

void getBtn() {
	prev_pin_0 = pin_0;
	pin_0 = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_0);
	prev_pin_1 = pin_1;
	pin_1 = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1);
	prev_pin_2 = pin_2;
	pin_2 = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2);
	prev_pin_3 = pin_3;
	pin_3 = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_3);
}

int InputWeightMenu(int CurrentWeight) {
	int iWeight = CurrentWeight;
	while (1) {
		lcd_clear();
		lcd_printxy(0, 0, "[In Your Weight]");
		while (1) {
			sprintf(buf, "[Weight:%d kg]+-", iWeight);
			lcd_printxy(0, 1, buf);

			getBtn();
			// 키 입력시 몸무게 증가
			if (pin_0 == 0 && prev_pin_0 == 1) {
				iWeight++;
				if (iWeight > 300)
					iWeight = 0;
				break;
			}
			// 키 입력시 몸무게 감소
			if (pin_1 == 0 && prev_pin_1 == 1) {
				iWeight--;
				if (iWeight < 0)
					iWeight = 0;
				break;
			}
			if (pin_2 == 0 && prev_pin_2 == 1) {
				lcd_clear();
				lcd_printxy(0, 0, "[Input Weight]");
				sprintf(buf, "[Weight:%d kg]+-", iWeight);
				lcd_printxy(0, 1, buf);
				return iWeight;
			}
			if (pin_3 == 0 && prev_pin_3 == 1) {
				lcd_clear();
				lcd_printxy(0, 0, "[Input Weight]");
				sprintf(buf, "[Weight:%d kg]+-", CurrentWeight);
				lcd_printxy(0, 1, buf);
				return CurrentWeight;
			}
		}
	}
}
double InputHeightMenu(volatile double CurrentHeight) {
	double iHeight = CurrentHeight;
	while (1) {
		lcd_clear();
		lcd_printxy(0, 0, "[In Your Height]");
		while (1) {

			sprintf(buf, "[Height:%.2f m]+-", iHeight);
			lcd_printxy(0, 1, buf);

			getBtn();
			// 키 입력시 키 증가
			if (pin_0 == 0 && prev_pin_0 == 1) {
				iHeight += 0.01;
				break;
			}
			// 키 입력시 키 감소
			if (pin_1 == 0 && prev_pin_1 == 1) {
				iHeight -= 0.01;
				break;
			}
			if (pin_2 == 0 && prev_pin_2 == 1) {
				lcd_clear();
				lcd_printxy(0, 0, "[Input Height]");
				sprintf(buf, "[Height:%.2f m]+-", iHeight);
				lcd_printxy(0, 1, buf);
				return iHeight;
			}
			if (pin_3 == 0 && prev_pin_3 == 1) {
				lcd_clear();
				lcd_printxy(0, 0, "[Input Height]");
				sprintf(buf, "[Height:%.2f m]+-", CurrentHeight);
				lcd_printxy(0, 1, buf);
				return CurrentHeight;
			}
		}
	}
}

int main(void) {
	init();
	lcd_init();
	SysTick_Config(SystemCoreClock / 1000 / 3.125);
	int iCurrentMenu = 0;
	int iWeight = 50;
	double iHeight = 1.60;
	for (;;) {
		lcd_clear();
		if (iCurrentMenu == 0) {
			lcd_printxy(0, 0, "[Input Weight]");
			sprintf(buf, "[Weight:%d kg]", iWeight);
			lcd_printxy(0, 1, buf);
		}
		if (iCurrentMenu == 1) {
			lcd_printxy(0, 0, "[Input Height]");
			sprintf(buf, "[Height:%.2f m]", iHeight);
			lcd_printxy(0, 1, buf);
		}
		if (iCurrentMenu == 2) {
			lcd_printxy(0, 0, "[Calc BMI]");
			double bmi = iWeight/(iHeight*iHeight);
			sprintf(buf, "[BMI:%.2f]", bmi);
			lcd_printxy(0,1,buf);
		}

		while (1) {
			getBtn();

			if (pin_0 == 0 && prev_pin_0 == 1) {
				iCurrentMenu = (iCurrentMenu + 1) % 3; // 다음 메뉴
				break;
			}
			if (pin_2 == 0 && prev_pin_2 == 1) {
				if (iCurrentMenu == 0)
					iWeight = InputWeightMenu(iWeight);
				if (iCurrentMenu == 1)
					iHeight = InputHeightMenu(iHeight);
				if (iCurrentMenu == 2){
					lcd_clear();
					double bmi = iWeight/(iHeight*iHeight);
					sprintf(buf, "[BMI : %.2f]", bmi);
					lcd_printxy(0, 0, buf);
					if(bmi>= 30)
					lcd_printxy(0,1,"Obesity");
					else if(bmi>=25)
						lcd_printxy(0,1,"OVER Weight");
					else if(bmi>=18.5)
						lcd_printxy(0,1,"Nomal Weight");
					else if(bmi <18.5)
						lcd_printxy(0,1,"Under Weight");
				}
			}
		}
	}
}


// Sprintf(buf,"%f", 15.5);
//소숫점 표기를 위해 프로젝트
//properties -> C/C++ build -> setting -> MCU GCC Linker -> miscellaneous -> -u _printf_float 추가
