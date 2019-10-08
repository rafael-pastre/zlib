/*
 *	Biblioteca do sensor de temperatura e pressao BMP280 para STM32
 *	Versao em C
 *
 *	Feito por: setor de Software Baixo Nivel - Zenith Aerospace
 *	Criado em: 03 out 2019
 */
/*
 Warning. Holding any interface pin (SDI, SDO, SCK or CSB) at a logical high level when VDDIO is
switched off can permanently damage the device due caused by excessive current flow through
the ESD protection diodes.
*/

#ifndef LIB_ZENITH_BMP280_H
#define LIB_ZENITH_BMP280_H

/* Platform Independent Includes */
#include <math.h>
#include "zstdtypes.h"

/* Platform Specific Includes */
#include "zI2C.h"

/* Communication Timeout */
#define BMP280_TIMEOUT			100		// Time in miliseconds to wait for I2C communication

/* BMP280 I2C Address */
#define BMP280_I2C_ADDR			0xEC	// Address in I2C bus bus with last bit = 0 and sdo = 0

/* Registers Address */
#define BMP280_REG_ID			0xD0	// Version Register
#define BMP280_REG_RESET		0xE0	// Software Reset Register
#define BMP280_REG_STATUS		0xF3	// Measurement Status Register
#define BMP280_REG_CONTROL		0xF4	// Measurement Control Register
#define BMP280_REG_CONFIG		0xF5	// Configuration Register
#define BMP280_REG_PRESS_MSB	0xF7	// Output Pressure Most Significant Byte Register
#define BMP280_REG_PRESS_LSB	0xF8	// Output Pressure Least Significant Byte Register
#define BMP280_REG_PRESS_XLSB	0xF9	// Output Pressure Extra Least Significant Byte Register
#define BMP280_REG_TEMP_MSB		0xFA	// Output Temperature Most Significant Byte Register
#define BMP280_REG_TEMP_LSB		0xFB	// Output Temperature Least Significant Byte Register
#define BMP280_REG_TEMP_XLSB	0xFC	// Output Temperature Extra Least Significant Byte Register

#define BMP280_REG_T1_LSB		0x88	// Calibration Register
#define BMP280_REG_T1_MSB		0x89	// Calibration Register
#define BMP280_REG_T2_LSB		0x8A	// Calibration Register
#define BMP280_REG_T2_MSB		0x8B	// Calibration Register
#define BMP280_REG_T3_LSB		0x8C	// Calibration Register
#define BMP280_REG_T3_MSB		0x8D	// Calibration Register
#define BMP280_REG_P1_LSB		0x8E	// Calibration Register
#define BMP280_REG_P1_MSB		0x8F	// Calibration Register
#define BMP280_REG_P2_LSB		0x90	// Calibration Register
#define BMP280_REG_P2_MSB		0x91	// Calibration Register
#define BMP280_REG_P3_LSB		0x92	// Calibration Register
#define BMP280_REG_P3_MSB		0x93	// Calibration Register
#define BMP280_REG_P4_LSB		0x94	// Calibration Register
#define BMP280_REG_P4_MSB		0x95	// Calibration Register
#define BMP280_REG_P5_LSB		0x96	// Calibration Register
#define BMP280_REG_P5_MSB		0x97	// Calibration Register
#define BMP280_REG_P6_LSB		0x98	// Calibration Register
#define BMP280_REG_P6_MSB		0x99	// Calibration Register
#define BMP280_REG_P7_LSB		0x9A	// Calibration Register
#define BMP280_REG_P7_MSB		0x9B	// Calibration Register
#define BMP280_REG_P8_LSB		0x9C	// Calibration Register
#define BMP280_REG_P8_MSB		0x9D	// Calibration Register
#define BMP280_REG_P9_LSB		0x9E	// Calibration Register
#define BMP280_REG_P9_MSB		0x9F	// Calibration Register

/* Control Values */
#define BMP280_DEFAULT_ID		0x58	// Default ID value of BMP280
#define BMP280_RESET_VALUE		0xB6	// Software Reset Register value for performing software reset
#define BMP280_MEASURING_MASK	0x0F	// Mask for getting 'Start Conversion' bit out of 'Measurement Control'

#define BMP280_SDO_ON			0x02	// Mask that represents SDO pin connected to Vcc
#define BMP280_SDO_OFF			0x00	// Mask that represents SDO pin connected to GND

#define BMP280_MODE_MASK		0xFC	// Mask to reset mode bits
#define BMP280_SLEEP_MODE		0x00	// Mask that represents sensor in sleep mode
#define BMP280_FORCED_MODE		0x01	// Mask that represents sensor in forced mode (single measurement)
#define BMP280_NORMAL_MODE		0x03	// Mask that represents sensor in normal mode (continuous measurement)

