/*Debug:
-Confirmar onde e que o programa enpanca => while (WIP(eepromReadStatus()));
-Rever codigo
-Rever pino do CS

-Checar funcionamento codigo exercicio anterior
*/





#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <spi.h>

uint8_t txbuf[4], rxbuf[4];
uint16_t txbuf16[4], rxbuf16[4];


////-----------------------depois passar todo codigo relativo a eeprom para uma função externa

enum eepromCMD { cmdREAD = 0x03, cmdWRITE = 0x02,
cmdWREN = 0x06, cmdWRDI = 0x04,
cmdRDSR = 0x05, cmdWRSR = 0x01 };

#define WIP(x) ((x) & 1)

void eepromInit();
void eepromWriteEnable(); //ok
void eepromWriteDisable();
uint8_t eepromReadStatus(); //ok
void eepromWriteStatus(uint8_t status);
int eepromWrite(uint8_t *buf, uint8_t cnt, uint16_t offset);
int eepromRead(uint8_t *buf, uint8_t cnt, uint16_t offset);
////-----------------------depois passar todo codigo relativo a eeprom para uma função externa

void main()
{
	int i, j;
//csInit();
	GPIO_InitTypeDef GPIO_InitStructure;
	//Enable Peripheral Clocks
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	//Configure Pins
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

//LEDInit(); optional for this tutorial
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	spiInit(SPI2);

//Loop-back for SPI in 8 bit mode
/*	for (i = 0; i < 8; i++) {
		for (j = 0; j < 4; j++)
			txbuf[j] = i*4 + j;
		GPIO_WriteBit(GPIOC , GPIO_Pin_3 , 0);
		spiReadWrite(SPI2 , rxbuf , txbuf , 4, SPI_SLOW);
		GPIO_WriteBit(GPIOC , GPIO_Pin_3 , 1);
		for (j = 0; j < 4; j++)
			if (rxbuf[j] != txbuf[j])
				assert_failed(__FILE__ , __LINE__);
	}
*/
//Loop-back for SPI in 16 bit mode
/*	for (i = 0; i < 8; i++) {
		for (j = 0; j < 4; j++)
			txbuf16[j] = i*4 + j + (i << 8);
		GPIO_WriteBit(GPIOC , GPIO_Pin_3 , 0);
		spiReadWrite16(SPI2 , rxbuf16 , txbuf16 , 4, SPI_SLOW);
		GPIO_WriteBit(GPIOC , GPIO_Pin_3 , 1);
		for (j = 0; j < 4; j++)
			if (rxbuf16[j] != txbuf16[j])
				assert_failed(__FILE__ , __LINE__);
	}
*/


//New------------------------------------------------------------------------
/*
A ideia é escrever uma coisa na EEPROM, leer essa coisa da EEPROM e confirmar se são iguais


*/






uint8_t *buf;
uint8_t cnt= 2;
uint16_t offset= 0x0002;

uint8_t ola = 0x00;

eepromWrite(buf, cnt, offset);
ola=eepromRead(buf, cnt, offset);

if(ola==0x0A){
	GPIO_WriteBit(GPIOC, GPIO_Pin_9, Bit_SET); //End of execution indicator
}else{

	GPIO_WriteBit(GPIOC, GPIO_Pin_9, Bit_RESET); //End of execution indicator
}

ola=ola+1;



}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{
/*Infinite loop*/
/*Use GDB to find out why we're here*/
}
#endif





////-----------------------depois passar todo codigo relativo a eeprom para uma função externa
uint8_t eepromReadStatus() {
	uint8_t cmd[] = {cmdRDSR , 0xff};
	uint8_t res[2];
	GPIO_WriteBit(GPIOC , GPIO_Pin_3 , 0);
	spiReadWrite(SPI2 , res, cmd, 2, SPI_SLOW);
	GPIO_WriteBit(GPIOC , GPIO_Pin_3 , 1);
	return res[1];
}



void eepromWriteEnable(){
	uint8_t cmd = cmdWREN;
	while (WIP(eepromReadStatus()));
	GPIO_WriteBit(GPIOC , GPIO_Pin_3 , 0);
	spiReadWrite(SPI2 , 0, &cmd, 1, SPI_SLOW);
	GPIO_WriteBit(GPIOC , GPIO_Pin_3 , 1);
}

void eepromInit(){ //??basicamente inicializar o SPI?
}

void eepromWriteDisable(){ //basear-se na função spiwrite enable
}

void eepromWriteStatus(uint8_t status){
}

int eepromWrite(uint8_t *buf, uint8_t cnt, uint16_t offset){ //for now writes only one byte
	uint8_t cmd[] = {cmdWRITE , 0x00, 0x02, 0x0A};	  
//page address begins with xxxx xxxx xxxx 0000 and ends with xxxx xxxx xxxx 1111 => implementar algum metodo de segurança para verificar fim de cada pagina	
	eepromWriteEnable();
	GPIO_WriteBit(GPIOC , GPIO_Pin_3 , 0);
	spiReadWrite(SPI2 , 0, cmd, 4, SPI_SLOW);
	GPIO_WriteBit(GPIOC , GPIO_Pin_3 , 1);
	return 0;
}

int eepromRead(uint8_t *buf, uint8_t cnt, uint16_t offset){
	uint8_t cmd[] = {cmdREAD, 0x00, 0x02};
	uint8_t res[1];
	GPIO_WriteBit(GPIOC , GPIO_Pin_3 , 0);
	spiReadWrite(SPI2 , res, cmd, 4, SPI_SLOW);
	GPIO_WriteBit(GPIOC , GPIO_Pin_3 , 1);
	return res[1];
}

////-----------------------depois passar todo codigo relativo a eeprom para uma função externa
