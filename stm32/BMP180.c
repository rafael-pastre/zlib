/*
 *	Biblioteca do sensor de temperatura e pressao BMP180 para STM32
 *	Versao em C
 *
 *	Feito por: setor de Software Baixo Nivel - Zenith Aerospace
 *	Criado em: 27 ago 2019
 */

#include "BMP180.h"

#define READ_REG(i2c_bus, reg_addr, pvalue);\
HAL_StatusTypeDef I2C_read_register(i2c_bus, BMP180_I2C_ADDR, reg_addr, pvalue, BMP180_TIMEOUT);

#define BURST_READ(i2c_bus, start_addr, pvalue, size);\
HAL_StatusTypeDef I2C_burst_read(i2c_bus, BMP180_I2C_ADDR, start_addr, pvalue, size, BMP180_TIMEOUT);

#define WRITE_REG(i2c_bus, reg_addr, value);\
HAL_StatusTypeDef I2C_write_register(i2c_bus, BMP180_I2C_ADDR, reg_addr, value, BMP180_TIMEOUT);

//DONE
HAL_StatusTypeDef BMP180_init(BMP180* sensor, I2C_HandleTypeDef i2c_bus, uint8_t sensor_version){
/*
 *	BMP180* sensor						-- structure that identifies the BMP180 sensor
 *	I2C_HandleTypeDef i2c_bus			-- structure that identifies the i2c bus the sensor uses for communication
 *	uint8_t sensor_version				-- expected version of the sensor (read value must be equal to this)
 */
	uint8_t sended_data[2];				// data sended to BMP180 via i2c bus
	uint8_t received_data[22];			// data received from BMP180 via i2c bus
	HAL_StatusTypeDef status = HAL_OK;	// status of the execution (checks if any error occurs)
	sensor->is_init = FLAG_OFF;			// sets initialization off until the end of this funcion
	
	/* Checks sensor ID (checks communication) */
	status = READ_REG(i2c_bus, BMP180_REG_ID, received_data);					// Reads value of ID register via I2C
	RETURN_ON_ERROR(status);															// Verifies if I2C communication was successfull
	if(received_data[0] != sensor_version){												// Verifies if the received ID matches the expected ID
		return HAL_ERROR;																// If not, returns error
	}
	
	/* Performs Software Reset */
	status = I2C_write_register(i2c_bus, BMP180_REG_RESET, BMP180_RESET_VALUE);			// Start software reset via I2C (Writes 0xB6 in RESET register)
	RETURN_ON_ERROR(status);															// Verifies if I2C communication was successfull
	HAL_Delay(100);																		// Wait for reset to finish
	
	/* Checks if Reset was successfull*/
	status = READ_REG(i2c_bus, BMP180_REG_RESET, received_data);				// Reads value of register RESET via I2C
	RETURN_ON_ERROR(status);															// Verifies if I2C communication was successfull
	if(received_data[0] != 0x00){														// Verifies if the received value of RESET matches the default value
		return HAL_ERROR;																// If not, returns error
	}
	
	/* Reads calibration data */
	status = I2C_burst_read(i2c_bus, BMP180_REG_AC1_MSB, received_data, 22);		// Reads values in calibration registers via I2C (burst read)
	RETURN_ON_ERROR(status);															// Verifies if I2C communication was successfull
	
	/* Associates received_data with c*/
	sensor->c.AC1 = received_data[0] + received_data[1]<<8;
	sensor->c.AC2 = received_data[2] + received_data[3]<<8;
	sensor->c.AC3 = received_data[4] + received_data[5]<<8;
	sensor->c.AC4 = received_data[6] + received_data[7]<<8;
	sensor->c.AC5 = received_data[8] + received_data[9]<<8;
	sensor->c.AC6 = received_data[10] + received_data[11]<<8;
	sensor->c.B1 = received_data[12] + received_data[13]<<8;
	sensor->c.B2 = received_data[14] + received_data[15]<<8;
	sensor->c.MB = received_data[16] + received_data[17]<<8;
	sensor->c.MC = received_data[18] + received_data[19]<<8;
	sensor->c.MD = received_data[20] + received_data[21]<<8;
	
	/* Checks if received calibration data is valid */
	for(int i = 0; i < 22; i = i + 2){															// Checks for invalid data
		if(((uint16_t*)received_data)[i] == 0 ||((uint16_t*)received_data)[i] == 0xFFFF){		// Values 0x0000 and 0xFFFF means invalid data
			return HAL_ERROR;																	// Returns error
		}
	}
	/*
	if(																					// Checks for invalid data
		sensor->c.AC1 == 0 || sensor->c.AC1 == 0xFFFF ||					// Values 0x0000 and 0xFFFF means invalid data
		sensor->c.AC2 == 0 || sensor->c.AC2 == 0xFFFF ||
		sensor->c.AC3 == 0 || sensor->c.AC3 == 0xFFFF ||
		sensor->c.AC4 == 0 || sensor->c.AC4 == 0xFFFF ||
		sensor->c.AC5 == 0 || sensor->c.AC5 == 0xFFFF ||
		sensor->c.AC6 == 0 || sensor->c.AC6 == 0xFFFF ||
		sensor->c.B1  == 0 || sensor->c.B1  == 0xFFFF	||
		sensor->c.B2  == 0 || sensor->c.B2  == 0xFFFF	||
		sensor->c.MB  == 0 || sensor->c.MB  == 0xFFFF ||
		sensor->c.MC  == 0 || sensor->c.MC  == 0xFFFF	||
		sensor->c.MD  == 0 || sensor->c.MD  == 0xFFFF
	)
	{																						// if exists invalid calibration data
		return HAL_ERROR;																	// Returns error
	}
	*/
	
	/* Exit with successfull initialization */
	sensor->i2c_bus = i2c_bus;					// Sets I2C_HandleTypeDef used for sensor communication
	sensor->is_init = FLAG_ON;					// Marks successfull initialization
	return HAL_OK;
}

