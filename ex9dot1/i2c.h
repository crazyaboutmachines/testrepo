#ifndef I2C_H
#define I2C_H

#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_i2c.h>

#define Timed(x) Timeout = 0xFFFF; while (x){ if (Timeout -- == 0) goto errReturn;}

enum Status { Success = 0, Error = -1 }; //dps checkar se esta linha é valida
typedef int Status; //dps checkar se esta linha é valida

void I2C_LowLevel_Init(I2C_TypeDef* I2Cx , int ClockSpeed , int OwnAddress);
Status I2C_Write(I2C_TypeDef* I2Cx , const uint8_t* buf, uint32_t nbyte , uint8_t SlaveAddress);
Status I2C_Read(I2C_TypeDef* I2Cx , uint8_t *buf, uint32_t nbyte , uint8_t SlaveAddress);

#endif
