# NextionUtils.h

### Libreria di controllo e inizializzazione del display Nextion, implementa la comunicazione tra ESP32 e display tradmite UART2

## Definizioni

Specifiche di definizione UART2 in *[UartUtils.h](UartUtils.md)*

*Inizializzazione display*: contiene i comandi per lanciare il task di recezione uart
```c++
void display_init(uart_port_t port);
```

<br>

*Trasmissione a display*
```c++
void sendDisplayStatement(char * data, int len);
```

<br>

*Gestione eventi*: gestione eventi di *touch* e *release* nel display
```c++
void setEventListener(void ( * event_callback)(int, int, component_t));
```

<br>

*Recezione update parameter*: recezione e decodifica del protocollo per l'aggiornamento di parametri nell' Asset ARM device secondo il protocollo specificato:

```c++
/* Protocollo update param 
*  | 0xF1 | ARM_ID<4:0>(LSB to MSB) | PARAM_ID | VALUE<0:4>(LSB to MSB) 
*  | 0xFF | 0xFF | 0xFF |
*/
static void handleUpdateParamEvent(uint8_t data)
```
-----------
[Home](display.md)