//DONE
HAL_StatusTypeDef BMP180_read_temperature(BMP180* sensor){
/*
 *	BMP180* sensor						-- structure that identifies the BMP180 sensor
 */
	uint8_t received_data[2];			// data received from BMP180 via i2c bus
	HAL_StatusTypeDef status = HAL_OK;	// status of the execution (checks if any error occurs)
	
	//DONE
	/* Checks Sensor Initialization */
	if(sensor->is_init == FLAG_OFF){			// Checks if sensor was initialized correctly
		return HAL_ERROR;						// If not, returns error
	}
	
	//DONE
	/* Checks Measurement in progress */
	status = I2C_read_register(&(sensor->i2c_bus), BMP180_REG_CONTROL, received_data);	// Reads value of Control Measurement register via I2C
	RETURN_ON_ERROR(status);																// Verifies if I2C communication was successfull
	if(received_data[0]&BMP180_SCO_MASK != 0){												// Verifies if any measurement are in progress (Sco = 1)
		return HAL_ERROR;																	// If yes, returns error
	}
	
	//DONE
	/* Starts Measurement */
	status = I2C_write_register(&(sensor->i2c_bus), BMP180_REG_CONTROL, BMP180_READ_TMP);// Start Temperature Measurement via I2C (Writes 0x2E in Ctrl register)
	RETURN_ON_ERROR(status);																// Verifies if I2C communication was successfull
	
	//DONE
	/* Reads Unconpensed Value */
	status = I2C_burst_read(&(sensor->i2c_bus), BMP180_REG_OUT_MSB, received_data, 2);	// Reads values in Output register via I2C (burst read)
	RETURN_ON_ERROR(status);																// Verifies if I2C communication was successfull
	UT = received_data[0] + received_data[1]<<8;											// Gets Unconpensed Temperature form received data

	//DONE
	/* Calculates Temperature */
	sensor->c.X1 = (sensor->c.UT - sensor->c.AC6)*sensor->c.AC5;							// Formula from Datasheet
	sensor->c.X1 = sensor->c.X1 >> 15;
	sensor->c.X2 = sensor->c.MC << 11;
	sensor->c.X2 /= (sensor->c.X1 + sensor->c.MD);
	sensor->c.B5 = sensor->c.X1 + sensor->c.X2;
	sensor->raw_temp = (sensor->c.B5 + 8)>>4;
	sensor->temp = (float)(sensor->raw_temp/10.0);
	
	return HAL_OK;
}


