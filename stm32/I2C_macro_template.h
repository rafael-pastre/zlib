#define READ_REG(i2c_bus, reg_addr, pvalue);\
HAL_StatusTypeDef I2C_read_register(i2c_bus, BMP280_I2C_ADDR, reg_addr, pvalue, BMP280_TIMEOUT);

#define BURST_READ(i2c_bus, start_addr, pvalue, size);\
HAL_StatusTypeDef I2C_burst_read(i2c_bus, BMP280_I2C_ADDR, start_addr, pvalue, size, BMP280_TIMEOUT);

#define WRITE_REG(i2c_bus, reg_addr, value);\
HAL_StatusTypeDef I2C_write_register(i2c_bus, BMP280_I2C_ADDR, reg_addr, value, BMP280_TIMEOUT);

#undef READ_REG
#undef BURST_READ
#undef WRITE_REG