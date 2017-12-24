#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include "uart.h"
#include "i2c.h"

void Delay(uint32_t nTime);

char frase[]="Hello World!\n\r";
int i;

int main(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//Enable Peripheral Clocks
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	//Configure Pins
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//Configure SysTick Timer
	if (SysTick_Config(SystemCoreClock/1000))
		while(1);
	//init uart
	uart_open(USART1 , 9600 , 0); //rever o que é aquela historia das flags

//I2Ccode:------------------------------------------------------------
I2C_LowLevel_Init(I2C1 , 10000 , 0x05);


// Init
#define NUNCHUK_ADDRESS 0xA4
const uint8_t buf[] = {0xf0, 0x55};
const uint8_t buf2[] = {0xfb, 0x00};
I2C_Write(I2C1 , buf, 2, NUNCHUK_ADDRESS);
I2C_Write(I2C1 , buf2 , 2, NUNCHUK_ADDRESS);

uint8_t data[6];
const uint8_t buf3[] = {0};
//--------------------------------------------------------------------

	while(1){
	

//I2Ccode:------------------------------------------------------------
Delay(500); // wait 250 ms


// Read
I2C_Write(I2C1 , buf3, 1, NUNCHUK_ADDRESS);
I2C_Read(I2C1 , data , 6, NUNCHUK_ADDRESS);

Delay(500);
//--------------------------------------------------------------------




static int ledval=0;
	//toggle led
	GPIO_WriteBit(GPIOC, GPIO_Pin_9, (ledval) ? Bit_SET : Bit_RESET);
	ledval = 1-ledval;
	//send character
	for (i=0; i<17; i++) {
		uart_putc(frase[i], USART1);
	}

	for (i=0; i<6; i++) {
		uart_putc(data[i], USART1);
	}

	}
}

//Timer code
static __IO uint32_t TimingDelay;

void Delay(uint32_t nTime){
	TimingDelay = nTime;
	while(TimingDelay != 0);
}

void SysTick_Handler(void){
	if(TimingDelay !=0x00)
	TimingDelay--;
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{
	/*Infinite loop*/
	/*Use GDB to find out why we're here*/
}
#endif




