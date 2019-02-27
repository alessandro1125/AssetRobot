#include "DisplayInterface.h"

controller_t controllers[RX_BUFFER_LEN];
int controllersCount;

static void executeOperation(queueParameters_t * parameters) {

    //queueParameters_t params = *((queueParameters_t *) parameters);
    queueParameters_t params = (*parameters);

    int evType = params.evType;
    controller_t controller = (*params.controller);

    ESP_LOGI("NEXTION","Compoennte trovato, arm_id: %d", ( * controller.motor).asses_id);
    if(evType == 0x01) {
        Motor motor = ( * controller.motor);
        if(controller.type == 0) {
            ESP_LOGI("NEXTION","Moving state %d", motor.checkMoving());
            if((motor.checkMoving() == MOTOR_NOT_MOVING)) {
                uint32_t tgPosition = motor.getParam(MENO_POSITION);
                ESP_LOGI("NEXTION","Position %d", tgPosition);
                if(tgPosition != SERIAL_TIMEOUT_ERROR && tgPosition != GENERIC_ERROR)
                    motor.executeMovement(tgPosition);      
            }
        }
        if(controller.type == 1) {
            if((motor.checkMoving() == MOTOR_NOT_MOVING)) {
                uint32_t tgPosition = motor.getParam(MAX_POSITION);
                ESP_LOGI("NEXTION","Position %d", tgPosition);
                if(tgPosition != SERIAL_TIMEOUT_ERROR && tgPosition != GENERIC_ERROR)
                    motor.executeMovement(tgPosition);
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

static void event_callback(int evType, component_t component) {
    ESP_LOGI("NEXTION","Evento ricevuto typer: %d id: %d", evType, component.id);

    // Cerco un controller con quell'id
    for(size_t i = 0; i < controllersCount; i++)
    {
        // Agisco sul controller con l'id corrispondente
        if (controllers[i].componentId == component.id) {
            queueParameters_t params;
            params.controller = &controllers[i];
            params.evType = evType;
            registerAction(0, &executeOperation, &params);
        }
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