/* 
    Created by Alessandro Giordano on 03/11/2018ù

    NOTE: la linea di abilitazione del MAX buffer è gestita dal pin RX1_ENABLE e viene attivato nella 
    libreria serial communication prima della comunicazione di una serie di bytes
*/

#ifndef UART_UTILS
#define UART_UTILS

#ifdef __cplusplus
extern "C" {
#endif

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "soc/uart_struct.h"
#include "string.h"
#include "driver/gpio.h"

#define BAUD_RATE 9600
#define  RX_BUF_SIZE 1024

#define TX1D_PIN (GPIO_NUM_4)   // TX Serial communication motors
#define RX1D_PIN (GPIO_NUM_5)   // RX Serial communication motors
#define TX2D_PIN (GPIO_NUM_17)  // TX display communications
#define RX2D_PIN (GPIO_NUM_16)  // RX display communications

#define TX1_ENABLE     (GPIO_NUM_32)

void startRxTask(uart_port_t port, void ( * rx_callback)(uint8_t * , size_t));
void uartInit(uart_port_t port);
int uartSendBuffer(uart_port_t port, const char * data);
void uartSendByte(uart_port_t port, char value);
int sendDysplayData(uart_port_t port, const char * data);

#ifdef __cplusplus
}
#endif

#endif