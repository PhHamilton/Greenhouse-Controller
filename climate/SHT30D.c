#include "SHT30D.h"
#include "i2c_handler.h"
#include <unistd.h>

static uint8_t compute_crc(const uint8_t *data, uint8_t size)
{
    uint8_t crc = 0xFF;

    for(uint8_t i = 0; i < size; i++)
    {
       crc ^= data[i];
       for(uint8_t bit = 0; bit < 8; bit++)
       {
            if(crc & 0x80)
            {
                crc = (crc << 1) ^ 0x31;
            }
            else
            {
                crc <<= 1;
            }
       }
    }

    return crc;
}

SHT30D_STATUS_t SHT30D_init(SHT30D_config_t *config)
{
    if(!config)
    {
        return SHT30D_ERROR;
    }

    if(i2c_init(&config->i2c_conf) != 0)
    {
        return SHT30D_ERROR_I2C;
    }

    return SHT30D_OK;
}

SHT30D_STATUS_t SHT30D_get_data(SHT30D_t *sht)
{
    if(!sht)
    {
        return SHT30D_ERROR;
    }

    // High repeatability, No clock stretching
    uint8_t tx_buf[2] = {0x24, 0x00};

    if(i2c_write_bytes(&sht->config.i2c_conf, tx_buf, 2) < 0)
    {
        return SHT30D_ERROR_I2C;
    }

    // Wait atleast 1 ms before next command
    usleep(500000);

    uint8_t rx_buf[6] = {0};

    if(i2c_read_bytes(&sht->config.i2c_conf, rx_buf, 6) < 0)
    {
        return SHT30D_ERROR_I2C;
    }

    int16_t temp_raw = rx_buf[0] << 8 | rx_buf[1];
    uint8_t temp_crc = rx_buf[2];
    int16_t humidity_raw = rx_buf[3] << 8 | rx_buf[4];
    uint8_t humidity_crc = rx_buf[5];

    uint8_t crc_req = compute_crc(rx_buf, 2);

    if(temp_crc != crc_req)
    {
        return SHT30D_TEMP_MEAS_ERROR;
    }

    crc_req = compute_crc(&rx_buf[3], 2);

    if(humidity_crc != crc_req)
    {
        return SHT30D_HUMID_MEAS_ERR;
    }

    sht->data.temperature = -45.0 + 175.0 * ((float)temp_raw)/ (65535 - 1);
    sht->data.humidity = 100.0 * ((float)humidity_raw) / (65535 - 1);

    return SHT30D_OK;
}

