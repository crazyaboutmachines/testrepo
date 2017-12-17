/**
  ******************************************************************************
  * @file	eeprom.h
  * @author	Gerardo de Lima
  * @version 	V0
  * @date   	17-Dez-2017
  * @brief	This file contains all the functions prototypes for the eeprom P.N. 
		25xx160.
  ******************************************************************************
  */

#ifndef EEPROM_H
#define EEPROM_H

#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <spi.h>

#define EEPROM_PORT GPIOC
#define EEPROM_CS GPIO_Pin_3
#define EEPROM_SPEED SPI_SLOW
#define EEPROM_SPI SPI2
#define WIP(x) ((x) & 1)

enum eepromCMD { cmdREAD = 0x03, cmdWRITE = 0x02,
		cmdWREN = 0x06, cmdWRDI = 0x04,
		cmdRDSR = 0x05, cmdWRSR = 0x01 };

void eepromInit();
void eepromWriteEnable();
void eepromWriteDisable();
uint8_t eepromReadStatus();
void eepromWriteStatus(uint8_t status);
int eepromWrite(uint8_t *buf, uint8_t cnt, uint16_t offset);
int eepromRead(uint8_t *buf, uint8_t cnt, uint16_t offset);

#endif

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
