#ifndef NEXTION_UTILS
#define NEXTION_UTILS

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "UartUtils.h"

typedef struct {
    uint16_t id;
}component_t;

/* Initialize display */
void display_init(uart_port_t port);

/* Trasmitt to display */
void sendDisplayStatement(char * data, int len);

/**
 * Events:
 * 1 -> Touch event
 * 2 -> Release Event
 */
void setEventListener(void ( * event_callback)(int, component_t));

#ifdef __cplusplus
}
#endif

#endif  