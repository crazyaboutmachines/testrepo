#include <stm32f10x.h>
#include <lcd.h>
//#include <stm32f10x_rcc.h>
//#include <stm32f10x_gpio.h>
//#include <spi.h>



//-----------------------ST7735 Internal Primitives
void ST7735_setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t madctl)
{

}


void ST7735_pushColor(uint16_t *color , int cnt)
{

}


void ST7735_init()
{

}


void ST7735_backLight(uint8_t on)
{

}



//-----------------------additional functions

void fillScreen(uint16_t color)
{
	uint8_t x,y;
	ST7735_setAddrWindow(0, 0, ST7735_width-1, ST7735_height-1, MADCTLGRAPHICS);
	for (x=0; x < ST7735_width; x++) {
		for (y=0; y < ST7735_height; y++) {
			ST7735_pushColor(&color ,1);
		}
	}
}
