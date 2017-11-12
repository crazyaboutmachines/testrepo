//P.S. no fim experimentar fazer uma limpeza de ficheiros para ver o que é essencial ou não
#include <stm32f10x.h>

int main(void)
{
int button;
//Enable Peripheral Clocks
RCC->APB2ENR|=0x00000014;
//Configure Pins
GPIOC->CRH=0x02;
GPIOA->CRL=0x04;

while(1){
//read button
button=((GPIOA ->IDR & 0x1) == 0);
	if(button){
		GPIOC->BSRR=0x01000000; //reset
	}else{
		GPIOC->BSRR=0x00000100; //set
	}	
}
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{
/*Infinite loop*/
/*Use GDB to find out why we're here*/
}
#endif
