void EXTI2_IRQHandler()
{
	
	if (EXTI_GetITStatus(EXTI_Line2) != RESET)
	//인터럽트 5..9 등이 같은 인터럽트를 공유하므로 그런 인터럽트들의 구분을 위해 체크.
	{

		// 인터럽트 대기 비트를 초기화; 하지 않으면 인터럽트가 처리되지 않은 것으로 보고 계속 호출. 
		EXTI_ClearITPendingBit(EXTI_Line2);
	}

}

void main()
{
	RCC_AHB1PeriphCLockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	GPIO_InitTypeDef gpio;
	gpio.GPIO_Mode = GPIO_Mode_IN;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_Pin = GPIO_Pin_2;
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOD, &gpio);

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource2);

	EXTI_InitTypeDef exti;
	exti.EXTI_Line = EXTI_Line2;
	exti.EXTI_Mode = EXTI_Mode_Interrupt;
	exti.EXTI_Trigger = EXTI_Trigger_Rising;
	exti.EXTI_LineCmd = ENABLE;
	EXTI_Init(&exti);

	NVIC_InitTypeDef nvic;
	nvic.NVIC_IRQChannel = EXTI2_IRQn;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	nvic.NVIC_IRQChannelPreemptionPriority = 0x00;
	nvic.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_Init(&nvic);

	while(1);
}








