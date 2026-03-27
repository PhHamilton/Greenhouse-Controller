#include "i2c_handler.h"
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

int i2c_init(i2c_config_t *conf)
{
    if(!conf)
    {
        return -1;
    }

    conf->fd = open(conf->bus, O_RDWR);
    if(conf->fd < 0)
    {
        return -1;
    }

    if(ioctl(conf->fd, I2C_SLAVE, conf->addr) < 0)
    {
        close(conf->fd);
        return -1;
    }
    return 0;
}

int i2c_write_read_register(i2c_config_t *conf, uint8_t reg, uint16_t value)
{
    int rc = i2c_write_register(conf, reg, value);
    if(rc != 0) return rc;

    uint16_t read_back;
    rc = i2c_read_register(conf, reg, &read_back);

    if(rc != 0)
    {
        return rc;
    }

    if(read_back != value) return -4;

    return 0;
}

int i2c_write_bytes(i2c_config_t *conf, uint8_t* bytes, uint8_t size)
{
    if(!conf || conf->fd< 0)
    {
        return -1;
    }

    if(write(conf->fd, bytes, size) != size)
    {
        return -1;
    }

    return 0;
}

int i2c_read_bytes(i2c_config_t *conf, uint8_t *data, uint8_t size)
{
    if(!conf || conf->fd < 0)
    {
        return -1;
    }

    if(read(conf->fd, data, size) != size)
    {
        return -1;
    }
    return 0;
}

int i2c_write_register(i2c_config_t *conf, uint8_t reg, uint16_t value)
{
    if(!conf || conf->fd < 0)
    {
        return -1;
    }

    uint8_t buf[3];
    buf[0] = reg;
    buf[1] = (value >> 8) & 0xFF;   // MSB
    buf[2] = value & 0xFF;          // LSB

    if(write(conf->fd, buf, 3) != 3)
    {
        return -1;
    }

    return 0;
}

int i2c_read_register(i2c_config_t *conf, uint8_t reg, uint16_t *value)
{
    if(!conf || conf->fd < 0 || !value)
    {
        return -1;
    }

    if(write(conf->fd, &reg, 1) != 1)
    {
        return -1;
    }

    uint8_t buf[2];
    if(read(conf->fd, buf, 2) != 2)
    {
        return -1;
    }

    *value = (buf[0] << 8) | buf[1];

    return 0;
}
