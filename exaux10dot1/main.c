#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_tim.h>

void Delay(uint32_t nTime);

int main(void)
{
	//Configure GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	//Enable GPIO Clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//Configure Pins
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//Configure SysTick Timer
	if (SysTick_Config(SystemCoreClock/1000))
		while(1);

	//Configure PWM Timer
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	//Enable timer clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
	// configure timer
	// PWM frequency = 100 hz with 24,000,000 hz system clock
	// 24,000,000/240 = 100,000
	// 100,000/2000 = 50
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = SystemCoreClock /100000 - 1; // 0..239
	TIM_TimeBaseStructure.TIM_Period = 2000 - 1; // 0..999
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2 , &TIM_TimeBaseStructure);
	// PWM1 Mode configuration: Channel2
	// Edge -aligned; not single pulse mode
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC2Init(TIM2 , &TIM_OCInitStructure);
	// Enable Timer
	TIM_Cmd(TIM2 , ENABLE);

	int pw=100;
	/*
	P.S. the pulse width for diferent servos seems to vary with the model e.g.: 
	https://www.ptrobotics.com/servo/4782-tower-pro-sg90-servo-9g.html
	*/
	while(1){
		if(pw==100){
			for(pw=100; pw<200; pw++){
				TIM_SetCompare2(TIM2 , pw);  //pw (pulse width) (0..1999) 
				Delay(10); // wait 10 ms
			} 
		}else{
			for(pw=200; pw>100; pw--){
				TIM_SetCompare2(TIM2 , pw); 
				Delay(10);
			}
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
