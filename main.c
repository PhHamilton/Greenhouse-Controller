#include <stdio.h>
#include "i2c_handler.h"
#include "ina219.h"

int main(int argc, char **argv)
{

    INA219_config_t light = {
                              .address = 0x40,
                              .shunt_resistance = 0.1f
                            };

    INA219_STATUS_t rc = ina219_init(&light);

    if(rc != INA219_OK)
    {
        printf("Failed to initialize INA: %i\n", rc);
    }
    else
    {
        printf("INA initialized sucessfully\n");
    }

    return 0;
}
