#include <stm32f10x.h>
#include <lcd.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <spi.h>

struct ST7735_cmdBuf {
	uint8_t command; // ST7735 command byte
	uint8_t delay; // ms delay after
	uint8_t len; // length of parameter data
	uint8_t data[16]; // parameter data
};

static const struct ST7735_cmdBuf initializers[] = { //initializer for LCD controller (for ILI9163C displays?(check datashet))
	{ 0x01, 50, 0, 0},		// 1: Software reset, no args, w 50ms delay
	{ 0x11, 100, 0, 0},		// 2: Out of sleep mode, no args, w 100ms delay
	{ 0x26, 0, 1, { 0x04}}, 	// 3: Set default gamma 16-bit color
	{ 0xb1, 0, 2, { 0x0b, 0x14 }}, 	// 4: Frame Rate
	{ 0xc0, 0, 2, { 0x08, 0x00 }}, 	// 5: VRH1[4:0] & VC[2:0]
	{ 0xc1, 0, 1, { 0x05}}, 	// 6: BT[2:0]
	{ 0xc5, 0, 2, { 0x41, 0x30 }}, 	// 7: VMH[6:0] & VML[6:0]
	{ 0xc7, 0, 1, { 0xc1}}, 	// 8: LCD Driving control
	{ 0xEC, 0, 1, { 0x1b}}, 	// 9: Set pumping color freq
	{ 0x3a, 100, 1, { 0x55 }},	// 10: Set color format, 16-bit color
	{ 0x2a, 0, 4, { 0x00, 0x00, 0x00, 0x7f}}, // 11: Set Column Address
	{ 0x2b, 0, 4, { 0x00, 0x00, 0x00, 0x9f}}, // 12: Set Page Address
	{ 0x36, 0, 1, { 0xc8}},		// 12+1: Set Scanning Direction
	{ 0xb7, 0, 1, { 0x00}}, 	// 14: Set Source Output Direction
	{ 0xf2, 0, 1, { 0x00}}, 	// 15: Enable Gamma bit
	{ 0xe0, 50, 15, { 0x28, 0x24, 0x22, 0x31, 0x2b, 0x0e, 0x53, 0xa5, 0x42, 0x16, 0x18, 0x12, 0x1a, 0x14, 0x03}},	// 16: magic
	{ 0xe1, 50, 15, { 0x17, 0x1b, 0x1d, 0x0e, 0x14, 0x11, 0x2c, 0xa5, 0x3d, 0x09, 0x27, 0x2d, 0x25, 0x2b, 0x3c}},	// 17: more magic
	{ 0x13, 10, 0, 0}, 		// 17: Normal display on, no args, w 10ms delay
	{ 0x29, 255, 0, 0} 		// 18: Main screen turn on, no args, w 255 delay
};


//-------------------------ST7735 Internal Primitives
static void LcdWrite(char dc, const char *data , int nbytes)
{
	GPIO_WriteBit(LCD_PORT ,GPIO_PIN_DC , dc); // dc 1 = data , 0 =control
	GPIO_ResetBits(LCD_PORT ,GPIO_PIN_SCE);
	spiReadWrite(SPILCD , 0, data , nbytes , LCDSPEED);
	GPIO_SetBits(LCD_PORT ,GPIO_PIN_SCE);
}

static void LcdWrite16(char dc, const uint16_t *data , int cnt)
{
	GPIO_WriteBit(LCD_PORT ,GPIO_PIN_DC , dc); // dc 1 = data , 0 =control
	GPIO_ResetBits(LCD_PORT ,GPIO_PIN_SCE);
	spiReadWrite16(SPILCD , 0, data , cnt, LCDSPEED);
	GPIO_SetBits(LCD_PORT ,GPIO_PIN_SCE);
}

static void ST7735_writeCmd(uint8_t c)
{
	LcdWrite(LCD_C , &c, 1);
}

