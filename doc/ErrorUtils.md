# ErrorUtls.h

### **Libreria di gestione generale errori della linea seriale d'errore asincrona e definizione dei condici generali d'errore utilizzati nel sistema**

##### NOTA: La linea d'errore viene gestita in interrupt nel task principale, assicurarsi di non avere processi importanti in esecuzione all'interno del task principale per cui essere interrotti causerebbe un blocco del sistema

-------------------

## Definizioni

*Codici Errore*
```c++
#define     GENERIC_ERROR           0xFFF

//___ 0x1xx => SERIAL ERRORS ___
#define     SERIAL_TIMEOUT_ERROR    0x100
#define     COMMAND_EXECUTED        0x110
#define     ALREADING_MOVING        0x111
#define     UPDATE_COMPLETED        0x120
#define     READ_COMPLETED          0x130
#define     SERIAL_PORT_BUSY        0x101

//___ 0x2xx => MOTION STATES ___
#define     MOTOR_NOT_MOVING        0x200
#define     MOTOR_MOVING            0x201

//___ 0x3xx => ERROR INTERFACE BUS ___
#define     LINE_ERRORS_EMPTY       0x300
#define     LINE_ERRORS_MOV_NRANGE  0x301
#define     LINE_ERRORS_ON_MOVING   0x302
#define     LINE_ERRORS_ARM_BLOCK   0x303
```

<br>

*Definizione pin linea errore*
```c++
#define ERROR_LINE (GPIO_NUM_14)
```

<br>

*Stato programma*: riporta lo stato d'errore del programma, se diverso da zero contiene il codice d'errore associato
```c++
static int errorState = 0;
```

<br>

*Set Up error line*: inizializzazione linea di errore e PINOUT
```c
void setUpErrorLineBus();
```

<br>

*interrupt linea d'errore*
```c
void handleErrorIsr();
```
-------------
[Home](../README.md)