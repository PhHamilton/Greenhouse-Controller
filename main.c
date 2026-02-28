#include <stdio.h>
#include "unistd.h"
#include "output_handler.h"

int main(int argc, char **argv)
{
    output_t led = {
                     .name = "lights",
                     .current_sensor = {
                                        .config = {
                                                   .address = 0x40,
                                                   .shunt_resistance = 0.1f
                                                  },
                                        .data = {0.0f}
                                       },
                     .gpio = {
                              .pin = 4
                             }
                   };


    if(!output_init(&led))
    {
        printf("Failed to init output: %s\n", led.name);
    }
    printf("Output: %s initialized successfully\n", led.name);

    output_enable(&led);
    sleep(1);

    if(!output_update_measurement(&led))
    {
        printf("Failed read data from: %s\n", led.name);
    }

    printf("Voltage: %f\n Current: %f\n", led.current_sensor.data.voltage, led.current_sensor.data.current);


    sleep(1);
    output_disable(&led);

    return 0;
}
