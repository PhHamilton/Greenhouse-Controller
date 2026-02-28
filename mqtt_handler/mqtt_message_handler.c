#include "mqtt_message_handler.h"
#include "mqtt_handler.h"
#include "main.h"
#include <string.h>
#include <stdlib.h>

extern mqtt_parameters_t mqtt_parameters;

bool validate_json_number(cJSON *parent, const char *key, uint8_t *out_value);

cJSON *create_data_item(uint8_t id, uint8_t status, float voltage, float current);

void status_update_handler(const char* topic, const char* message)
{
    cJSON *json = cJSON_Parse(message);

    if(!json)
    {
        fprintf(stderr, "Error parsing JSON %s\n", cJSON_GetErrorPtr());
        return;
    }

    uint8_t request;
    if(!validate_json_number(json, "Request", &request))
    {
        cJSON_Delete(json);
        return;
    }

    cJSON *root = cJSON_CreateObject();
    cJSON *data_array = cJSON_CreateArray();

    cJSON_AddNumberToObject(root, "Request", request);

/*
    if(!get_start_end_index((status_request_type_t)request, &start, &end))
    {
        fprintf(stderr, "Unable to fetch loop indexes\n");
        cJSON_Delete(json);
        cJSON_Delete(root);
        return;
    }
    for(uint8_t i = start; i <= end; i++)
    {
        if(i < NUMBER_OF_CHANNELS)
        {
            cJSON_AddItemToArray(
                                 data_array,
                                 create_data_item(
                                                  i,
                                                  gui_parameters.measurements[i].output_state,
                                                  gui_parameters.measurements[i].voltage,
                                                  gui_parameters.measurements[i].current
                                                 )
                                );
        }
    }
*/

    cJSON_AddItemToObject(root, "Data", data_array);
    char *json_string = cJSON_PrintUnformatted(root);

    if(json_string)
    {
        const char* topic = NULL;
        for(uint8_t i = 0; i < mqtt_parameters.publish_topics.size; i++)
        {
            if(strstr(mqtt_parameters.publish_topics.topics[i], "/response"))
            {
                topic = mqtt_parameters.publish_topics.topics[i];
            }
        }

        if(topic)
        {
            publish_message(topic, json_string);
        }
        free(json_string);
    }

    cJSON_Delete(json);
    cJSON_Delete(root);
}

void output_update_handler(const char* topic, const char* message)
{
     cJSON *json = cJSON_Parse(message);

    if(!json)
    {
        fprintf(stderr, "Error parsing JSON %s\n", cJSON_GetErrorPtr());
        return;
    }

    uint8_t request, status;
    if(!validate_json_number(json, "Request", &request))
    {
        cJSON_Delete(json);
        return;
    }

    if(!validate_json_number(json, "Status", &status))
    {
        cJSON_Delete(json);
        return;
    }

    cJSON *root = cJSON_CreateObject();
    cJSON *data_array = cJSON_CreateArray();

    cJSON_AddNumberToObject(root, "Request", request);

    /*
    if(!get_start_end_index((status_request_type_t)request, &start, &end))
    {
        fprintf(stderr, "Unable to fetch loop indexes\n");
        cJSON_Delete(json);
        cJSON_Delete(root);
        return;
    }

    for(uint8_t i = start; i <= end; i++)
    {
        if(i < NUMBER_OF_CHANNELS)
        {
            if(status != gui_parameters.measurements[i].output_state)
            {
                gui_parameters.measurements[i].output_state = status;
            }

            cJSON_AddItemToArray(
                                 data_array,
                                 create_status_item(
                                                    i,
                                                    gui_parameters.measurements[i].output_state
                                                   )
                                );
        }
    }
    */

    cJSON_AddItemToObject(root, "Data", data_array);
    char *json_string = cJSON_PrintUnformatted(root);

    if(json_string)
    {
        publish_message("power_controller/response", json_string);
        free(json_string);
    }

    cJSON_Delete(json);
    cJSON_Delete(root);
}

cJSON *create_data_item(uint8_t id, uint8_t status, float voltage, float current)
{
    cJSON *data_item = cJSON_CreateObject();

    if(!data_item) return NULL;

    cJSON_AddNumberToObject(data_item, "ID", id);
    cJSON_AddNumberToObject(data_item, "Status", status);
    cJSON_AddNumberToObject(data_item, "Voltage", voltage);
    cJSON_AddNumberToObject(data_item, "Current", current);

    return data_item;
}

cJSON *create_status_item(uint8_t id, uint8_t status)
{
    cJSON *data_item = cJSON_CreateObject();
    cJSON_AddNumberToObject(data_item, "ID", id);
    cJSON_AddNumberToObject(data_item, "Status", status);

    return data_item;
}

bool validate_json_number(cJSON *parent, const char *key, uint8_t *out_value)
{
    cJSON *item = cJSON_GetObjectItem(parent, key);

    if(!item)
    {
        fprintf(stderr, "Missing '%s' item\n", key);
        return false;
    }

    if(!cJSON_IsNumber(item))
    {
        fprintf(stderr, "'%s' item is not a number\n", key);
        return false;
    }
    *out_value = item->valueint;
    return true;
}
