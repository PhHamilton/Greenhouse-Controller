#include <stdio.h>
#include "unistd.h"
#include "output_handler.h"
#include "mqtt_handler.h"
#include "mqtt_message_handler.h"
#include "SHT30D.h"

int main(int argc, char **argv)
{
    mqtt_register_callback("ghc/status_update", status_update_handler);
//    mqtt_register_callback("ghc/output_update", output_update_handler);

    SHT30D_t climate = {
                          .config = {
                                      .address  = 0x44,
                                      .i2c_conf  = {
                                                    .bus  = "/dev/i2c-1",
                                                    .addr = 0x44
                                                   }
                                    },
                          .data = {0.0f}
                       };


    if(SHT30D_init(&climate.config) != SHT30D_OK)
    {
        printf("Failed to init SHT30D");
        return 1;
    }

/*
    printf("rc: %i\n", rc);
    rc = SHT30D_get_data(&climate);
    printf("rc: %i\n", rc);
*/

    output_t led = {
                     .name = "lights",
                     .current_sensor = {
                                        .config = {
                                                   .i2c_conf = {
                                                                .bus = "/dev/i2c-1",
                                                                .addr = 0x40
                                                               },
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
        return 1;
    }
    printf("Output: %s initialized successfully\n", led.name);

    output_enable(&led);
    sleep(1);

    if(!output_update_measurement(&led))
    {
        printf("Failed read data from: %s\n", led.name);
        return 1;
    }

    printf("Voltage: %f\n Current: %f\n", led.current_sensor.data.voltage, led.current_sensor.data.current);

    if(!SHT30D_get_data(&climate) == SHT30D_OK)
    {
        printf("Failed to read data from climate sensor\n");
        return 1;
    }

    printf("Temperature: %f\n Humidity: %f\n", climate.data.temperature, climate.data.humidity);

    sleep(1);
    output_disable(&led);

    const char* config_path = "./mqtt_handler/config.json";

    if(initialize_mqtt_handler(config_path) != MQTT_HANDLER_OK)
    {
        printf("Failed to initialize MQTT");
        return 1;
    }

    if(start_mqtt_client() != MQTT_HANDLER_OK)
    {
        printf("Failed to start MQTT client");
        return 1;
    }

 

    while(1);
    return 0;
}
