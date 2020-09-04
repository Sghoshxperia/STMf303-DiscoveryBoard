#include "stm32f303xc.h"

volatile static uint16_t myTicks = 0;
volatile static uint32_t val = 0;
volatile static uint32_t mapfdone = 0;
volatile static uint32_t value= 0;


uint16_t map(uint32_t z,float input_l,float input_h,float output_l,float output_h)
{
	float mapf = (z - input_l)/(input_h-input_l)*(output_h-output_l)+output_l;
	mapfdone = (uint16_t)mapf;
	
	
	return(mapfdone);
}


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
	
	RCC ->AHBENR = RCC_AHBENR_GPIODEN ;
	
	GPIOD ->MODER |= GPIO_MODER_MODER15_1;
	GPIOD->AFR[1] |= 1<<29;

	
	//Setting up PWM
	RCC ->APB1ENR |= RCC_APB1ENR_TIM4EN;
	
	TIM4 ->CCER |= TIM_CCER_CC4E;
	TIM4 ->CR1 |= TIM_CR1_ARPE;
	TIM4 ->CCMR2 &= ~(TIM_CCMR2_CC4S);
  TIM4 ->CCMR2 |= TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4PE;	
	
	TIM4 ->PSC = 1;
	TIM4 ->ARR = 1000;
	TIM4 ->CCR4 = 0;
	
	TIM4 ->EGR = TIM_EGR_UG;
	TIM4 ->CR1 = TIM_CR1_CEN;
	

	RCC ->CFGR2 |= RCC_CFGR2_ADCPRE12_DIV6;
  RCC ->AHBENR |= RCC_AHBENR_ADC12EN | RCC_AHBENR_GPIOAEN;
	
	GPIOA ->MODER |= GPIO_MODER_MODER2_1 | GPIO_MODER_MODER2_0;
	//GPIOA ->MODER &= ~(GPIO_MODER_MODER1_0);
	
	
	ADC1 ->CR &= ~ADC_CR_ADEN;
	
	ADC1->CR &= ~(ADC_CR_ADVREGEN_1 | ADC_CR_ADVREGEN_0);
	ADC1->CR |= ADC_CR_ADVREGEN_0;
	delayMs(10);
	
	
	
	ADC1_2_COMMON ->CCR |= ADC12_CCR_CKMODE_0;
	
	ADC1->CR &= ~(ADC_CR_ADCALDIF);
	ADC1 ->CR |= ADC_CR_ADCAL;
	while((ADC1 ->CR & ADC_CR_ADCAL)!=0);
	delayMs(10);
	
	ADC1 ->CR |= ADC_CR_ADEN;
	while((ADC1 ->ISR & ADC_ISR_ADRDY)==0);
	
	ADC1->SQR1 |= ADC_SQR1_SQ1_1 | ADC_SQR1_SQ1_0;//channel 3 in sequence 1
	ADC1->SMPR1 |= ADC_SMPR1_SMP3_2 | ADC_SMPR1_SMP3_1; 
	
	ADC1 ->CFGR |= ADC_CFGR_CONT;
	ADC1 ->CFGR &= ~ADC_CFGR_RES;
	
	ADC1 ->CR |= ADC_CR_ADSTART;
	
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock/1000);
	
	
	
	while(1)
	{
		
		
		//while((ADC1 ->ISR & ADC_ISR_EOC)==0);
		val = ADC1 ->DR;
		TIM4 ->CCR4 = map(val, 0, 4095, 0, 1000);
		delayMs(10);
		
	}
	
	
	


}

