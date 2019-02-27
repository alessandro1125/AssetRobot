#ifndef WDT_UTILS
#define WDT_UTILS

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"
#include "esp_system.h"
#include "esp_log.h"

#define WDT_TIMEOUT_S 20

void wdtInit(int timeOutS);
void wdtSubscribe(TaskHandle_t handle);
void wdtUnSubscribe(TaskHandle_t handle);
void wdtDeinit(void);
void wdtReset(void);


#ifdef __cplusplus
}
#endif

#endif