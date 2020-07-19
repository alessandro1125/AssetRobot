# UartUtils.h

Configurazione e gestione hadrware delle periferiche UART

## Definizioni


*Pin configurations*: definizione pins
```c
#define TX1D_PIN (GPIO_NUM_4)   // TX Serial communication motors
#define RX1D_PIN (GPIO_NUM_5)   // RX Serial communication motors
#define TX2D_PIN (GPIO_NUM_17)  // TX display communications
#define RX2D_PIN (GPIO_NUM_16)  // RX display communications

#define TX1_ENABLE (GPIO_NUM_32)
```

<br>

*Port initialization*: inizializzazione periferica UART 

```c
void uartInit(uart_port_t port);
```

<br>

*Start uart handler task*: definizione allbacks per gestione eventi uart

```c
void startRxTask(uart_port_t port, void ( * rx_callback)(uint8_t * , size_t));
```

<br>

*Transmitt buffer*: trasmissione buffer 

```c
int uartSendBuffer(uart_port_t port, const char * data);
```

<br>

*Send display data*: trasmissione dati e comandi secondo standard display *nextion

```c
int sendDysplayData(uart_port_t port, const char * data);
```
