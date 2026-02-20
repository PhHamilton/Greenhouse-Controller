#include "ina219.h"
#include <stdio.h>
#include "i2c_handler.h"

#define INA219_REG_CONFIG      0x00
#define INA219_REG_SHUNT_VOLT  0x01
#define INA219_REG_BUS_VOLT    0x02
#define INA219_REG_POWER       0x03
#define INA219_REG_CURRENT     0x04
#define INA219_REG_CALIB       0x05

#define INA219_CONFIG_DEFAULT  0x399F

static int write_register(INA219_config_t* conf, uint8_t reg, uint16_t value)
{
    int hi = (value >> 8) & 0xFF;
    int lo = value & 0xFF;
    int rc = i2c_write_register(conf->address, reg, (lo << 8) | hi);
    return (rc == 0) ? INA219_OK : INA219_ERROR_I2C;
}

static int read_register(INA219_config_t* conf, uint8_t reg, uint16_t* value)
{
    uint16_t raw;
    if(i2c_read_register(conf->address, reg, &raw) < 0) return INA219_ERROR_I2C;
    ;
    *value = ((raw & 0xFF) << 8) | (raw >> 8);
    return INA219_OK;
}

INA219_STATUS_t ina219_init(INA219_config_t* conf)
{
    if (!conf) return INA219_ERROR;

    // default config
    if (write_register(conf, INA219_REG_CONFIG, INA219_CONFIG_DEFAULT) < 0)
        return INA219_ERROR_I2C;

    conf->current_lsb = conf->shunt_resistance > 0 ? 1.0 / 1000.0 : 0.0001;
    conf->power_lsb = conf->current_lsb * 20;
    conf->calibration_value = (uint16_t)(0.04096f / (conf->current_lsb * conf->shunt_resistance));

    if (write_register(conf, INA219_REG_CALIB, conf->calibration_value) < 0)
        return INA219_ERROR_I2C;

    return INA219_OK;
}

INA219_STATUS_t ina219_read(INA219_config_t* conf, INA219_data_t* data)
{
    if (!conf || !data) return -1;

    uint16_t raw_bus, raw_current, raw_power;
    if (read_register(conf, INA219_REG_BUS_VOLT, &raw_bus) < 0) return -1;
    if (read_register(conf, INA219_REG_CURRENT, &raw_current) < 0) return -1;
    if (read_register(conf, INA219_REG_POWER, &raw_power) < 0) return -1;

    data->voltage = ((raw_bus >> 3) * 0.004f); // 4mV per bit
    data->current = (int16_t)raw_current * conf->current_lsb;
    data->power = raw_power * conf->power_lsb;

    return INA219_OK;
}
