#ifndef __MQTT_HANDLER_H__
#define __MQTT_HANDLER_H__

#include "mosquitto.h"

#define MAX_SUBSCRIPTION_TOPICS 10
#define MAX_PARAMETER_SIZE 100

typedef void (*mqtt_message_callback_t)(const char* topic, const char* message);

typedef enum
{
    MQTT_HANDLER_OK,
    MQTT_HANDLER_ERROR
}mqtt_handler_status_t;

typedef struct
{
    const char *topic;
    mqtt_message_callback_t callback;
}mqtt_topic_handler_t;

typedef struct
{
    char **topics;
    uint8_t size;
}pub_sub_t;

typedef struct
{
    char client_id[MAX_PARAMETER_SIZE];
    char address[MAX_PARAMETER_SIZE];
    uint16_t port;
    pub_sub_t publish_topics;
    pub_sub_t subscription_topics;
    uint8_t QOS;
    uint16_t keep_alive;
    struct
    {
        char message_online[MAX_PARAMETER_SIZE];
        char message_offline[MAX_PARAMETER_SIZE];
        char topic[MAX_PARAMETER_SIZE];
        uint8_t QOS;
        bool retain;
    }last_will;
   uint16_t heartbeat_interval;
}mqtt_parameters_t;

void mqtt_register_callback(const char* topic, mqtt_message_callback_t callback);

mqtt_handler_status_t initialize_mqtt_handler(const char* config_path);
mqtt_handler_status_t start_mqtt_client(void);
mqtt_handler_status_t publish_message(const char* topic, const char* message);
void deconstruct_mqtt(void);

#endif
