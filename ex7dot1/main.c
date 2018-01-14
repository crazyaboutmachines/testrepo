/*
*
*Completar aquelas inicializações de pinos
*Tentar ja fazer algo no main
*
*Ver onde por aquelas estruturas
*
*
*
*
*/


#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <lcd.h>

void Delay(uint32_t nTime);

int main(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//Enable Peripheral Clocks
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); // (1)
	//Configure Pins
	GPIO_StructInit(&GPIO_InitStructure); //2
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	//Configure SysTick Timer
	if (SysTick_Config(SystemCoreClock/1000))
		while(1);

	//init LCD
	ST7735_init();

	while(1){
		static int ledval=0;
		//toggle led
		GPIO_WriteBit(GPIOC, GPIO_Pin_9, (ledval) ? Bit_SET : Bit_RESET);
		//GPIO_WriteBit(66, GPIO_Pin_9, (ledval) ? Bit_SET : Bit_RESET);
		ledval = 1-ledval;
		Delay(5000); // wait 250 ms

//----------------------LCD code
/*Sample colors:
*0x0000 Black
*0x001F Dark blue
*0x07E0 Green
*0x07FF Light blue
*0xF800 Red
*0xF81F Magenta
*0xFFE0 Yellow
*0xFFFF White
*/

fillScreen(0x07E0);
//------------------------------
	}
}

//Timer code
static __IO uint32_t TimingDelay;

void Delay(uint32_t nTime)
{
	TimingDelay = nTime;
	while(TimingDelay != 0);
}

void SysTick_Handler(void)
{
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
