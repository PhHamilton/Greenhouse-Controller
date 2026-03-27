#ifndef __SHT30D_H__
#define __SHT30D_H__

#include "i2c_handler.h"
#include <stdio.h>

#define SHT30D_ADDRESS 0x44

typedef struct
{
    uint8_t address;
    i2c_config_t i2c_conf;

} SHT30D_config_t;

typedef struct
{
    float temperature;
    float humidity;
}SHT30D_data_t;

typedef struct
{
    SHT30D_config_t config;
    SHT30D_data_t data;
}SHT30D_t;

typedef enum
{
    SHT30D_OK = 0,
    SHT30D_ERROR_I2C = -1,
    SHT30D_ERROR = -2,
    SHT30D_TEMP_MEAS_ERROR = -3,
    SHT30D_HUMID_MEAS_ERR = -4
}SHT30D_STATUS_t;

SHT30D_STATUS_t SHT30D_init(SHT30D_config_t *config);
SHT30D_STATUS_t SHT30D_get_data(SHT30D_t *sht);

#endif
