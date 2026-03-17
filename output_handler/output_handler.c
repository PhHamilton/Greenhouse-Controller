#include "output_handler.h"
#define EPS 0.0001

bool output_init(output_t *out)
{
    if(ina219_init(&out->current_sensor.config) != INA219_OK)
    {
        return false;
    }

    if(!gpio_init_output(&out->gpio))
    {
        return false;
    }

    return true;
}

bool output_enable(output_t *out)
{
    return gpio_set(&out->gpio, true);
}

bool output_disable(output_t *out)
{
    return gpio_set(&out->gpio, false);
}

bool output_update_measurement(output_t *out)
{
/*
    if(!out->enabled)
        return false;
*/

    if(ina219_read(&out->current_sensor.config, &out->current_sensor.data) != INA219_OK)
    {
        return false;
    }

/*
    if(out->measurements.voltage < EPS)
    {
        return false;
    }

    if(out->measurements.current < EPS)
    {
        return false;
    }
*/
    return true;
}
