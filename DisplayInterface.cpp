#include "DisplayInterface.h"

controller_t controllers[RX_BUFFER_LEN];
int controllersCount;

static void event_callback(int evType, component_t component) {
    ESP_LOGI("NEXTION","Evento ricevuto type: %d id: %d", evType, component.id);

    // Cerco un controller con quell'id
    for(size_t i = 0; i < RX_BUFFER_LEN; i++)
    {
        Motor motor = (* controllers[i].motor);

        // Agisco sul controller con l'id corrispondente
        if (controllers[i].componentId == component.id) {
            if(evType == 0x01) {
                if(controllers[i].type == 0) {
                    uint32_t tgPosition = motor.getParam(MENO_POSITION);
                    motor.executeMovement(tgPosition);
                }
                if(controllers[i].type == 1) {
                    uint32_t tgPosition = motor.getParam(MAX_POSITION);
                    motor.executeMovement(tgPosition);
                }
            }
            if(evType == 0x00) {
                if(controllers[i].type == 1 || controllers[i].type == 0) {
                    motor.stopMovement();
                    // Write the motor position
                    int32_t maxPosition = (int32_t) motor.getParam(MAX_POSITION);
                    int32_t menoPosition = (int32_t) motor.getParam(MENO_POSITION);
                    int32_t currentPosition = (int32_t) motor.getCurrentPosition();
                    ESP_LOGI("DEBUG","Meno position: %d, Max Position: %d, Current position: %d", 
                            menoPosition, macxPosition, currentPosition);
                    uint64_t range = maxPosition - menoPosition;
                    currentPosition -= menoPosition;
                    int ratio = currentPosition / range;
                    ESP_LOGI("DEBUG","Ratio: %d", ratio);
                    char buffer[255];
                    size_t len = sizeof(controllers[i].progressBarId) + 8;
                    sprintf(buffer, "%s.val=%03d", ratio);
                    sendDisplayStatement(buffer, len);
                }
            }
        }
    }
}

void InitializeDisplay(void) {
    display_init(UART_NUM_2);
    setEventListener(event_callback); 
}

void DefineController(int displayId, char * progressBarId, Motor * mc, int type) {
    controller_t controller;
    controller.componentId = displayId;
    controller.progressBarId = progressBarId;
    controller.motor = mc;
    controller.type = type;
    controllers[controllersCount] = controller;
}