#ifndef LCD_H
#define LCD_H

#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <spi.h>

#define LCD_PORT GPIOC
#define GPIO_PIN_DC GPIO_Pin_2
#define GPIO_PIN_SCE GPIO_Pin_0
#define GPIO_PIN_RST GPIO_Pin_1 //PC1
#define LCD_PORT_BKL GPIOA
#define GPIO_PIN_BKL GPIO_Pin_1

#define GPIO_PIN_SD GPIO_Pin_6//optional
#define SD_PORT GPIOC//optional

#define SPILCD SPI2
#define LCDSPEED SPI_SLOW   //conforme a velocidade de clock ver se há que mudar esta velocidade

#define MADCTLGRAPHICS 0x6
#define MADCTLBMP 0x2
#define ST7735_width 128
#define ST7735_height 160

#define LOW 0
#define HIGH 1
#define LCD_C LOW
#define LCD_D HIGH

#define ST7735_CASET 0x2A
#define ST7735_RASET 0x2B
#define ST7735_MADCTL 0x36
#define ST7735_RAMWR 0x2C
#define ST7735_RAMRD 0x2E
#define ST7735_COLMOD 0x3A
#define MADVAL(x) (((x) << 5) | 8)
static uint8_t madctlcurrent = MADVAL(MADCTLGRAPHICS);

//ST7735 Internal Primitives
static void LcdWrite(char dc, const char *data , int nbytes);
static void LcdWrite16(char dc, const uint16_t *data , int cnt);
static void ST7735_writeCmd(uint8_t c);
//Simple driver
void ST7735_setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t madctl);
void ST7735_pushColor(uint16_t *color , int cnt);
void ST7735_init();
void ST7735_backLight(uint8_t on);
//Aditional functions
void fillScreen(uint16_t color);

#endif
