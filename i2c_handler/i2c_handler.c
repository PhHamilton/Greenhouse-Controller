#include "i2c_handler.h"
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

int i2c_write_register(uint8_t addr, uint8_t reg, uint16_t value)
{
    int fd = open("/dev/i2c-1", O_RDWR);
    if (fd < 0)
        return -1;

    if(ioctl(fd, I2C_SLAVE, addr) < 0)
    {
        close(fd);
        return -1;
    }

    uint8_t buf[3];
    buf[0] = reg;
    buf[1] = (value >> 8) & 0xFF;   // MSB
    buf[2] = value & 0xFF;          // LSB

    if(write(fd, buf, 3) != 3)
    {
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}

int i2c_read_register(uint8_t addr, uint8_t reg, uint16_t *value)
{
    int fd = open("/dev/i2c-1", O_RDWR);
    if (fd < 0)
        return -1;

    if(ioctl(fd, I2C_SLAVE, addr) < 0)
    {
        close(fd);
        return -1;
    }

    if(write(fd, &reg, 1) != 1)
    {
        close(fd);
        return -1;
    }

    uint8_t buf[2];
    if(read(fd, buf, 2) != 2)
    {
        close(fd);
        return -1;
    }

    *value = (buf[0] << 8) | buf[1];

    close(fd);
    return 0;
}

