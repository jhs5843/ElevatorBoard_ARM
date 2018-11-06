Timer Set 방법 
1. 
Processer 정의 (-d옵션)에 HSE_VALUE = 8000000 추가
SystemCoreClockUpdat();
SysTick_Config(SystemCoreClock / 1000);
2. 
	SysTick_Config(53676000 / 1000)
// 보통은 SystemCoreClock 변수를 사용 한다.	