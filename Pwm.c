#include "stm32f303xc.h"

int main()
{
	//Pwm in channel 4 of timer4 pinb9
	
	
	RCC ->AHBENR = RCC_AHBENR_GPIOAEN;
	
	GPIOA ->MODER |= GPIO_MODER_MODER11_1;
	GPIOA ->MODER &= ~(GPIO_MODER_MODER11_0);
	
	GPIOA ->OTYPER &= ~(GPIO_OTYPER_OT_11);
	
	GPIOA ->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR11;
	
	//Setting up PWM
	RCC ->APB1ENR |= RCC_APB1ENR_TIM4EN;
	
	TIM4 ->CCER |= TIM_CCER_CC4E;
	TIM4 ->CR1 |= TIM_CR1_ARPE;
	TIM4 ->CCMR2 &= ~(TIM_CCMR2_CC4S);
  TIM4 ->CCMR2 |= TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4PE;	
	
	TIM4 ->PSC = 8;
	TIM4 ->ARR = 1000;
	TIM4 ->CCR4 = 250;
	
	TIM4 ->EGR = TIM_EGR_UG;
	TIM4 ->CR1 = TIM_CR1_CEN;
	
	while(1)
	{
		
		for(uint16_t i=0;i<1000;i++)
		TIM4 ->CCR4 = i;
		
	}
	
}
