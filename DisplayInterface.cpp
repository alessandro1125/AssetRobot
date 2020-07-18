#include "DisplayInterface.h"

extern Motor motors[32];

controller_t controllers[RX_BUFFER_LEN];
int controllersCount;

static void executeOperation(queueParameters_t * parameters) {

    //queueParameters_t params = *((queueParameters_t *) parameters);
    queueParameters_t params = (*parameters);

    if(params.actionType == UPDATE_PARAM_EVENT_TXT) {
        uint16_t paramAddress = 0;
        switch(params.component.paramId) {
            case 1:
                paramAddress = MAX_POSITION;
                break;
            case 2:
                paramAddress = MENO_POSITION;
                break;
            default:break;
        }
        int res = -2;
        if(paramAddress != 0)
            res = motors[params.component.armId-1].updateMcParam(paramAddress, params.component.paramValue);
        ESP_LOGI("NEXTION","Parametro aggiornato, return res: %d", res);
    }
    
    if(params.actionType == TOUCH_EVENT) {

        int evType = params.evType;
        controller_t controller = (*params.controller);

        ESP_LOGI("NEXTION","Compoennte trovato, arm_id: %d", ( * controller.motor).asses_id);
        if(evType == 0x01) {
            Motor motor = ( * controller.motor);
            if(controller.type == 0) {
                if((motor.checkMoving() == MOTOR_NOT_MOVING)) {
                    uint32_t tgPosition = motor.getParam(MENO_POSITION);
                    ESP_LOGI("NEXTION","Position %d", tgPosition);
                    if(tgPosition != SERIAL_TIMEOUT_ERROR && tgPosition != GENERIC_ERROR)
                        motor.executeMovement(tgPosition);      
                }else {
                    ESP_LOGI("NEXTION","Moving state %d", motor.checkMoving());
                }
            }
            if(controller.type == 1) {
                if((motor.checkMoving() == MOTOR_NOT_MOVING)) {
                    uint32_t tgPosition = motor.getParam(MAX_POSITION);
                    ESP_LOGI("NEXTION","Position %d", tgPosition);
                    if(tgPosition != SERIAL_TIMEOUT_ERROR && tgPosition != GENERIC_ERROR)
                        motor.executeMovement(tgPosition);
                }else {
                    ESP_LOGI("NEXTION","Moving state %d", motor.checkMoving());
                }
            }
        }
        if(evType == 0x00) {
            Motor motor = ( * controller.motor);
            if(controller.type == 1 || controller.type == 0) {
                ESP_LOGI("DEBUG","Stopping mmovment res: %d", motor.stopMovement());
                /*
                // Write the motor position
                int32_t maxPosition = (int32_t) motor.getParam(MAX_POSITION);
                int32_t menoPosition = (int32_t) motor.getParam(MENO_POSITION);
                int32_t currentPosition = (int32_t) motor.getCurrentPosition();

                
                if(maxPosition != SERIAL_TIMEOUT_ERROR && maxPosition != GENERIC_ERROR &&
                    menoPosition != SERIAL_TIMEOUT_ERROR && menoPosition != GENERIC_ERROR &&
                    currentPosition != SERIAL_TIMEOUT_ERROR && currentPosition != GENERIC_ERROR){
                    ESP_LOGI("DEBUG","Meno position: %d, Max Position: %d, Current position: %d", 
                            menoPosition, maxPosition, currentPosition);
                    uint64_t range = maxPosition - menoPosition;
                    currentPosition -= menoPosition;
                    int ratio = currentPosition / range;
                    char buffer[255];
                    size_t len = sizeof(controller.progressBarId) + 8;
                    sprintf(buffer, "%s.val=%03d", controller.progressBarId, ratio);
                    sendDisplayStatement(buffer, len);
                }*/
            }
        }
    }
}

static void event_callback(int actionType, int evType, component_t component) {
    ESP_LOGI("NEXTION","Evento ricevuto type: %d id: %d", evType, component.id);

    
    if(actionType == TOUCH_EVENT) {
        // Cerco un controller con quell'id
        for(size_t i = 0; i < controllersCount; i++)
        {
            // Agisco sul controller con l'id corrispondente
            if (controllers[i].componentId == component.id) {
                queueParameters_t params;
                params.controller = &controllers[i];
                params.evType = evType;
                params.actionType = actionType;
                registerAction(0, &executeOperation, &params);
            }
        }
    }

    if(actionType == UPDATE_PARAM_EVENT_TXT) {
        queueParameters_t params;
        params.evType = evType;
        params.actionType = actionType;
        params.component = component;
        registerAction(0, &executeOperation, &params);
    }
}

void InitializeDisplay(void) {
    display_init(UART_NUM_2);
    setEventListener(event_callback); 
    queueInitialize();
}

void DefineController(int displayId, const char * progressBarId, Motor * mc, int type) {
    controller_t controller;
    controller.componentId = displayId;
    controller.progressBarId = progressBarId;
    controller.motor = mc;
    controller.type = type;
    controllers[controllersCount] = controller;
    controllersCount++;
}