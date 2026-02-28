#ifndef __GPIO_HANDLER_H__
#define __GPIO_HANDLER_H__

#include <stdint.h>
#include <stdbool.h>
#include <gpiod.h>

typedef enum
{
    GPIO_ON,
    GPIO_OFF
}gpio_state_t;

typedef enum
{
    GPIO_INPUT,
    GPIO_OUTPUT
}gpio_output_state_t;

typedef struct
{
    struct gpiod_line_request *request;
    unsigned int pin;
    uint8_t error_flags;
}gpio_t;

bool gpio_init_output(gpio_t *gpio);
bool gpio_init_input(gpio_t *gpio);
bool gpio_set(gpio_t *gpio, bool value);
bool gpio_get(gpio_t *gpio, bool *value);
void gpio_release(gpio_t *gpio);

#endif
