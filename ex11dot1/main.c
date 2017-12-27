#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <misc.h>
#include <stm32f10x_tim.h>

static int ledval=0; //try without static?

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
// Configure clock for TIM2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
// Configure NVIC
	/*
	* NVIC_PriorityGroup_0 0 bits priority , 4 bits subgroup
	* NVIC_PriorityGroup_1 1 bits priority , 3 bits subgroup
	* NVIC_PriorityGroup_2 2 bits priority , 2 bits subgroup
	* NVIC_PriorityGroup_3 3 bits priority , 1 bits subgroup
	* NVIC_PriorityGroup_4 4 bits priority , 0 bits subgroup
	*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	NVIC_InitTypeDef NVIC_InitStructure;
	// No StructInit call in API
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
// Configure Timer
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	// PWM frequency = 100 hz with 24,000,000 hz system clock
	// 24,000,000/2400 = 10,000
	// 10,000/5000 = 2
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = SystemCoreClock /10000 - 1; // 0..2399
	TIM_TimeBaseStructure.TIM_Period = 5000 - 1; // 0..4999
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2 , &TIM_TimeBaseStructure);
// Enable Timer Interrupt , enable timer
	TIM_ITConfig(TIM2 , TIM_IT_Update , ENABLE);
	TIM_Cmd(TIM2 , ENABLE);

	while(1){ /* do nothing */ }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{
/*Infinite loop*/
/*Use GDB to find out why we're here*/
}
#endif

void TIM2_IRQHandler(void)
{
	GPIO_WriteBit(GPIOC, GPIO_Pin_9, ledval);
	ledval = 1-ledval;
	TIM_ClearITPendingBit(TIM2 ,TIM_IT_Update);
}

