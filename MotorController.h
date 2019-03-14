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

#define MOTOR_CONTROLLER_TAG "Motor Controller"

class Motor {
    public:
        /* External params */
        uint16_t asses_id = 0xFFFF;

        /* Internal params */
        /*uint16_t aceleration;
        uint16_t maxSpeed;
        uint16_t deceleration;
        uint16_t errorTollerance;
        int16_t kp;
        int16_t ki;
        uint32_t timeToErrorToll;
        int32_t menoPosition;
        int32_t maxPosition;
        int32_t zeroPosition;
        int32_t targetPosition;
        int32_t positionProfile;
        int32_t currentPosition;
        int32_t errore;
        uint16_t pwmPid;
        int onMoving;*/

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
};

#ifdef __cplusplus
}
#endif

#endif  