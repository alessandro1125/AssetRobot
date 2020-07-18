#ifndef MOTOR_CONTROLLER
#define MOTOR_CONTROLLER

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "SerialCommunicationUtils.h"
#include "ErrorUtils.h"

#define MOTOR_CONTROLLER_TAG "MOTOR_CONTROLLER"

class Motor {
    public:
        /* External params */
        uint16_t asses_id = 0xFFFF;

        /* TODO serial address */

        void setArmId(uint16_t armId);
        int initPosition(); // Finecorsa
        int checkMoving();  // Read on moving params
        int executeMovement(int32_t position);
        int stopMovement();
        int getLastError();
        uint32_t getCurrentPosition();
        uint32_t getParam(uint16_t address);
        int updateMcParam(uint16_t address, uint32_t value);
        void handleErrorEvent(uint32_t errorCode);
};

#ifdef __cplusplus
}
#endif

#endif  