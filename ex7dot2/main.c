/*
* Ver o porque daquelas linhas pretas horizontais
* Ver porque o backlight nao esta a funcionar
*
*Reconstruir vector initializers[] baseado no codigo do arduino
*Procurar por diferentes delays na escrita de comandos de inicialização
*
* Dar uma olhada em bibliotecas de graficos tipo Adafruit_GFX lib
* Problema na velocidade SPI que estou a usar?
*/


/*
*Adafruit lib: https://github.com/adafruit/Adafruit-ST7735-Library
*Arduino LCD page: https://store.arduino.cc/arduino-lcd-screen
*Adafruit LCD page: https://www.adafruit.com/product/358
*Adafruit GFX lib: https://github.com/adafruit/Adafruit-GFX-Library
*
*Experimentar comandos para o Bscreen
*Esperimentar com um arduino e depois de ter certeza de funcionamento do LCD re-examinar codigo
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
	ST7735_backLight(1);
	fillScreen(0x0000);

	while(1){
		static int ledval=0;
		//toggle led
		GPIO_WriteBit(GPIOC, GPIO_Pin_9, (ledval) ? Bit_SET : Bit_RESET);
		//GPIO_WriteBit(66, GPIO_Pin_9, (ledval) ? Bit_SET : Bit_RESET);
		ledval = 1-ledval;
		 // wait 250 ms

//----------------------LCD code
/*Sample colors:
*0x0000 Black
*0x001F Dark blue	<-
*0x07E0 Green	<-
*0x07FF Light blue
*0xF800 Red	<-
*0xF81F Magenta
*0xFFE0 Yellow
*0xFFFF White
*/
Delay(2000);
drawChar(20, 20, 'C', 0x07E0, 0xF800, 0);  //void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size) 
drawString(20, 40, "Hello world!", 0x07E0, 0xF800, 0); 

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
