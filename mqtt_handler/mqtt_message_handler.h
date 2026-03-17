#ifndef __MQTT_MESSAGE_HANDLER_H__
#define __MQTT_MESSAGE_HANDLER_H__

#include "stdio.h"
#include "cJSON.h"
#include <stdint.h>

typedef enum
{
    DISABLE,
    ENABLE
}output_status_types_t;

typedef enum
{
    DATA,
    UPDATE_OUTPUT
}request_types_t;

void status_update_handler(const char* topic, const char* message);
void output_update_handler(const char* topic, const char* message);

cJSON *create_status_item(uint8_t id, uint8_t status);

#endif //__MQTT_MESSAGE_HANDLER_H__
