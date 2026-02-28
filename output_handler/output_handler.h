#ifndef __OUTPUT_HANDLER_H__
#define __OUTPUT_HANDLER_H__

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "ina219.h"
#include "gpio_handler.h"

typedef enum
{
    OUTPUT_STATUS_NOT_INITIALIZED,
    OUTPUT_STATUS_NO_VOLTAGE,
    OUTPUT_STATUS_NO_CURRENT,
    OUTPUT_STATUS_ENABLED_NO_CURRENT
}output_status_t;

typedef struct
{
    char name[20];

    gpio_t gpio;
    INA219_t current_sensor;
    bool enabled;
}output_t;

bool output_init(output_t *out);
bool output_enable(output_t *out);
bool output_disable(output_t *out);
bool output_update_measurement(output_t *out);

#endif //__OUTPUT_HANDLER_H__
