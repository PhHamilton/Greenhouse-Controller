#ifndef __I2C_HANDLER_H__
#define __I2C_HANDLER_H__

#include <stdint.h>

typedef enum
{
    I2C_OK,
    I2C_ERROR,
    I2C_REG_MISMATCH
}i2c_status_t;

typedef struct
{
    int fd;
    uint8_t addr;
    const char *bus;
}i2c_config_t;

int i2c_init(i2c_config_t *conf);
int i2c_write_read_register(i2c_config_t *conf, uint8_t reg, uint16_t value);
int i2c_write_register(i2c_config_t *conf, uint8_t reg, uint16_t value);
int i2c_read_register(i2c_config_t *conf, uint8_t reg, uint16_t *value);

#endif //__I2C_HANDLER_H__
