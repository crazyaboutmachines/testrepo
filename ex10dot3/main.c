#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include "uart.h"
#include <stm32f10x_tim.h>

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
//Timers code:-------------------------------------------------------------------------
	
	//Enable Peripheral Clocks
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//Configure Pins
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);


//Configure PWM @ pin PB9 => TIM4_CH4
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	//Enable timer clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE);
	// configure timer
	// PWM frequency = 100 hz with 24,000,000 hz system clock
	// 24,000,000/240 = 100,000 (kHz) (output from the preescaler)
	// 100,000/10000 = 10 (Hz) (output from the counter)
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = SystemCoreClock /100000 - 1; // 0..239
	TIM_TimeBaseStructure.TIM_Period = 10000 - 1; // 0..9999 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4 , &TIM_TimeBaseStructure);
	// PWM1 Mode configuration: Channel4
	// Edge -aligned; not single pulse mode
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC4Init(TIM4 , &TIM_OCInitStructure);
	// Enable Timer
	TIM_Cmd(TIM4 , ENABLE);

	int pw=1; //10us == 1 count at 100kHz 



//-------------------------------------------------------------------------------------

	while(1){
	static int ledval=0;
//--------------------------------------------------------------------Timer code---------
TIM_SetCompare4(TIM4 , pw);  //pw (pulse width) (0..1999) 
				Delay(10); // wait 10 ms

//-------------------------------------------------------------------------------------
	//toggle led
	GPIO_WriteBit(GPIOC, GPIO_Pin_9, (ledval) ? Bit_SET : Bit_RESET);
	ledval = 1-ledval;
	//send character
	for (i=0; i<17; i++) {
		uart_putc(frase[i], USART1);
	}
	Delay(250); // wait 250 ms

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


