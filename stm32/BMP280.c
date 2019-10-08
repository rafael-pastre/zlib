/*
 *	Biblioteca do sensor de temperatura e pressao BMP280 para STM32
 *	Versao em C
 *
 *	Feito por: setor de Software Baixo Nivel - Zenith Aerospace
 *	Criado em: 03 out 2019
 */

#include "BMP280.h"

#define READ_REG(i2c_bus, reg_addr, pvalue)\
I2C_read_register(i2c_bus, sensor->i2c_addr, reg_addr, pvalue, BMP280_TIMEOUT)

#define BURST_READ(i2c_bus, start_addr, pvalue, size)\
I2C_burst_read(i2c_bus, sensor->i2c_addr, start_addr, pvalue, size, BMP280_TIMEOUT)

#define WRITE_REG(i2c_bus, reg_addr, value)\
I2C_write_register(i2c_bus, sensor->i2c_addr, reg_addr, value, BMP280_TIMEOUT)

//DONE
HAL_StatusTypeDef BMP280_init(BMP280* sensor, I2C_HandleTypeDef i2c_bus, uint8_t sensor_version, uint8_t sdo){
/*
 *	BMP280* sensor						-- structure that identifies the BMP280 sensor
 *	I2C_HandleTypeDef i2c_bus			-- structure that identifies the i2c bus the sensor uses for communication
 *	uint8_t sensor_version				-- expected version of the sensor (read value must be equal to this)
 */
	uint8_t sended_data[2];				// data sended to BMP280 via i2c bus
	uint8_t received_data[24];			// data received from BMP280 via i2c bus
	HAL_StatusTypeDef status = HAL_OK;	// status of the execution (checks if any error occurs)
	sensor->is_init = FLAG_OFF;			// sets initialization off until the end of this funcion

	/* Checks if sdo value is valid */
	if(sdo != BMP280_SDO_ON && sdo != BMP280_SDO_OFF){
		return HAL_ERROR;
	}
	sensor->i2c_addr = BMP280_I2C_ADDR|sdo;

	/* Checks sensor ID (checks communication) */
	status = READ_REG(i2c_bus, BMP280_REG_ID, received_data);				// Reads value of ID register via I2C
	RETURN_ON_ERROR(status);															// Verifies if I2C communication was successfull
	if(received_data[0] != sensor_version){												// Verifies if the received ID matches the expected ID
		return HAL_ERROR;																// If not, returns error
	}

	/* Performs Software Reset */
	status = WRITE_REG(i2c_bus, BMP280_REG_RESET, BMP280_RESET_VALUE);		// Start software reset via I2C (Writes 0xB6 in RESET register)
	RETURN_ON_ERROR(status);															// Verifies if I2C communication was successfull
	HAL_Delay(100);																		// Wait for reset to finish

	/* Checks if Reset was successfull*/
	status = READ_REG(i2c_bus, BMP280_REG_RESET, received_data);			// Reads value of register RESET via I2C
	RETURN_ON_ERROR(status);															// Verifies if I2C communication was successfull
	if(received_data[0] != 0x00){														// Verifies if the received value of RESET matches the default value
		return HAL_ERROR;																// If not, returns error
	}

	/* Reads calibration data */
	status = BURST_READ(i2c_bus, BMP280_REG_T1_LSB, received_data, 24);		// Reads values in calibration registers via I2C (burst read)
	RETURN_ON_ERROR(status);															// Verifies if I2C communication was successfull

	/* Associates received_data with calibration data*/
	sensor->c.dig_T1 = ((uint16_t)received_data[1]<<8) + (uint16_t)received_data[0];
	sensor->c.dig_T2 = ((uint16_t)received_data[3]<<8) + (uint16_t)received_data[2];
	sensor->c.dig_T3 = ((uint16_t)received_data[5]<<8) + (uint16_t)received_data[4];
	sensor->c.dig_P1 = ((uint16_t)received_data[7]<<8) + (uint16_t)received_data[6];
	sensor->c.dig_P2 = ((uint16_t)received_data[9]<<8) + (uint16_t)received_data[8];
	sensor->c.dig_P3 = ((uint16_t)received_data[11]<<8) + (uint16_t)received_data[10];
	sensor->c.dig_P4 = ((uint16_t)received_data[13]<<8) + (uint16_t)received_data[12];
	sensor->c.dig_P5 = ((uint16_t)received_data[15]<<8) + (uint16_t)received_data[14];
	sensor->c.dig_P6 = ((uint16_t)received_data[17]<<8) + (uint16_t)received_data[16];
	sensor->c.dig_P7 = ((uint16_t)received_data[19]<<8) + (uint16_t)received_data[18];
	sensor->c.dig_P8 = ((uint16_t)received_data[21]<<8) + (uint16_t)received_data[20];
	sensor->c.dig_P9 = ((uint16_t)received_data[23]<<8) + (uint16_t)received_data[22];

	/* Checks if received calibration data is valid */
	for(int i = 0; i < 24; i = i + 2){															// Checks for invalid data
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
HAL_StatusTypeDef BMP280_set_power_mode(BMP280* sensor, uint8_t mode){
	HAL_StatusTypeDef status = HAL_OK;	// status of the execution (checks if any error occurs)

	/* Checks Sensor Initialization */
	if(sensor->is_init == FLAG_OFF){			// Checks if sensor was initialized correctly
		return HAL_ERROR;						// If not, returns error
	}

	/* Checks if mode is valid */
	if(mode > 3){								// Checks if oversampling value is vaid. Must be between 0 and 3
		return HAL_ERROR;						// If not, returns error
	}

	/* Reads Control Measurement Register from sensor */
	status = READ_REG(sensor->i2c_bus, BMP280_REG_CONTROL, &(sensor->ctrl_meas));			// Reads value of register RESET via I2C
	RETURN_ON_ERROR(status);															// Verifies if I2C communica

	/* Calculates value to send to Control Measurement Register */
	sensor->ctrl_meas &= BMP280_MODE_MASK;
	sensor->ctrl_meas |= mode;

	/* Writes in Control Measurement Register */
	status = WRITE_REG(sensor->i2c_bus, BMP280_REG_CONTROL, sensor->ctrl_meas);		// Start software reset via I2C (Writes 0xB6 in RESET register)
	RETURN_ON_ERROR(status);															// Verifies if I2C communication was successfull

	return HAL_OK;
}

//DONE
HAL_StatusTypeDef BMP280_set_filter(BMP280* sensor, uint8_t filter){
	uint8_t received_data = 0xFF;		// data received from BMP280 via i2c bus
	HAL_StatusTypeDef status = HAL_OK;	// status of the execution (checks if any error occurs)

	/* Checks Sensor Initialization */
	if(sensor->is_init == FLAG_OFF){			// Checks if sensor was initialized correctly
		return HAL_ERROR;						// If not, returns error
	}

	/* Checks if mode is valid */
	if(filter > 7){								// Checks if oversampling value is vaid. Must be between 0 and 3
		return HAL_ERROR;						// If not, returns error
	}

	/* Reads Configuration Register from sensor */
	status = READ_REG(sensor->i2c_bus, BMP280_REG_CONFIG, &(sensor->config));			// Reads value of register RESET via I2C
	RETURN_ON_ERROR(status);															// Verifies if I2C communica

	/* Calculates value to send to Configuration Register */
	sensor->config &= BMP280_FILTER_MASK;
	sensor->config |= filter<<2;

	/* Writes in Configuration Register */
	status = WRITE_REG(sensor->i2c_bus, BMP280_REG_CONFIG, sensor->config);		// Start software reset via I2C (Writes 0xB6 in RESET register)
	RETURN_ON_ERROR(status);															// Verifies if I2C communication was successfull

	/* Reads Configuration Register for verification */
	status = READ_REG(sensor->i2c_bus, BMP280_REG_CONFIG, &(received_data));			// Reads value of register RESET via I2C
	RETURN_ON_ERROR(status);															// Verifies if I2C communica
	if(received_data != sensor->config){
		return HAL_ERROR;
	}

	return HAL_OK;
}

//DONE
HAL_StatusTypeDef BMP280_set_standby_time(BMP280* sensor, uint8_t stb_time){
	uint8_t received_data = 0xFF;		// data received from BMP280 via i2c bus
	HAL_StatusTypeDef status = HAL_OK;	// status of the execution (checks if any error occurs)

	/* Checks Sensor Initialization */
	if(sensor->is_init == FLAG_OFF){			// Checks if sensor was initialized correctly
		return HAL_ERROR;						// If not, returns error
	}

	/* Checks if mode is valid */
	if(stb_time > 7){								// Checks if oversampling value is vaid. Must be between 0 and 3
		return HAL_ERROR;						// If not, returns error
	}

	/* Reads Configuration Register from sensor */
	status = READ_REG(sensor->i2c_bus, BMP280_REG_CONFIG, &(sensor->config));			// Reads value of register RESET via I2C
	RETURN_ON_ERROR(status);															// Verifies if I2C communica

	/* Calculates value to send to Configuration Register */
	sensor->config &= BMP280_STB_TIME_MASK;
	sensor->config |= stb_time<<5;

	/* Writes in Configuration Register */
	status = WRITE_REG(sensor->i2c_bus, BMP280_REG_CONFIG, sensor->config);		// Start software reset via I2C (Writes 0xB6 in RESET register)
	RETURN_ON_ERROR(status);															// Verifies if I2C communication was successfull

	/* Reads Configuration Register for verification */
	status = READ_REG(sensor->i2c_bus, BMP280_REG_CONFIG, &(received_data));			// Reads value of register RESET via I2C
	RETURN_ON_ERROR(status);															// Verifies if I2C communica
	if(received_data != sensor->config){
		return HAL_ERROR;
	}

	return HAL_OK;
}

//DONE
HAL_StatusTypeDef BMP280_set_temp_osrs(BMP280* sensor, uint8_t osrs){
	uint8_t received_data = 0xFF;		// data received from BMP280 via i2c bus
	HAL_StatusTypeDef status = HAL_OK;	// status of the execution (checks if any error occurs)

	/* Checks Sensor Initialization */
	if(sensor->is_init == FLAG_OFF){			// Checks if sensor was initialized correctly
		return HAL_ERROR;						// If not, returns error
	}

	/* Checks if mode is valid */
	if(osrs > 7){								// Checks if oversampling value is vaid. Must be between 0 and 3
		return HAL_ERROR;						// If not, returns error
	}

	/* Reads Control Measurement Register from sensor */
	status = READ_REG(sensor->i2c_bus, BMP280_REG_CONTROL, &(sensor->ctrl_meas));			// Reads value of register RESET via I2C
	RETURN_ON_ERROR(status);															// Verifies if I2C communica

	/* Calculates value to send to Control Measurement Register */
	sensor->ctrl_meas &= BMP280_OSRS_T_MASK;
	sensor->ctrl_meas |= osrs<<5;

	/* Writes in Control Measurement Register */
	status = WRITE_REG(sensor->i2c_bus, BMP280_REG_CONTROL, sensor->ctrl_meas);		// Start software reset via I2C (Writes 0xB6 in RESET register)
	RETURN_ON_ERROR(status);															// Verifies if I2C communication was successfull

	/* Reads Configuration Register for verification */
	status = READ_REG(sensor->i2c_bus, BMP280_REG_CONTROL, &(received_data));			// Reads value of register RESET via I2C
	RETURN_ON_ERROR(status);															// Verifies if I2C communica
	if(received_data != sensor->ctrl_meas){
		return HAL_ERROR;
	}

	return HAL_OK;
}

//DONE
HAL_StatusTypeDef BMP280_set_press_osrs(BMP280* sensor, uint8_t osrs){
	uint8_t received_data = 0xFF;		// data received from BMP280 via i2c bus
	HAL_StatusTypeDef status = HAL_OK;	// status of the execution (checks if any error occurs)

	/* Checks Sensor Initialization */
	if(sensor->is_init == FLAG_OFF){			// Checks if sensor was initialized correctly
		return HAL_ERROR;						// If not, returns error
	}

	/* Checks if mode is valid */
	if(osrs > 7){								// Checks if oversampling value is vaid. Must be between 0 and 3
		return HAL_ERROR;						// If not, returns error
	}

	/* Reads Control Measurement Register from sensor */
	status = READ_REG(sensor->i2c_bus, BMP280_REG_CONTROL, &(sensor->ctrl_meas));			// Reads value of register RESET via I2C
	RETURN_ON_ERROR(status);															// Verifies if I2C communica

	/* Calculates value to send to Control Measurement Register */
	sensor->ctrl_meas &= BMP280_OSRS_P_MASK;
	sensor->ctrl_meas |= osrs<<2;

	/* Writes in Control Measurement Register */
	status = WRITE_REG(sensor->i2c_bus, BMP280_REG_CONTROL, sensor->ctrl_meas);		// Start software reset via I2C (Writes 0xB6 in RESET register)
	RETURN_ON_ERROR(status);															// Verifies if I2C communication was successfull

	/* Reads Configuration Register for verification */
	status = READ_REG(sensor->i2c_bus, BMP280_REG_CONTROL, &(received_data));			// Reads value of register RESET via I2C
	RETURN_ON_ERROR(status);															// Verifies if I2C communica
	if(received_data != sensor->ctrl_meas){
		return HAL_ERROR;
	}

	return HAL_OK;
}

HAL_StatusTypeDef BMP280_read(BMP280* sensor){
	uint8_t received_data[24];			// data received from BMP280 via i2c bus
	HAL_StatusTypeDef status = HAL_OK;	// status of the execution (checks if any error occurs)
	/* Checks Sensor Initialization */
	if(sensor->is_init == FLAG_OFF){			// Checks if sensor was initialized correctly
		return HAL_ERROR;						// If not, returns error
	}

	/* Checks if any measurement is in progress */
	status = READ_REG(sensor->i2c_bus, BMP280_REG_STATUS, received_data);
	RETURN_ON_ERROR(status);
	if((received_data[0]&BMP280_MEASURING_MASK) != 0){
		return HAL_ERROR;
	}

	/* Read results */
	status = BURST_READ(sensor->i2c_bus, BMP280_REG_PRESS_MSB,received_data, 6);
	RETURN_ON_ERROR(status);
	sensor->adc_P = ((uint32_t)received_data[0]<<12)+((uint32_t)received_data[1]<<4)+((uint32_t)received_data[2]>>4);
	sensor->adc_T = ((uint32_t)received_data[3]<<12)+((uint32_t)received_data[4]<<4)+((uint32_t)received_data[5]>>4);

	/* Converts Temperature */
	status = BMP280_compensate_temp(sensor);
	RETURN_ON_ERROR(status);
	sensor->temp = (float)sensor->raw_temp/100.0;

	/* Converts Pressure */
	status = BMP280_compensate_press(sensor);
	sensor->press = (float)sensor->raw_press;
	RETURN_ON_ERROR(status);

	/* Calculates Altitude */
	sensor->alt = 44330.0*(1.0 - pow((sensor->press/101325.0), 0.190295));

	return HAL_OK;
}

HAL_StatusTypeDef BMP280_compensate_temp(BMP280* sensor){
	int32_t var1, var2;
	var1 = ((((sensor->adc_T>>3)-((int32_t)sensor->c.dig_T1<<1)))*((int32_t)sensor->c.dig_T2))>>11;
	var2 = (((((sensor->adc_T>>4)-((int32_t)sensor->c.dig_T1))*((sensor->adc_T>>4)-((int32_t)sensor->c.dig_T1)))>>12)*((int32_t)sensor->c.dig_T3))>>14;
	sensor->c.t_fine = var1 + var2;
	sensor->raw_temp = (sensor->c.t_fine*5 + 128)>>8;
	return HAL_OK;
}

HAL_StatusTypeDef BMP280_compensate_press(BMP280* sensor){
	int64_t var1, var2, p;
	var1 = ((int64_t)sensor->c.t_fine)-128000;
	var2 = var1*var1*(int64_t)sensor->c.dig_P6;
	var2 = var2 + ((var1*(int64_t)sensor->c.dig_P5)<<17);
	var2 = var2 + (((int64_t)sensor->c.dig_P4)<<35);
	var1 = ((var1 * var1 * (int64_t)sensor->c.dig_P3)>>8) + ((var1*(int64_t)sensor->c.dig_P2)<<12);
	var1 = (((((int64_t)1)<<47)+var1))*((int64_t)sensor->c.dig_P1)>>33;
	if (var1 == 0){
		return HAL_ERROR; // avoid exception caused by division by zero
	}
	p = 1048576-sensor->adc_P;
	p = (((p<<31)-var2)*3125)/var1;
	var1 = (((int64_t)sensor->c.dig_P9)*(p>>13)*(p>>13))>>25;
	var2 = (((int64_t)sensor->c.dig_P8)*p)>>19;
	p = ((p + var1 + var2)>>8) + (((int64_t)sensor->c.dig_P7)<<4);
	sensor->raw_press = p/256;
	return HAL_OK;
}
#undef READ_REG
#undef BURST_READ
#undef WRITE_REG
