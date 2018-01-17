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

static const struct ST7735_cmdBuf initializers[] = {
	{ 0x01, 50, 0, 0},//ok
	{ 0x11, 100, 0, 0},//ok
	{ 0x26, 0, 1, { 0x04}}, //ok
	{ 0xb1, 0, 2, { 0x0b, 0x14 }}, //ok
	{ 0xc0, 0, 2, { 0x08, 0x00 }}, //ok
	{ 0xc1, 0, 1, { 0x05}}, //ok
	{ 0xc5, 0, 2, { 0x41, 0x30 }}, //ok
	{ 0xc7, 0, 1, { 0xc1}}, //ok
	{ 0xEC, 0, 1, { 0x1b}}, //ok
	{ 0x3a, 100, 1, { 0x55 }}, //ok
	{ 0x2a, 0, 4, { 0x00, 0x00, 0x00, 0x7f}}, //ok
	{ 0x2b, 0, 4, { 0x00, 0x00, 0x00, 0x9f}}, //ok
	{ 0x36, 0, 1, { 0xc8}}, //ok
	{ 0xb7, 0, 1, { 0x00}}, //ok
	{ 0xf2, 0, 1, { 0x00}}, //ok
	{ 0xe0, 50, 15, { 0x28, 0x24, 0x22, 0x31, 0x2b, 0x0e, 0x53, 0xa5, 0x42, 0x16, 0x18, 0x12, 0x1a, 0x14, 0x03}}, //ok
	{ 0xe1, 50, 15, { 0x17, 0x1b, 0x1d, 0x0e, 0x14, 0x11, 0x2c, 0xa5, 0x3d, 0x09, 0x27, 0x2d, 0x25, 0x2b, 0x3c}}, //ok
	{ 0x13, 10, 0, 0}, //ok
	{ 0x29, 255, 0, 0} //ok
};

//P.S. tentar confirmar ordem dos argumentos
//O CASET/RASET do codigo arduino estao errados?
//tentar usar uma funsao ainda mais simples do que o fillscreen
//procurar no gogle por algo tipo arduino LCD horizontal lines problem
static const struct ST7735_cmdBuf initializers_old[] = {
	{ 0x01, 150, 0, 0}, //ok
	{ 0x11, 255, 0, 0}, //ok
	{ 0xB1, 0, 3, { 0x01, 0x2C, 0x2D }},//ok
	{ 0xB2, 0, 3, { 0x01, 0x2C, 0x2D }}, //not used
	{ 0xB3, 0, 6, { 0x01, 0x2C, 0x2D, 0x01, 0x2C, 0x2D }}, //not used
	{ 0xB4, 0, 1, { 0x07 }},//ok
	{ 0xC0, 0, 3, { 0xA2, 0x02, 0x84}},//ok
	{ 0xC1, 0, 1, { 0xC5}}, //ok
	{ 0xC2, 0, 2, { 0x0A, 0x00 }}, //ok
	{ 0xC3, 0, 2, { 0x8A, 0x2A}},  //not used
	{ 0xC4, 0, 2, { 0x8A, 0xEE}},   //not used
	{ 0xC5, 0, 1, { 0x0E }}, //ok
	{ 0x20, 0, 0, 0}, //not used
	{ ST7735_MADCTL , 0, 1, {0xC8}},  //ST7735_MADCTL==0x36   ok //arg=={MADVAL(MADCTLGRAPHICS)}
	{ ST7735_COLMOD , 0, 1, {0x05}}, //ST7735_COLMOD==0x3A   ok

//green tab???
	{ ST7735_CASET , 0, 4, {0x00, 0x00, 0x00, 0x7F }}, //ST7735_CASET==0x2A   ok
	{ ST7735_RASET , 0, 4, {0x00, 0x00, 0x00, 0x9F }}, //ST7735_RASET==0x2B   ok
//green tab???
	{ 0xE0, 0, 16, {0x02, 0x1C, 0x07, 0x12, 0x37, 0x32, 0x29, 0x2D, 0x29, 0x25, 0x2B, 0x39, 0x00, 0x01, 0x03, 0x10 }}, //ok
	{ 0xE1, 0, 16, {0x03, 0x1d, 0x07, 0x06, 0x2E, 0x2C, 0x29, 0x2D, 0x2E, 0x2E, 0x37, 0x3F, 0x00, 0x00, 0x02, 0x10 }}, //ok
	{ 0x13, 10, 0, 0}, //ok
	{ 0x29, 100, 0, 0}  //ok
};


