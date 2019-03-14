/* 
    Created by Alessandro Giordano on 03/11/2018
*/

#include "UartUtils.h"

void ( * uart1_rx_handler)(uint8_t * , size_t); // Callback for rx recived bytes
void ( * uart2_rx_handler)(uint8_t * , size_t); // Callback for rx recived bytes

int uart1_en = 0;
int uart2_en = 0;

uint8_t rxBufferData[RX_BUF_SIZE];

int uartSendBuffer(uart_port_t port, const char * data)
{
    const int len = strlen(data);
    const int txBytes = uart_write_bytes(port, data, len);

    ESP_LOGI("Uart tx", "Wrote %d bytes", txBytes);
    return txBytes;
}

void uartSendByte(uart_port_t port, char value) {
    char dataSend[1];
    dataSend[0] = value;
    uart_write_bytes(port, dataSend, 1);
    //ESP_LOGI("Uart tx", "Wrote 1 byte");
}

int sendDysplayData(uart_port_t port, const char * data) {
    const int len = strlen(data);
    const int txBytes = uart_write_bytes(port, data, len);

    /* Send  display cmd */
    char * cmd_exc[1];
    cmd_exc[0] = 0xff;
    const int len_cmd = strlen(cmd_exc);
    uart_write_bytes(port, cmd_exc, len_cmd);
    uart_write_bytes(port, cmd_exc, len_cmd);
    uart_write_bytes(port, cmd_exc, len_cmd);

    //ESP_LOGI("Uart tx", "Wrote %d bytes", txBytes);
    return txBytes;
}

static void rx_task(void * pvParameters) {
    while (1) {
        if(uart1_en == 1) {
            const int rxBytes = uart_read_bytes(UART_NUM_1, rxBufferData, RX_BUF_SIZE, 0);
            if (rxBytes > 0) {
                uart1_rx_handler(rxBufferData, rxBytes);
            }
        }
        if(uart2_en == 1) {
            const int rxBytes = uart_read_bytes(UART_NUM_2, rxBufferData, RX_BUF_SIZE, 0);
            if (rxBytes > 0) {

                /* debug */
                for(size_t i = 0; i < rxBytes; i++)
                {
                    //ESP_LOGI("UART", "Byte: %d", rxBufferData[i]);
                }
                
                uart2_rx_handler(rxBufferData, rxBytes);
            }
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

void startRxTask(uart_port_t port, void ( * rx_callback)(uint8_t * , size_t)) {
    if(port == UART_NUM_1)
        uart1_rx_handler = rx_callback;
    if(port == UART_NUM_2)
        uart2_rx_handler = rx_callback;

    if(port == UART_NUM_1)
        uart1_en = 1;
    if(port == UART_NUM_2)
        uart2_en = 1;
}


void uartInit(uart_port_t port) {
    const uart_config_t uart_config = {
        .baud_rate = BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(port, &uart_config);
    if(port == UART_NUM_1)
        uart_set_pin(port, TX1D_PIN, RX1D_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    if(port == UART_NUM_2)
        uart_set_pin(port, TX2D_PIN, RX2D_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    // We won't use a buffer for sending data.
    uart_driver_install(port, RX_BUF_SIZE * 2, 0, 0, NULL, 0);

    xTaskCreate(rx_task, "uart_rx_task", 1024*2, NULL, configMAX_PRIORITIES, NULL);
}