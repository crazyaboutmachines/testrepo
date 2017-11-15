#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>

#include <stm32f10x_usart.h>

void Delay(uint32_t nTime);

//funções a implementar (dps passar p ficheiro externo)
int uart_open(USART_TypeDef* USARTx , uint32_t baud , uint32_t flags);
int uart_getc(USART_TypeDef* USARTx);
int uart_putc(int c, USART_TypeDef* USARTx);
int uart_close(USART_TypeDef* USARTx);

//char frase[]="Hello World!";
char frase[]="Hello World!\n\r";
int i;

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

//init uart
uart_open(USART1 , 9600 , 0); //rever o que é aquela historia das flags

while(1){
static int ledval=0;
//toggle led
GPIO_WriteBit(GPIOC, GPIO_Pin_9, (ledval) ? Bit_SET : Bit_RESET);
ledval = 1-ledval;
//send character
//Hello World!\n\r-------------------------
for (i=0; i<17; i++) {
uart_putc(frase[i], USART1);
}

//------------------------------------

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


//================================================================temp

int uart_open(USART_TypeDef* USARTx , uint32_t baud , uint32_t flags)
{
	//1. Initialize usart/gpio clocks.
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA , ENABLE);
	//2. Configure usart pins
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_StructInit(&GPIO_InitStruct);
		// Initialize USART1_Tx
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA , &GPIO_InitStruct);
		// Initialize USART1_RX
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA , &GPIO_InitStruct);
	//3. Configure and enable the usart1
		// see stm32f10x_usart.h
	USART_InitTypeDef USART_InitStructure;
		// Initialize USART structure
	USART_StructInit(&USART_InitStructure);
		// Modify USART_InitStructure for non-default values , e.g.
		// USART_InitStructure.USART_BaudRate = 38400;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1 ,&USART_InitStructure);
	USART_Cmd(USART1 , ENABLE);
return 0;
}

int uart_getc(USART_TypeDef* USARTx){
while (USART_GetFlagStatus(USARTx , USART_FLAG_RXNE) == RESET);
return USARTx ->DR & 0xff;
}

int uart_putc(int c, USART_TypeDef* USARTx){
while (USART_GetFlagStatus(USARTx , USART_FLAG_TXE) == RESET);
USARTx ->DR = (c & 0xff);
return 0;
}


int uart_close(USART_TypeDef* USARTx){
USART_DeInit(USARTx);
return 0;
}