#define BMP280_STB_TIME_MASK	0x1F	// Mask to reset standby time bits
#define BMP280_STB_TIME_0d5		0x00	// Mask that represents 0.5 ms standby time
#define BMP280_STB_TIME_62d5	0x01	// Mask that represents 62.5 ms standby time
#define BMP280_STB_TIME_125		0x02	// Mask that represents 125 ms standby time
#define BMP280_STB_TIME_250		0x03	// Mask that represents 250 ms standby time
#define BMP280_STB_TIME_500		0x04	// Mask that represents 500 ms standby time
#define BMP280_STB_TIME_1000	0x05	// Mask that represents 1000 ms standby time
#define BMP280_STB_TIME_2000	0x06	// Mask that represents 2000 ms standby time
#define BMP280_STB_TIME_4000	0x07	// Mask that represents 4000 ms standby time

#define BMP280_FILTER_MASK		0xE3
#define BMP280_FILTER_OFF		0x00
#define BMP280_FILTER_2			0x01
#define BMP280_FILTER_4			0x02
#define BMP280_FILTER_8			0x03
#define BMP280_FILTER_16		0x04

#define BMP280_OSRS_P_MASK		0xE3
#define BMP280_OSRS_T_MASK		0x1F
#define BMP280_OSRS_SKIPPED		0x00
#define BMP280_OSRS_1			0x01
#define BMP280_OSRS_2			0x02
#define BMP280_OSRS_4			0x03
#define BMP280_OSRS_8			0x04
#define BMP280_OSRS_16			0x05

/* Structure for storing Calibration Data */
typedef struct _bmp280_cal_data{
	uint16_t	dig_T1;				// Read from sensor memory
	int16_t		dig_T2;				// Read from sensor memory
	int16_t		dig_T3;				// Read from sensor memory
	uint16_t	dig_P1;				// Read from sensor memory
	int16_t		dig_P2;				// Read from sensor memory
	int16_t		dig_P3;				// Read from sensor memory
	int16_t		dig_P4;				// Read from sensor memory
	int16_t		dig_P5;				// Read from sensor memory
	int16_t		dig_P6;				// Read from sensor memory
	int16_t		dig_P7;				// Read from sensor memory
	int16_t		dig_P8;				// Read from sensor memory
	int16_t		dig_P9;				// Read from sensor memory
	int32_t		t_fine;				// Calculated
}BMP280_cal_data;

/* Structure that represents the BMP280 sensor */
typedef struct _bmp280{
	I2C_HandleTypeDef i2c_bus;			// I2C bus
	BMP280_cal_data	c;					// Calibration Data
	uint8_t i2c_addr;					// Value that store sensor i2c address, depending on whether sdo pin is connected or not
	uint8_t ctrl_meas;					// Value sent to Control Measurement Register
	uint8_t config;						// Value sent to Configuration Register
	int32_t adc_T;						// Unconpensed Temperature
	int32_t adc_P;						// Unconpensed Pressure
	int32_t	raw_temp;					// Temperature in 0.01*C
	int32_t	raw_press;					// Pressure in Pa
	float temp;							// Temperature in *C
	float press;						// Pressure in Pa
	float alt;							// Altitude in meters
	flag_t is_init;						// Flag that stores if initialization was done successfully
}BMP280;

/* Oversampling Modes */
typedef enum _bmp280_oss_modes{
	OSS_ULOW, OSS_LOW, OSS_STD, OSS_HIGH, OSS_UHIGH
}BMP280_oss_modes_t;

/* BMP280 Functions */
HAL_StatusTypeDef BMP280_init(BMP280* sensor, I2C_HandleTypeDef i2c_bus, uint8_t sensor_version, uint8_t sdo);	// can also be used to reset the sensor
HAL_StatusTypeDef BMP280_read_temperature(BMP280* sensor);														// atualizes raw_temp and temp
HAL_StatusTypeDef BMP280_read_pressure(BMP280* sensor, uint8_t oss);											// also reads temperature. oss stands for oversampling, must be between 0 and 3
HAL_StatusTypeDef BMP280_read_altitude(BMP280* sensor);															// this function also reads pressure
HAL_StatusTypeDef BMP280_read(BMP280* sensor);															// this function also reads pressure

HAL_StatusTypeDef BMP280_set_power_mode(BMP280* sensor, uint8_t mode);
HAL_StatusTypeDef BMP280_set_filter(BMP280* sensor, uint8_t filter);
HAL_StatusTypeDef BMP280_set_standby_time(BMP280* sensor, uint8_t stb_time);
HAL_StatusTypeDef BMP280_set_temp_osrs(BMP280* sensor, uint8_t osrs);
HAL_StatusTypeDef BMP280_set_press_osrs(BMP280* sensor, uint8_t osrs);

HAL_StatusTypeDef BMP280_compensate_temp(BMP280* sensor);
HAL_StatusTypeDef BMP280_compensate_press(BMP280* sensor);
#endif//LIB_ZENITH_BMP280_H
