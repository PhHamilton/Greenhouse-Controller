#ifndef __INA219_H__
#define __INA219_H__

#include <math.h>
#include <unistd.h>
#include <stdint.h>
#include "i2c_handler.h"

#define INA219_ADDRESS_1 0x40

typedef struct {
    float shunt_resistance;
    float max_current;
    i2c_config_t i2c_conf;
    float current_lsb;
    float power_lsb;
} INA219_config_t;

typedef struct {
    float voltage; // in volts
    float current; // in amperes
    float power;   // in watts
} INA219_data_t;

typedef struct
{
    INA219_config_t config;
    INA219_data_t data;
}INA219_t;

typedef enum
{
    INA219_OK = 0,
    INA219_ERROR_I2C = -1,
    INA219_WARNING_OLD_MEASUREMENT = -2,
    INA219_WARNING_OVERFLOW = -3,
    INA219_ERROR = -4
}INA219_STATUS_t;

INA219_STATUS_t ina219_init(INA219_config_t *conf);
INA219_STATUS_t ina219_read(INA219_config_t *conf, INA219_data_t *data);

#endif //__INA219_H__
