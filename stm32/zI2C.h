/*
 *	Biblioteca do protocolo de comunicacao I2C para STM32
 *	Versao em C
 *
 *	Feito por: setor de Software Baixo Nivel - Zenith Aerospace
 *	Criado em: 02 out 2019
 */

#ifndef LIB_ZENITH_I2C_H
#define LIB_ZENITH_I2C_H

/* Platform Specific Includes */
#include "zstm32.h"

/* I2C Macro Functions */
#define I2C_ADDR_R(addr) (addr|0b00000001)
#define I2C_ADDR_W(addr) (addr&0b11111110)

/* I2C Functions */
HAL_StatusTypeDef I2C_read_register(I2C_HandleTypeDef i2c_bus, uint8_t i2c_addr, uint8_t reg_addr, uint8_t* pvalue, uint32_t timeout);					// received data is stored in value
HAL_StatusTypeDef I2C_burst_read(I2C_HandleTypeDef i2c_bus, uint8_t i2c_addr, uint8_t start_addr, uint8_t* pvalue, uint16_t size, uint32_t timeout);	// reads n registers
HAL_StatusTypeDef I2C_write_register(I2C_HandleTypeDef i2c_bus, uint8_t i2c_addr, uint8_t reg_addr, uint8_t value, uint32_t timeout);					// writes in register

#endif//LIB_ZENITH_I2C_H
