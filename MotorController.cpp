/* 
    Created By Alessandro Giordano 01/11/2018
*/

#include "MotorController.h"

void Motor::setArmId(uint16_t armId) {
    Motor::asses_id = armId;
}

int Motor::checkMoving() {
    param_t destParam;
    while(serialFree() != 0);
    switch(readParam(&destParam, ON_MOVING, Motor::asses_id)) {
        case -2:
            return SERIAL_PORT_BUSY;
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
    while(serialFree() != 0);
    switch(executeCmd(INIT_POSITION_CMD, Motor::asses_id)) {
        case -2:
            return SERIAL_PORT_BUSY;
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
    while(serialFree() != 0);
    switch(executeCmd(STOP_MOVEMENT_CMD, Motor::asses_id)) {
        case -2:
            return SERIAL_PORT_BUSY;
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
    while(serialFree() != 0);
    int res = executeCmd(GET_ERRORS_CMD, Motor::asses_id);
    if(res == -2)
        return SERIAL_PORT_BUSY;
    if(res == -1)
        return -1; // SERIAL_ERROR
    else
        return res;
}

uint32_t Motor::getCurrentPosition() {
    param_t destParam;
    while(serialFree() != 0);
    switch(readParam(&destParam, CURRENT_POSITION , Motor::asses_id)) {
        case -2:
            return SERIAL_PORT_BUSY;
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
    while(serialFree() != 0);
    switch(readParam(&destParam, address , Motor::asses_id)) {
        case -2:
            return SERIAL_PORT_BUSY;
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
    while(serialFree() != 0);
    switch(updateParam(newPosition, Motor::asses_id)) {
        case -2:
            return SERIAL_PORT_BUSY;
        case -1: 
            return SERIAL_TIMEOUT_ERROR;
        case 0:break;
        default:break;
    }
    // Execute movement
    switch(executeCmd(EXECUTE_MOVEMENT_CMD, Motor::asses_id)) {
        case -2:
            return SERIAL_PORT_BUSY;
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