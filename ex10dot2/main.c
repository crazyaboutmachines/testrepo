#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stdio.h> //Add: "LDLIBS += -lm" on local Makefile
#include "uart.h"
#include "i2c.h"

void Delay(uint32_t nTime);

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
	uart_open(USART1 , 9600 , 0); //Rever o que é aquela historia das flags

	//init I2C
	I2C_LowLevel_Init(I2C1 , 10000 , 0x05);
	// Init
	#define NUNCHUK_ADDRESS 0xA4
	const uint8_t buf[] = {0xf0, 0x55};
	const uint8_t buf2[] = {0xfb, 0x00};
	I2C_Write(I2C1 , buf, 2, NUNCHUK_ADDRESS);
	I2C_Write(I2C1 , buf2 , 2, NUNCHUK_ADDRESS);
	uint8_t data[6];
	const uint8_t buf3[] = {0};
	int i, n, JX, JY, AX, AY, AZ, C, Z;
	char buffer [60];

	while(1){
	// I2C Read
	I2C_Write(I2C1 , buf3, 1, NUNCHUK_ADDRESS);
	I2C_Read(I2C1 , data , 6, NUNCHUK_ADDRESS);
	JX=data[0];
	JY=data[1];
	AX=((data[5]&0b00001100)>>2)+(data[2]<<2);
	AY=((data[5]&0b00110000)>>4)+(data[3]<<2);
	AZ=((data[5]&0b11000000)>>6)+(data[4]<<2);
	C=1-((data[5]&0b00000010)>>1);
	Z=1-(data[5]&0b00000001);
	//Added "LDFLAGS+= -specs=nosys.specs" on local makefile in order to use sprintf
	n=sprintf(buffer, "JX:%d \tJY:%d \tAX:%d \tAY:%d \tAZ:%d \tC:%d \tZ:%d \n\r", JX, JY, AX, AY, AZ, C, Z); 
	Delay(100); // wait 100 ms

	static int ledval=0;
	//toggle led
	GPIO_WriteBit(GPIOC, GPIO_Pin_9, (ledval) ? Bit_SET : Bit_RESET);
	ledval = 1-ledval;
	//send values to PC
	for (i=0; i<n; i++) {
		uart_putc(buffer[i], USART1);
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

