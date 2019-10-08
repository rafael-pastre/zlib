/*
 *	Biblioteca do sensor de temperatura e pressao BMP180 para STM32
 *	Versao em C
 *
 *	Feito por: setor de Software Baixo Nivel - Zenith Aerospace
 *	Criado em: 27 ago 2019
 */

#ifndef LIB_ZENITH_BMP180_H
#define LIB_ZENITH_BMP180_H

/* Platform Independent Includes */
#include <math.h>
#include "zstdtypes.h"

/* Platform Specific Includes */
#include "zI2C.h"

/* Communication Timeout */
#define BMP180_TIMEOUT			100		// Time in miliseconds to wait for I2C communication

/* BMP180 I2C Address */
#define BMP180_I2C_ADDR			0xEE	// Address in I2C bus with last bit = 0

/* Registers Address */
#define BMP180_REG_ID			0xD0	// Version Register
#define BMP180_REG_RESET		0xE0	// Software Reset Register
#define BMP180_REG_CONTROL		0xF4	// Measurement Control Register
#define BMP180_REG_OUT_MSB		0xF6	// Output Most Significant Byte Register
#define BMP180_REG_OUT_LSB		0xF7	// Output Least Significant Byte Register
#define BMP180_REG_OUT_XLSB		0xF8	// Output Extra Least Significant Byte Register
#define BMP180_REG_AC1_MSB		0xAA	// Calibration Register
#define BMP180_REG_AC1_LSB		0xAB	// Calibration Register
#define BMP180_REG_AC2_MSB		0xAC	// Calibration Register
#define BMP180_REG_AC2_LSB		0xAD	// Calibration Register
#define BMP180_REG_AC3_MSB		0xAE	// Calibration Register
#define BMP180_REG_AC3_LSB		0xAF	// Calibration Register
#define BMP180_REG_AC4_MSB		0xB0	// Calibration Register
#define BMP180_REG_AC4_LSB		0xB1	// Calibration Register
#define BMP180_REG_AC5_MSB		0xB2	// Calibration Register
#define BMP180_REG_AC5_LSB		0xB3	// Calibration Register
#define BMP180_REG_AC6_MSB		0xB4	// Calibration Register
#define BMP180_REG_AC6_LSB		0xB5	// Calibration Register
#define BMP180_REG_B1_MSB		0xB6	// Calibration Register
#define BMP180_REG_B1_LSB		0xB7	// Calibration Register
#define BMP180_REG_B2_MSB		0xB8	// Calibration Register
#define BMP180_REG_B2_LSB		0xB9	// Calibration Register
#define BMP180_REG_MB_MSB		0xBA	// Calibration Register
#define BMP180_REG_MB_LSB		0xBB	// Calibration Register
#define BMP180_REG_MC_MSB		0xBC	// Calibration Register
#define BMP180_REG_MC_LSB		0xBD	// Calibration Register
#define BMP180_REG_MD_MSB		0xBE	// Calibration Register
#define BMP180_REG_MD_LSB		0xBF	// Calibration Register

/* Control Values */
#define BMP180_DEFAULT_ID		0x55	// Default ID value of BMP180
#define BMP180_READ_TMP			0x2E	// Control Register value for reading temperature
#define BMP180_READ_PRES_0		0x34	// Control Register value for reading pressure with oversampling 0
#define BMP180_READ_PRES_1		0x74	// Control Register value for reading pressure with oversampling 1
#define BMP180_READ_PRES_2		0xB4	// Control Register value for reading pressure with oversampling 2
#define BMP180_READ_PRES_3		0xF4	// Control Register value for reading pressure with oversampling 3
#define BMP180_RESET_VALUE		0xB6	// Software Reset Register value for performing software reset
#define BMP180_SCO_MASK			0x20	// Mask for getting 'Start Conversion' bit out of 'Measurement Control'

/* Structure for storeing Calibration Data */
typedef struct _bmp180_cal_data{
	uint16_t	AC1;				// Read from sensor memory
	uint16_t	AC2;				// Read from sensor memory
	uint16_t	AC3;				// Read from sensor memory
	uint16_t	AC4;				// Read from sensor memory
	uint16_t	AC5;				// Read from sensor memory
	uint16_t	AC6;				// Read from sensor memory
	uint16_t	B1;					// Read from sensor memory
	uint16_t	B2;					// Read from sensor memory
	int32_t		B3;					// Calculated
	uint32_t	B4;					// Calculated
	int32_t		B5;					// Calculated
	int32_t		B6;					// Calculated
	uint32_t	B7;					// Calculated
	uint16_t	MB;					// Read from sensor memory
	uint16_t	MC;					// Read from sensor memory
	uint16_t	MD;					// Read from sensor memory
	int32_t		X1;					// Calculated
	int32_t		X2;					// Calculated
	int32_t		X3;					// Calculated
}BMP180_cal_data;

/* Structure that represents the BMP180 sensor */
typedef struct _bmp180{
	I2C_HandleTypeDef i2c_bus;			// I2C bus
	BMP180_cal_data	c;					// Calibration Data
	int32_t UT;							// Unconpensed Temperature
	int32_t UP;							// Unconpensed Pressure
	int32_t	raw_temp;					// Temperature in 0.1*C
	int32_t	raw_press;					// Pressure in Pa
	float temp;							// Temperature in *C
	float press;						// Pressure in Pa
	float alt;							// Altitude in meters
	flag_t is_init = FLAG_OFF;			// Flag that stores if initialization was done successfully
}BMP180;



/* BMP180 Functions */
HAL_StatusTypeDef BMP180_init(BMP180* sensor, I2C_HandleTypeDef i2c_bus, uint8_t sensor_version);				// can also be used to reset the sensor
HAL_StatusTypeDef BMP180_read_temperature(BMP180* sensor);														// atualizes raw_temp and temp
HAL_StatusTypeDef BMP180_read_pressure(BMP180* sensor, uint8_t oss);											// also reads temperature. oss stands for oversampling, must be between 0 and 3
HAL_StatusTypeDef BMP180_read_altitude(BMP180* sensor);															// this function also reads pressure

#endif//LIB_ZENITH_BMP180_H