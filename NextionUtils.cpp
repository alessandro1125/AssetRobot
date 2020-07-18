#include "NextionUtils.h"

void ( * event_listener)(int, int, component_t); // Callback for rx recived bytes

int eventListening = 0;

uart_port_t usedPort;

/* Decoding state parameters */
static int eventRecivingType = 0;
static int state = 0;
/* Action event parameters */
uint8_t msbId;
uint8_t lsbId;
uint8_t actionId;

/* Update Param event type */
uint16_t armId;
uint8_t  paramId;
uint32_t paramValue;

/* Protocollo update param 
    | 0xF1 | ARM_ID<4:0>(LSB to MSB) | PARAM_ID | VALUE<0:4>(LSB to MSB) | 0xFF | 0xFF | 0xFF |
*/
static void handleUpdateParamEvent(uint8_t data) {
    uint16_t sign16Tmp = (uint16_t)data;
    uint32_t sign32Tmp = (uint32_t)data;
    switch (state)
    {
        case 1:
            armId = (uint16_t)data;
            state = 2;
            break;
        case 2:
            state = 3;
            break;
        case 3:
            state = 4;
            break;
        case 4:
            state = 5;
            break;
        case 5:
            paramId = data;
            state = 6;
            break;
        case 6:
            paramValue = data;
            state = 7;
            break;
        case 7:
            sign32Tmp = sign32Tmp << 8;
            paramValue |= sign32Tmp;
            state = 8;
            break;
        case 8:
            sign32Tmp = sign32Tmp << 16;
            paramValue |= sign32Tmp;
            state = 9;
            break;
        case 9:
            sign32Tmp = sign32Tmp << 24;
            paramValue |= sign32Tmp;
            state = 10;
            break;
        case 10:
            if(data == 0xFF) {
                ESP_LOGI("Nextion Utils", "Pacchetto get ricevuto");
                component_t component;
                component.armId = armId;
                component.paramValue = paramValue;
                component.paramId = paramId;
                ESP_LOGI("Nextion Utils", "Evento get ricevuto values: %d, %d, %d", armId, paramId, paramValue);
                event_listener(UPDATE_PARAM_EVENT_TXT, 0, component);
                state = 0;
            }else {
                ESP_LOGI("Nextion Utils", "Pacchetto update non ricevuto correttamente");
                state = 0;
            }
            break;
        default:
            state = 0;
            break;
    }
}

static void handleTouchEvent(uint8_t data) {
    switch (state)
    {
        case 1:
                msbId = data;
                state = 2;
                break;
            case 2:
                lsbId = data;
                state = 3;
                break;
            case 3:
                actionId = data;
                state = 4;
                break;
            case 4:
                if(data == 0xFF) {
                    component_t component;
                    uint16_t compId = (uint16_t)msbId;
                    compId = (compId << 8);
                    compId |= lsbId;
                    component.id = compId;
                    ESP_LOGI("Nextion Utils", "Evento ricevuto: %d", actionId);
                    event_listener(TOUCH_EVENT, actionId, component);
                }
                state = 0;
                break;
        default:
            break;
    }
}

static void uart2_handler(uint8_t * recivedData, size_t len) {
    for(size_t i = 0; i < len; i++)
    {
        uint8_t data = recivedData[i];
        ESP_LOGI("UART", "Byte: %d", data);
        if(state == 0) {
            if(data == TOUCH_EVENT || data == ERRROR_EVENT || data == UPDATE_PARAM_EVENT_TXT) {
                eventRecivingType = data;
                state = 1;
                ESP_LOGI("Nextion Utils", "Recezione update param iniziata");
            }
        }else {
            if(eventRecivingType == TOUCH_EVENT) 
                handleTouchEvent(data);
            if(eventRecivingType == UPDATE_PARAM_EVENT_TXT) {
                handleUpdateParamEvent(data);
            }
        }
    }
}

void setEventListener(void ( * event_callback)(int, int, component_t)) {
    event_listener = event_callback;
    eventListening = 1;
}

void sendDisplayStatement(char * data, int len) {
    const int txBytes = uart_write_bytes(usedPort, data, len);

    /* Send  display cmd */
    char cmd_exc[1];
    cmd_exc[0] = 0xff;
    const int len_cmd = strlen(cmd_exc);
    uart_write_bytes(usedPort, cmd_exc, len_cmd);
    uart_write_bytes(usedPort, cmd_exc, len_cmd);
    uart_write_bytes(usedPort, cmd_exc, len_cmd);
}

void display_init(uart_port_t port) {
    usedPort = port;
    uartInit(port);
    startRxTask(port, uart2_handler);
    vTaskDelay(20 / portTICK_PERIOD_MS);
}