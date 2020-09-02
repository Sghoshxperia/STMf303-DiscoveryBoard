#include "stm32f303xc.h"

int main()
{
	RCC ->AHBENR = RCC_AHBENR_GPIOBEN;
	
	GPIOB ->MODER |= GPIO_MODER_MODER9_0;
	GPIOB ->MODER &= ~(GPIO_MODER_MODER9_1);
	
	GPIOB ->OTYPER &= ~(GPIO_OTYPER_OT_9);
	
	GPIOB ->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR9;
	
	while(1)
	{
		GPIOB ->BSRR |= GPIO_BSRR_BS_9;
		for(int i = 0; i < 200000; i++);
		
		GPIOB ->BRR |= GPIO_BRR_BR_9;
		for(int i = 0; i < 200000; i++);
	
	}
}




