/**
  ******************************************************************************
  * @file	eeprom.c
  * @author	Gerardo de Lima
  * @version	V0
  * @date	17-Dez-2017
  * @brief	This file contains all the functions for the eeprom P.N. 
		25xx160.
  ******************************************************************************
  */

#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <spi.h>
#include <eeprom.h>


/**
  * @}
  */
void eepromInit(){
	//csInit();
	//Configure Pins
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	spiInit(SPI2);
}

/**
  * @}
  */
void eepromWriteEnable(){
	uint8_t cmd = cmdWREN;
	while (WIP(eepromReadStatus()));	//So se pode escrever quando a flag WIP do estatus register for 0
	GPIO_WriteBit(EEPROM_PORT , EEPROM_CS , 0);
	spiReadWrite(EEPROM_SPI , 0, &cmd, 1, EEPROM_SPEED);
	GPIO_WriteBit(EEPROM_PORT , EEPROM_CS , 1);
}

/**
  * @}
  */
void eepromWriteDisable(){ 
	uint8_t cmd = cmdWRDI;
	while (WIP(eepromReadStatus()));	//So se pode escrever quando a flag WIP do estatus register for 0
	GPIO_WriteBit(EEPROM_PORT , EEPROM_CS , 0);
	spiReadWrite(EEPROM_SPI , 0, &cmd, 1, EEPROM_SPEED);
	GPIO_WriteBit(EEPROM_PORT , EEPROM_CS , 1);
}

/**
  * @}
  */
uint8_t eepromReadStatus() {
	uint8_t cmd[] = {cmdRDSR , 0xff};
	uint8_t res[2];
	GPIO_WriteBit(EEPROM_PORT , EEPROM_CS , 0);
	spiReadWrite(EEPROM_SPI , res, cmd, 2, EEPROM_SPEED);
	GPIO_WriteBit(EEPROM_PORT , EEPROM_CS , 1);
	return res[1];
}

/**
  * @}
  */
void eepromWriteStatus(uint8_t status){
	uint8_t cmd[] = {cmdWRSR, status};
	while (WIP(eepromReadStatus()));	//So se pode escrever quando a flag WIP do estatus register for 0
	GPIO_WriteBit(EEPROM_PORT , EEPROM_CS , 0);
	spiReadWrite(EEPROM_SPI , 0, cmd, 2, EEPROM_SPEED);
	GPIO_WriteBit(EEPROM_PORT , EEPROM_CS , 1);
}

/**
  * @brief  Writes on eeprom memory
  * @note   For now writes only one byte without end of page measures.
  * @param  *buf: ponteiro para vector de dados a escrever.
  * @param  cnt: nº de bytes a escrever (tamanho de buf[]). 
  * @param  offset: adress da eprom a escrever.   
  * @retval ? 
  */
int eepromWrite(uint8_t *buf, uint8_t cnt, uint16_t offset){

	uint8_t cmd[19] = {cmdWRITE , (offset>>8), (offset&0x00FF), 0};
	//page: [xxxx xxxx xxxx 0000; xxxx xxxx xxxx 1111] => implementar algum metodo de segurança para 		verificar fim de 	cada pagina ou passar automaticamente para a proxima pagina

	memcpy(&cmd[3],buf,cnt);
	
	eepromWriteEnable();
	GPIO_WriteBit(GPIOC , GPIO_Pin_3 , 0);
	spiReadWrite(SPI2 , 0, cmd, (3+cnt), SPI_SLOW);
	GPIO_WriteBit(GPIOC , GPIO_Pin_3 , 1);
	return 0;
}

/**
  * @}
  */
int eepromRead(uint8_t *buf, uint8_t cnt, uint16_t offset){  // o read nao tem aquele limite de 16 bites??
	uint8_t cmd[] = {cmdREAD, (offset>>8), (offset&0x00FF)};	
	//uint8_t res[(3+cnt)];
	uint8_t res[19]={0,0,0,0}; //implementar um malloc ou assim p isto
	int i;

	memcpy(&res[3],buf,cnt);

	GPIO_WriteBit(GPIOC , GPIO_Pin_3 , 0);
	spiReadWrite(SPI2 , res, cmd, (3+cnt), SPI_SLOW);  //podia-se mandar directamente buf para aqui sem criar o res?
	GPIO_WriteBit(GPIOC , GPIO_Pin_3 , 1);
	//*buf=res[3];	 ou passar isto para um ciclo for
 
	for(i=0;i<cnt;i++){
		*(buf+i)=res[3+i];
	}
	return 0;
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************END OF FILE****/