//-----------------------Simple driver
void ST7735_setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t madctl)
{
	madctl = MADVAL(madctl);
	if (madctl != madctlcurrent){
		ST7735_writeCmd(ST7735_MADCTL);
		LcdWrite(LCD_D , &madctl , 1);
		madctlcurrent = madctl;
	}
	ST7735_writeCmd(ST7735_CASET);
	LcdWrite16(LCD_D , &x0, 1);
	LcdWrite16(LCD_D , &x1, 1);
	
	ST7735_writeCmd(ST7735_RASET);
	LcdWrite16(LCD_D , &y0, 1);
	LcdWrite16(LCD_D , &y1, 1);

	ST7735_writeCmd(ST7735_RAMWR);
}


void ST7735_pushColor(uint16_t *color , int cnt)
{
	LcdWrite16(LCD_D , color , cnt);
}

void ST7735_init()
{
	const struct ST7735_cmdBuf *cmd;
	// set up pins--------------------------------------------------
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);
	//Enable Peripheral Clocks
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA| RCC_APB2Periph_GPIOC, ENABLE);
	//Configure Pins
	//BKL
	GPIO_InitStructure.GPIO_Pin=GPIO_PIN_BKL;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(LCD_PORT_BKL, &GPIO_InitStructure);
	//RESET
	GPIO_InitStructure.GPIO_Pin=GPIO_PIN_RST;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(LCD_PORT, &GPIO_InitStructure);
	//RS
	GPIO_InitStructure.GPIO_Pin=GPIO_PIN_DC;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(LCD_PORT, &GPIO_InitStructure);
	//LCD CS PC0
	GPIO_InitStructure.GPIO_Pin=GPIO_PIN_SCE;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(LCD_PORT, &GPIO_InitStructure);
	//SD_CS PC6
	GPIO_InitStructure.GPIO_Pin=GPIO_PIN_SD;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(SD_PORT, &GPIO_InitStructure);
	//MISO, MOSI, SCLK and SPIinit
	spiInit(SPILCD);

	// set cs, reset low--------------------------------------------
	GPIO_WriteBit(SD_PORT ,GPIO_PIN_SD , HIGH); //SD not used for now
	GPIO_WriteBit(LCD_PORT ,GPIO_PIN_SCE , HIGH);
	GPIO_WriteBit(LCD_PORT ,GPIO_PIN_RST , HIGH);
	Delay(10);
	GPIO_WriteBit(LCD_PORT ,GPIO_PIN_RST , LOW);
	Delay(10);
	GPIO_WriteBit(LCD_PORT ,GPIO_PIN_RST , HIGH);
	Delay(10);
	// Send initialization commands to ST7735
	for (cmd = initializers; cmd->command; cmd++)
	{
		LcdWrite(LCD_C , &(cmd->command), 1);
		if (cmd->len)
			LcdWrite(LCD_D , cmd->data , cmd->len);
		if (cmd->delay)
			Delay(cmd->delay);
	}
}


void ST7735_backLight(uint8_t on)
{
	if (on)
		GPIO_WriteBit(LCD_PORT_BKL ,GPIO_PIN_BKL , LOW);
	else
		GPIO_WriteBit(LCD_PORT_BKL ,GPIO_PIN_BKL , HIGH);
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

void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size) 
{
	int i, j;
	int8_t aux;
	//for now size and clipping not implemented
	//draw
	ST7735_setAddrWindow(x, y, x+4 , y+7, MADCTLGRAPHICS);
	for (j=0; j < 8; j++) {
		for (i=0; i < 5; i++) {
			aux=(int8_t)font[c*5+i];
			if(((aux>>j) & 0x01)==0x01)
			{

				ST7735_pushColor(&color ,1);
			}else{
				ST7735_pushColor(&bg ,1);
			}
		}
	}
}
/*
*	v==&v[0]
*	*v==v[0]
*	*(v+n)==v[n]
*/
void drawString(int16_t x, int16_t y, unsigned char *c, int16_t cnt, uint16_t color, uint16_t bg, uint8_t size) 
{

//#define ST7735_height 160

	int i, auxx=x, auxy=y;
	for (i=0; i < cnt; i++) { //falta implementar numero de caracteres na string flexivel
		if(auxx>(ST7735_width-6)){auxy=auxy+10; auxx=0;} //end of horizontal space
		if(auxy>(ST7735_height-10)){auxy=0; auxx=0;} //end of vertical space
		drawChar(auxx, auxy, c[i], color, bg, 0);
		auxx=auxx+6;	
	}
}






