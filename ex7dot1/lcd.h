#ifndef LCD_H
#define LCD_H

#include <stm32f10x.h>
//#include <stm32f10x_rcc.h>
//#include <stm32f10x_gpio.h>
//#include <spi.h>

#define MADCTLGRAPHICS 0x6
#define MADCTLBMP 0x2
#define ST7735_width 128
#define ST7735_height 160

//simple driver--------------------
void ST7735_setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t madctl);
void ST7735_pushColor(uint16_t *color , int cnt);
void ST7735_init();
void ST7735_backLight(uint8_t on);
//----------------------------------

//aditional functions---------------
void fillScreen(uint16_t color);
//----------------------------------

#endif
