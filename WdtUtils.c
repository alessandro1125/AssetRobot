#include "WdtUtils.h"

void wdtInit(int timeOutS) {
    esp_task_wdt_init(timeOutS, false);
}

void wdtSubscribe(TaskHandle_t handle) {
    esp_task_wdt_add(handle);
}

void wdtUnSubscribe(TaskHandle_t handle) {
    esp_task_wdt_delete(handle);
}

void wdtDeinit(void) {
    esp_task_wdt_deinit();
}

void wdtReset(void) {
    esp_task_wdt_reset();
}