static const struct ST7735_cmdBuf initializers_old2[] = {
	// SWRESET Software reset
	{ 0x01, 150, 0, 0}, //ok
	// SLPOUT Leave sleep mode
	{ 0x11, 150, 0, 0}, //ok
	// FRMCTR1 , FRMCTR2 Frame Rate configuration -- Normal mode , idle
	// frame rate = fosc / (1 x 2 + 40) * (LINE + 2C + 2D)
	{ 0xB1, 0, 3, { 0x01, 0x2C, 0x2D }},//ok
	{ 0xB2, 0, 3, { 0x01, 0x2C, 0x2D }}, //not used
	// FRMCTR3 Frame Rate configureation -- partial mode
	{ 0xB3, 0, 6, { 0x01, 0x2C, 0x2D, 0x01, 0x2C, 0x2D }}, //not used
	// INVCTR Display inversion (no inversion)
	{ 0xB4, 0, 1, { 0x07 }},//ok
	// PWCTR1 Power control -4.6V, Auto mode
	{ 0xC0, 0, 3, { 0xA2, 0x02, 0x84}},//ok
	// PWCTR2 Power control VGH25 2.4C, VGSEL -10, VGH = 3 * AVDD
	{ 0xC1, 0, 1, { 0xC5}}, //ok
	// PWCTR3 Power control , opamp current smal , boost frequency
	{ 0xC2, 0, 2, { 0x0A, 0x00 }}, //ok
	// PWCTR4 Power control , BLK/2, opamp current small and medium low
	{ 0xC3, 0, 2, { 0x8A, 0x2A}},  //not used
	// PWRCTR5 , VMCTR1 Power control
	{ 0xC4, 0, 2, { 0x8A, 0xEE}},   //not used
	{ 0xC5, 0, 1, { 0x0E }}, //ok
	// INVOFF Don't invert display
	{ 0x20, 0, 0, 0}, //not used
	// Memory access directions. row address/col address , bottom to top refesh (10.1.27)
	{ ST7735_MADCTL , 0, 1, {MADVAL(MADCTLGRAPHICS)}},  //ST7735_MADCTL==0x36   ok
	// Color mode 16 bit (10.1.30
	{ ST7735_COLMOD , 0, 1, {0x05}}, //ST7735_COLMOD==0x3A   ok
	// Column address set 0..127
	{ ST7735_CASET , 0, 4, {0x00, 0x00, 0x00, 0x7F }}, //ST7735_CASET==0x2A   ok
	// Row address set 0..159
	{ ST7735_RASET , 0, 4, {0x00, 0x00, 0x00, 0x9F }}, //ST7735_RASET==0x2B   ok
	// GMCTRP1 Gamma correction
	{ 0xE0, 0, 16, {0x02, 0x1C, 0x07, 0x12, 0x37, 0x32, 0x29, 0x2D, 0x29, 0x25, 0x2B, 0x39, 0x00, 0x01, 0x03, 0x10 }}, //ok
	// GMCTRP2 Gamma Polarity corrction
	{ 0xE1, 0, 16, {0x03, 0x1d, 0x07, 0x06, 0x2E, 0x2C, 0x29, 0x2D, 0x2E, 0x2E, 0x37, 0x3F, 0x00, 0x00, 0x02, 0x10 }}, //ok
	// DISPON Display on
	{ 0x29, 100, 0, 0},  //ok
	// NORON Normal on
	{ 0x13, 10, 0, 0}, //ok
	// End
	{ 0, 0, 0, 0} //not used
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
