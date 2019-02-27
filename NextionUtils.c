#include "NextionUtils.h"

void ( * event_listener)(int, component_t); // Callback for rx recived bytes

int eventListening = 0;

uart_port_t usedPort;

/* Decoding state parameters */
int state = 0;
uint8_t msbId;
uint8_t lsbId;
uint8_t actionId;

void uart2_handler(uint8_t * recivedData, size_t len) {
    

    for(int i = 0; i < len ; i++)
        ESP_LOGI("NEXTION","Code recieved %d", recivedData[i]);

    for(size_t i = 0; i < len; i++)
    {
        uint8_t data = recivedData[i];
        switch (state)
        {
            case 0:
                if(data == 0x65)
                    state = 1;
                break;
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
                    event_listener(actionId, component);
                }
                state = 0;
                break;
            default:
                break;
        }
    }
}

void setEventListener(void ( * event_callback)(int, component_t)) {
    event_listener = event_callback;
    eventListening = 1;
}

void sendDisplayStatement(char * data, int len) {
    const int txBytes = uart_write_bytes(usedPort, data, len);

    /* Send  display cmd */
    char * cmd_exc[1];
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