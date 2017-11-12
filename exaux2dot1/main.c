#include <stm32f10x.h>

void delay(void){
	int i = 100000;/* About 1/4 second delay */
	while (i-- > 0)
		asm("nop");
}

int main(void){
	int n = 0;
	int button;

	/* Enable the GPIOA (bit 2) and GPIOC (bit 4) */
	/* See 6.3.7 in stm32f100x reference manual */

	RCC->APB2ENR |= 0x10 | 0x04;

	/* Set GPIOC Pin 8 and Pin 9 to outputs */
	/* 7.2.2 in stm32f100x reference manual */

	GPIOC->CRH = 0x11;

	/* Set GPIOA Pin 0 to input floating */
	/* 7.2.1 in stm32f100x reference manual */

	GPIOA->CRL = 0x04;

	while(1){
		delay();
		// Read the button - the button pulls down PA0 to logic 0
		button = ((GPIOA ->IDR & 0x1) == 0);
		n++;

		/* see 7.2.5 in stm32f100x reference manual */

		if (n & 1) {	//PC8
			GPIOC ->BSRR = 1<<8 ;
			} else {
			GPIOC ->BSRR = 1<<24;
			}
		if ((n & 4) && button) {	//PC9
			GPIOC ->BSRR = 1<<9 ;
			} else {
			GPIOC ->BSRR = 1<<25;
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
