#include "stm32f10x.h"

uint32_t CAN_Receive()
{
	uint32_t data = 0;
	
	if(CAN1 ->RF0R & CAN_RF0R_FULL0)
	{
		data = CAN1 ->sFIFOMailBox[0].RDLR;
		CAN1 ->RF0R |= CAN_RF0R_RFOM0;
		
	}
	return data;
}

int main()
{
	RCC ->APB2ENR |= RCC_APB2ENR_IOPBEN;
	
	GPIOB ->CRH &= ~(GPIO_CRH_CNF12);
	GPIOB ->CRH |= GPIO_CRH_MODE12_0;
	
	
	
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
	
	
	CAN1 ->FMR |= CAN_FMR_FINIT;
	CAN1 ->FA1R &= ~(unsigned)(CAN_FA1R_FACT0);
	CAN1 ->FM1R |= CAN_FM1R_FBM0;
	CAN1 ->FS1R|= CAN_FS1R_FSC0;
	CAN1 ->FFA1R &= ~(unsigned)(CAN_FFA1R_FFA0);
	CAN1 ->sFilterRegister[0].FR1=  CAN_F0R1_FB1 | CAN_F0R1_FB3;
	CAN1 ->FA1R |= CAN_FA1R_FACT0;
	CAN1 ->FMR &= ~(unsigned)(CAN_FMR_FINIT);
	
	while(1)
	{
		if(CAN_Receive()==0x42)
		{
			GPIOB ->BSRR |= GPIO_BSRR_BS12;
			for(int i = 0; i < 200000; i++);
			GPIOB ->BRR |= GPIO_BRR_BR12;
			for(int i = 0; i < 200000; i++);
		}
		
	}
	
}



