/* 
    Created By Alessandro Giordano 01/11/2018
*/

#include "MotorController.h"

uint16_t armAddress = 0xFFFF;

void Motor::setArmId(uint16_t armId) {
    armAddress = armId;
}

int Motor::checkMoving() {
    param_t destParam;
    switch(readParam(&destParam, ON_MOVING, armAddress)) {
        case -1:
            return SERIAL_TIMEOUT_ERROR;
        case 0:
            ESP_LOGI(MOTOR_CONTROLLER_TAG,"Motor moving: %d", destParam.value);
            if(destParam.value == 0)
                return MOTOR_NOT_MOVING;
            else
                return MOTOR_MOVING;
        default:break;
    }
    return GENERIC_ERROR;
}

int Motor::initPosition() {
    ESP_LOGI(MOTOR_CONTROLLER_TAG,"Initializing arm to default position");
    switch(executeCmd(INIT_POSITION_CMD, armAddress)) {
        case -1:
            return SERIAL_TIMEOUT_ERROR;
        case 0:
            return COMMAND_EXECUTED;
        default:break;
    }
    return GENERIC_ERROR;
}

int Motor::stopMovement() {
    ESP_LOGI(MOTOR_CONTROLLER_TAG,"Stopping movement");
    switch(executeCmd(STOP_MOVEMENT_CMD, armAddress)) {
        case -1:
            return SERIAL_TIMEOUT_ERROR;
        case 0:
            return COMMAND_EXECUTED;
        default:break;
    }
    return GENERIC_ERROR;
}

int Motor::getLastError() {
    ESP_LOGI(MOTOR_CONTROLLER_TAG,"Reading errors");
    int res = executeCmd(GET_ERRORS_CMD, armAddress);
    if(res == -1)
        return -1; // SERIAL_ERROR
    else
        return res;
}

uint32_t Motor::getCurrentPosition() {
    param_t destParam;
    switch(readParam(&destParam, CURRENT_POSITION , armAddress)) {
        case -1:
            return SERIAL_TIMEOUT_ERROR;
        case 0:
            return destParam.value;
        default:break;
    }
    return GENERIC_ERROR;
}

uint32_t Motor::getParam(uint16_t address) {
    param_t destParam;
    switch(readParam(&destParam, address , armAddress)) {
        case -1:
            return SERIAL_TIMEOUT_ERROR;
        case 0:
            return destParam.value;
        default:break;
    }
    return GENERIC_ERROR;
}

int Motor::executeMovement(int32_t position) {
    ESP_LOGI(MOTOR_CONTROLLER_TAG,"Start movement to position %d", position);
    // Update targetPositionParam
    param_t newPosition;
    newPosition.address = TARGET_POSITION;
    newPosition.value = (uint32_t) position;
    switch(updateParam(newPosition, armAddress)) {
        case -1: 
            return SERIAL_TIMEOUT_ERROR;
        case 0:break;
        default:break;
    }
    // Execute movement
    switch(executeCmd(EXECUTE_MOVEMENT_CMD, armAddress)) {
        case -1:
            return SERIAL_TIMEOUT_ERROR;
        case 1:
            return ALREADING_MOVING;
        case 0:
            return COMMAND_EXECUTED;
        default:break;
    }
    return GENERIC_ERROR;
}