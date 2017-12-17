#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <spi.h>
#include <eeprom.h>

uint8_t txbuf[4], rxbuf[4];
uint16_t txbuf16[4], rxbuf16[4];

//Timer code
void Delay(uint32_t nTime);

void main()
{
	//Enable Peripheral Clocks
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	//Configure Pins
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);
	//LEDInit(); optional for this tutorial
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//Init epprom
	eepromInit();
	//Configure SysTick Timer
	if (SysTick_Config(SystemCoreClock/1000)) while(1);

	uint8_t txbufa[16]={0x0F,0x0E,0x0D,0x0C,0x0B,0x0A,0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00};
	uint8_t rxbufa[16]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	uint8_t status=0;

	eepromWrite(&txbufa, 16, 0x0020);

	Delay(1000); //monte de tempo, dpc confirmar se continue robusto sem isto

	eepromRead(&rxbufa, 16, 0x0020);

	if((rxbufa[0]==0x0F)){	//End of execution indicator
		GPIO_WriteBit(GPIOC, GPIO_Pin_9, Bit_SET);
	}else{
		GPIO_WriteBit(GPIOC, GPIO_Pin_9, Bit_RESET);
	}

	eepromWriteEnable();
	status=eepromReadStatus();
	eepromWriteStatus(0x00);
	status=eepromReadStatus();

}

//Assert code
#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{
	/*Infinite loop*/
	/*Use GDB to find out why we're here*/
}
#endif

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
