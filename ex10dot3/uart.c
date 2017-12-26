#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_usart.h>
#include "uart.h"

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
		// Modify USART_InitStructure for non-default values
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

