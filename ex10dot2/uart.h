#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_usart.h>
//ver como se deve fazer aquela cena de preprocessador #ifndef ... #def

int uart_open(USART_TypeDef* USARTx , uint32_t baud , uint32_t flags);
int uart_getc(USART_TypeDef* USARTx);
int uart_putc(int c, USART_TypeDef* USARTx);
int uart_close(USART_TypeDef* USARTx);
