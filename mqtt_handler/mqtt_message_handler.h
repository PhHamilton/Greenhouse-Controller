#ifndef __MQTT_MESSAGE_HANDLER_H__
#define __MQTT_MESSAGE_HANDLER_H__

#include "stdio.h"
#include "cJSON.h"
#include <stdint.h>

typedef enum
{
    VOLTAGE_3V3,
    VOLTAGE_5V,
    VOLTAGE_12V,
    VOLTAGE_NEG12V,
    ALL
}status_request_type_t;

void status_update_handler(const char* topic, const char* message);
void output_update_handler(const char* topic, const char* message);

cJSON *create_status_item(uint8_t id, uint8_t status);

#endif //__MQTT_MESSAGE_HANDLER_H__