//DONE
HAL_StatusTypeDef BMP180_read_pressure(BMP180* sensor, uint8_t oss){
/*
 *	BMP180* sensor						-- structure that identifies the BMP180 sensor
 *	uint8_t oss							-- oversampling value (determines the precision of measurement)
 */
	uint8_t sended_data[2];				// data sended to BMP180 via i2c bus
	uint8_t received_data[2];			// data received from BMP180 via i2c bus
	HAL_StatusTypeDef status = HAL_OK;	// status of the execution (checks if any error occurs)
	
	
	//DONE
	/* Checks Sensor Initialization */
	if(sensor->is_init == FLAG_OFF){			// Checks if sensor was initialized correctly
		return HAL_ERROR;						// If not, returns error
	}

	//DONE
	/* Checks Oversampling */
	if(oss > 3){								// Checks if oversampling value is vaid. Must be between 0 and 3
		return HAL_ERROR;						// If not, returns error
	}

	//DONE
	/* Reads temperature */
	status = BMP180_read_temperature(sensor);	// Reads temperature via I2C
	RETURN_ON_ERROR(status);					// Returns if temperature was not read successfully
	
	//DONE
	/* Checks Measurement in progress */
	status = I2C_read_register(&(sensor->i2c_bus), BMP180_REG_CONTROL, received_data);	// Reads value of Control Measurement register via I2C
	RETURN_ON_ERROR(status);																// Verifies if I2C communication was successfull
	if(received_data[0]&BMP180_SCO_MASK != 0){												// Verifies if any measurement are in progress (Sco = 1)
		return HAL_ERROR;																	// If yes, returns error
	}
	
	//DONE
	/* Starts Measurement */
	sended_data[0] = BMP180_REG_CONTROL;															// Address of Control Measurement Register
	if(oss == 0){																					// if oss == 0
		sended_data[1] = BMP180_READ_TMP;															// Value to start Pressure Measurement with oss = 0
	}
	else if(oss == 1){																				// if oss == 1
		sended_data[1] = BMP180_READ_TMP;															// Value to start Pressure Measurement with oss = 1
	}
	else if(oss == 2){																				// if oss == 2
		sended_data[1] = BMP180_READ_TMP;															// Value to start Pressure Measurement with oss = 2
	}
	else{																							// if oss == 3
		sended_data[1] = BMP180_READ_TMP;															// Value to start Pressure Measurement with oss = 3
	}
	status = HAL_I2C_Master_Transmit(&(sensor->i2c_bus), BMP180_I2C_ADDR_W, sended_data, 2, 100);	// Start Temperature Measurement via I2C (Writes 0x2E in Ctrl register)
	if(status != HAL_OK){																			// Verifies if I2C communication was successfull
		return status;																				// If not, returns error
	}
	if(oss == 0){																					// if oss == 0
		HAL_Delay(5);																				// Wait for Pressure Measurement to finish
	}
	else if(oss == 1){																				// if oss == 1
		HAL_Delay(8);																				// Wait for Pressure Measurement to finish
	}
	else if(oss == 2){																				// if oss == 2
		HAL_Delay(14);																				// Wait for Pressure Measurement to finish
	}
	else{																							// if oss == 3
		HAL_Delay(26);																				// Wait for Pressure Measurement to finish
	}
	
	
	//DONE
	/* Reads Unconpensed Value */
	status = I2C_burst_read(&(sensor->i2c_bus), BMP180_REG_OUT_MSB, received_data, 2);	// Reads values in Output register via I2C (burst read)
	RETURN_ON_ERROR(status);																// Verifies if I2C communication was successfull
	UP = received_data[0] + received_data[1]<<8;											// Gets Unconpensed Pressure form received data
	
	
	
	/* Calculates Pressure */
	sensor->c.B6 = sensor->c.B5 - 4000;														// Formula from Datasheet
	sensor->c.X1 = (sensor->c.B6*sensor->c.B6)>>12;
	sensor->c.X1 = (sensor->c.B2*sensor->c.X1)>>11;
	sensor->c.X2 = (sensor->c.AC2*sensor->c.B6)>>11;
	sensor->c.X3 = sensor->c.X1 + sensor->c.X2;
	sensor->c.B3 = (sensor->c.AC1*4 + sensor->c.X3)<<oss;
	sensor->c.B3 = (sensor->c.B3 + 2)/4;
	sensor->c.X1 = (sensor->c.AC3*sensor->c.B6)>>13;
	sensor->c.X2 = (sensor->c.B6*sensor->c.B6)>>12;
	sensor->c.X2 = (sensor->c.B1*sensor->c.X2)>>16;
	sensor->c.X3 = ((sensor->c.X1 + sensor->c.X2)+2)>>2;
	sensor->c.B4 = sensor->c.AC4*(uint32_t)(sensor->c.X3+32768)>>15;
	sensor->c.B7 = ((uint32_t)sensor->c.UP - sensor->c.B3)*(50000>>oss);
	if(sensor->c.B7 < 0x80000000){
		sensor->raw_press = (sensor->c.B7<<1)/sensor->c.B4;
	}
	else{
		sensor->raw_press = (sensor->c.B7/sensor->c.B4)<<1;
	}
	sensor->c.X1 = (sensor->raw_press>>8)*(sensor->raw_press>>8);
	sensor->c.X1 = (sensor->c.X1*3038)>>16;
	sensor->c.X2 = (-7357*sensor->raw_press)>>16;
	sensor->raw_press += (sensor->c.X1+sensor->c.X2+3791)>>4;
	sensor->press = sensor->raw_press;
	
	return HAL_OK;
}


//DONE
HAL_StatusTypeDef BMP180_read_altitude(BMP180* sensor, uint8_t oss){
	HAL_StatusTypeDef status = HAL_OK;	// status of the execution (checks if any error occurs)
	
	
	//DONE
	/* Checks Sensor Initialization */
	if(sensor->is_init == FLAG_OFF){			// Checks if sensor was initialized correctly
		return HAL_ERROR;						// If not, returns error
	}
	
	//DONE
	/* Reads pressure */
	status = BMP180_read_pressure(sensor, oss);	// Reads pressure via I2C
	RETURN_ON_ERROR(status);					// Returns if pressure was not read correctly

	//DONE
	/* Calculates altitude */
	sensor->alt = 44330.0*(1.0 - pow((sensor->press/101325.0), 0.190295));
	return HAL_OK;
}