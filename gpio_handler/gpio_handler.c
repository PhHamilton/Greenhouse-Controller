#include "gpio_handler.h"

#define CHIP_NAME "/dev/gpiochip0"

static struct gpiod_chip *chip = NULL;

static bool ensure_chip()
{
    if (!chip)
    {
        chip = gpiod_chip_open(CHIP_NAME);
        if (!chip)
            return false;
    }
    return true;
}

static bool gpio_init_common(gpio_t *gpio,
                             enum gpiod_line_direction direction)
{
    if (!gpio || !ensure_chip())
        return false;

    struct gpiod_line_settings *settings = gpiod_line_settings_new();
    gpiod_line_settings_set_direction(settings, direction);

    if (direction == GPIOD_LINE_DIRECTION_OUTPUT)
        gpiod_line_settings_set_output_value(settings, 0);

    struct gpiod_line_config *line_cfg = gpiod_line_config_new();
    gpiod_line_config_add_line_settings(line_cfg, &gpio->pin, 1, settings);

    struct gpiod_request_config *req_cfg = gpiod_request_config_new();
    gpiod_request_config_set_consumer(req_cfg, "greenhouse");

    gpio->request = gpiod_chip_request_lines(chip, req_cfg, line_cfg);

    gpiod_line_settings_free(settings);
    gpiod_line_config_free(line_cfg);
    gpiod_request_config_free(req_cfg);

    return (gpio->request != NULL);

}

bool gpio_init_output(gpio_t *gpio)
{
    return gpio_init_common(gpio, GPIOD_LINE_DIRECTION_OUTPUT);
}

bool gpio_init_input(gpio_t *gpio)
{
    return gpio_init_common(gpio, GPIOD_LINE_DIRECTION_INPUT);
}

bool gpio_set(gpio_t *gpio, bool value)
{
    if (!gpio || !gpio->request)
        return false;

    return (gpiod_line_request_set_value(gpio->request,
                                         gpio->pin,
                                         value) == 0);
}

bool gpio_get(gpio_t *gpio, bool *value)
{
    if (!gpio || !gpio->request || !value)
        return false;

    int ret = gpiod_line_request_get_value(gpio->request,
                                           gpio->pin);
    if (ret < 0)
        return false;

    *value = ret;
    return true;
}

void gpio_release(gpio_t *gpio)
{
    if (gpio && gpio->request)
        gpiod_line_request_release(gpio->request);
}
