#include <stdio.h>
#include "stm32f4xx.h"
#include "lcd.h"

uint32_t FLASH_DATA_SECTOR = 0x080E0000; // 섹터 11의 시작 메모리 번지 

int main(void)
{
	volatile unsigned short size = *(unsigned short*)(0x1FFF7A22); // Flash Size in KB

	volatile int value = ((int*)(FLASH_DATA_SECTOR))[0];

	if(value == 0xffffffff) // Initial State: 초기화가 필요함.
		value = 0;

	value++;

	int ret;

	FLASH_Unlock();  // 잠금 해제 
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);  //에러 플래그들 초기화함.
	ret = FLASH_EraseSector(FLASH_Sector_11, VoltageRange_3); // 값 저장 전에는 섹터를 초기화해야함.
	ret = FLASH_ProgramWord(FLASH_DATA_SECTOR, value); // 한 바이트씩 저장하는 것은 FLASH_ProgramByte
	FLASH_Lock();  // 잠금

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	lcd_init();

	char buf[255];
	sprintf(buf, "%d th boot.", value);
	lcd_printxy(0, 1, buf);

	for(;;)
	{
	}
}
