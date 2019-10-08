/*
 *	Biblioteca do protocolo de comunicacao I2C para STM32
 *	Versao em C
 *
 *	Feito por: setor de Software Baixo Nivel - Zenith Aerospace
 *	Criado em: 02 out 2019
 */
 
#include "zI2C.h"

//DONE
HAL_StatusTypeDef I2C_read_register(I2C_HandleTypeDef i2c_bus, uint8_t i2c_addr, uint8_t reg_addr, uint8_t* pvalue, uint32_t timeout){
	uint8_t sended_data[1];																			// data sended to BMP180 via i2c bus
	HAL_StatusTypeDef status = HAL_OK;																// status of the execution (checks if any error occurs)
	
	/* Writes Register Address in I2C */
	sended_data[0] = reg_addr;																		// Register Address
	status = HAL_I2C_Master_Transmit(&i2c_bus, I2C_ADDR_W(i2c_addr), sended_data, 1, timeout);		// Request value of Register register via I2C
	RETURN_ON_ERROR(status);																		// Verifies if I2C communication was successfull
	
	/* Reads Register Value from I2C */
	status = HAL_I2C_Master_Receive(&i2c_bus, I2C_ADDR_R(i2c_addr), pvalue, 1, timeout);			// Receive Register Value via I2C
	return status;																					// Any lefting error is returned
}

//DONE
HAL_StatusTypeDef I2C_burst_read(I2C_HandleTypeDef i2c_bus, uint8_t i2c_addr, uint8_t start_addr, uint8_t* pvalue, uint16_t size, uint32_t timeout){
	uint8_t sended_data[1];																			// data sended to BMP180 via i2c bus
	HAL_StatusTypeDef status = HAL_OK;																// status of the execution (checks if any error occurs)
	
	/* Writes Register Address in I2C */
	sended_data[0] = start_addr;																	// First Register Address
	status = HAL_I2C_Master_Transmit(&i2c_bus, I2C_ADDR_W(i2c_addr), sended_data, 1, timeout);		// Request Registers Data via I2C
	RETURN_ON_ERROR(status);																		// Verifies if I2C communication was successfull
	
	/* Reads Register Value from I2C */
	status = HAL_I2C_Master_Receive(&i2c_bus, I2C_ADDR_R(i2c_addr), pvalue, size, timeout);			// Receive Registers Values via I2C
	return status;																					// Any lefting error is returned
}

//DONE
HAL_StatusTypeDef I2C_write_register(I2C_HandleTypeDef i2c_bus, uint8_t i2c_addr, uint8_t reg_addr, uint8_t value, uint32_t timeout){
	uint8_t sended_data[2];																			// data sended to BMP180 via i2c bus
	
	/* Writes Register Address in I2C */
	sended_data[0] = reg_addr;																		// Address of Register
	sended_data[1] = value;																			// Value to write in register
	return HAL_I2C_Master_Transmit(&i2c_bus, I2C_ADDR_W(i2c_addr), sended_data, 2, timeout);		// Writes in register. Any lefting error is returned
}
