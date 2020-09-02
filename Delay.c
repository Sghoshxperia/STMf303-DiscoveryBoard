#include "stm32f303xc.h"

volatile static uint16_t myTicks=0;

void SysTick_Handler(void)
{
	myTicks++;
}

void delayMs(uint16_t ms)
{
	myTicks = 0;
	while(myTicks<ms);
}


int main()
{
	RCC ->AHBENR = RCC_AHBENR_GPIOBEN;
	
	GPIOB ->MODER |= GPIO_MODER_MODER9_0;
	GPIOB ->MODER &= ~(GPIO_MODER_MODER9_1);
	
	GPIOB ->OTYPER &= ~(GPIO_OTYPER_OT_9);
	
	GPIOB ->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR9;
	
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock/1000);
	
	while(1)
	{
		GPIOB ->BSRR |= GPIO_BSRR_BS_9;
		delayMs(1000);
		
		GPIOB ->BRR |= GPIO_BRR_BR_9;
		delayMs(1000);
	
	}
}




