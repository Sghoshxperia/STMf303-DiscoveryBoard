#include "stm32f10x.h"

uint32_t value = 0x42;

void CAN_Transmit(uint32_t data)
{
	CAN1 ->sTxMailBox[0].TDTR |= (1<<0);
	CAN1 ->sTxMailBox[0].TIR |= (1<<24) | (1<<22);
	CAN1 ->sTxMailBox[0].TDLR = data;
	CAN1 ->sTxMailBox[0].TIR |= (1<<0);
	
	while(~(CAN1 ->TSR & 0x01)); 
}

int main()
{
	
	RCC ->APB1ENR |= RCC_APB1ENR_CAN1EN;
	RCC ->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN;
	
	GPIOA ->CRH |= GPIO_CRH_CNF12_1 | GPIO_CRH_MODE12;
	GPIOA ->CRH &= ~GPIO_CRH_CNF12_0;
	GPIOA ->CRH &= ~GPIO_CRH_MODE11;
	
	CAN1 ->MCR |= CAN_MCR_INRQ;
	
	CAN1->BTR|=(1<<0);                                       // Prescaler=9                      
	CAN1->BTR|=(1<<19)|(1<<18);                              // Seg1=13
	CAN1->BTR|=(1<<20);                                      // Seg2=2
	
	CAN1 ->MCR &= ~(unsigned)(CAN_MCR_INRQ);
	while((CAN1 ->MCR & 0x01) == 1);
	
	while(1)
	{
		CAN_Transmit(value);
		for(int i = 0; i<100000; i++ );
		
	}
	
	
	
}

