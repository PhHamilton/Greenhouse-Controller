#include "mqtt_message_handler.h"
#include "mqtt_handler.h"
#include "main.h"
#include <string.h>
#include <stdlib.h>

extern mqtt_parameters_t mqtt_parameters;

bool validate_json_number(cJSON *parent, const char *key, uint8_t *out_value);
bool validate_json_object(cJSON *parent, const char *key);

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

    cJSON *data = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "Data", data);

    cJSON_AddNumberToObject(data, "ID", 1); //
    cJSON_AddNumberToObject(data, "Status", 1); //Define status!

    cJSON *climate = cJSON_CreateObject();
    cJSON_AddItemToObject(data, "Climate", climate);
    cJSON_AddNumberToObject(climate, "Temperature", 20);
    cJSON_AddNumberToObject(climate, "Humidity", 50);

    cJSON *outputs = cJSON_CreateArray();
    cJSON_AddItemToObject(data, "Outputs", outputs);

    cJSON *output = cJSON_CreateObject();
    cJSON_AddItemToArray(outputs, output);
    cJSON_AddStringToObject(output, "Name", "Light");
    cJSON_AddNumberToObject(output, "State", 0);
    cJSON_AddNumberToObject(output, "Type", 0);
    cJSON *current_measurements = cJSON_CreateObject();
    cJSON_AddItemToObject(output, "Measurements", current_measurements);
    cJSON_AddNumberToObject(current_measurements, "Voltage", 3);
    cJSON_AddNumberToObject(current_measurements, "Current", 100);
    cJSON_AddNumberToObject(current_measurements, "Power", 300);

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

    cJSON *data = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "Data", data);

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

bool validate_json_object(cJSON *parent, const char *key)
{
    cJSON *item = cJSON_GetObjectItem(parent,key);

    if(!item)
    {
        fprintf(stderr, "Missing '%s' item\n", key);
        return false;
    }

    if(!cJSON_IsObject(item))
    {
        fprintf(stderr, "'%s' item is not an object\n", key);
        return false;
    }

    return true;
}
