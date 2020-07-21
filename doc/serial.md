# Serial Library

Gestione seriale delle comunicazioni con devices Asset ARM per il controllo motore. Necessario l'auslio di un MAX3483 buffer per bus di comunicazione con abilitazione su **tx** e **rx** a stato idle LOW (necessario intervento), la configurazione utilizzata pilota **!RE = DI = IO32**.


-------------------

## Definizione del protocollo di trasmissione e recezione
Viene riportata la descrizione di programma del protocollo di reasmissione e recezione adottato

```c
* Serial comunications work synchronus, attempting to read or execute any cmd by serial, library will wait for
 * period in READING_TIMEOUT_MS for a response from sender, than return result byte
 * 
 * Communications function:
 * - update param
 * - read param
 * - send CMD
 * 
 * PROTOCOLS DESCRIPTION
 * 
 * TX UART protocol:
 * - update address [ 0x02 | ARM ADDRESSH | ARM ADDRESSL | CMD (0x01) | ADDRESS | VALUEHH | VALUEHL | VALUELH | VALUELL | CTRL | 0x03 ]
 * - read address   [ 0x02 | ARM ADDRESSH | ARM ADDRESSL | CMD (0x02) | ADDRESS | CTRL | 0x03 ]
 * - send cmd       [ 0x02 | ARM ADDRESSH | ARM ADDRESSL | CMD (0x03) | VALUE   | CTRL | 0x03 ]
 * CTRL = (char) (TRASMISSION BYTES)
 * 
 * 
 * 
 * RX UART protocol
 * [ 0x02 | ARM ADDRESSH | ARM ADDRESSL | LEN | ADDRESS | TRASMISSION_BYTES<? .. ?> | CTRL | 0x03 ]
 * LEN = len(ADDRESS) + len(TRASMISSION_BYTES) + len(CTRL)
 * CTRL = (char) (ADDRESS + TRASMISSION_BYTES)
 * 
 * ADDRESS List
 * - 0xFE -> Result Code
 * 
 * CMD values
 * 0x00 - no op
 * 0x01 - update address (len 6)
 * 0x02 - read address (2)
 * 0x03 - send cmd (2)
 * 
 * Commands
 * - 0x00 initialize (finecorsa)
 * - 0x01 start movement
 * - 0x02 stop movement
 * - 0x03 getLastError
 * 
 * PARAMS ADDRESSING
 * - 0x01 -> aceleration
 * - 0x02 -> maxSpeed
 * - 0x03 -> deceleration
 * - 0x04 -> errorTollerance
 * - 0x05 -> kp
 * - 0x06 -> ki
 * - 0x07 -> tomeToErrorToll
 * - 0x08 -> targetPosition
 * - 0x09 -> positionProfile
 * - 0x0A -> currentPosition
 * - 0x0B -> errore
 * - 0x0C -> pwmPid
 * - 0x0D -> onMoving
 * - 0x0E -> menoPosition
 * - 0x0F -> maxPosition
 * - 0x10 -> zeroPosition
 * 
 * - 0xFF -> lastErrorOccurred
 * 
 * 
 * FEEDBACK PROTOCOLS
 * 
 * __ StartMovement()
 *  - 0x00 Movimento eseguito
 *  - 0x01 Non eseguito, onMoving = 1
 * 
 * __ UpdatePosition()
 *  - 0x00 Updated

/* PARAMS DEFINITIONS */
#define ACELERATION         0x01
#define MAX_SPEED           0x02
#define DECELERATION        0x03
#define ERROR_TOLLERANCE    0x04
#define KP                  0x05
#define KI                  0x06
#define TIME_TO_ERROR_TOLL  0x07
#define TARGET_POSITION     0x08
#define POSITION_PROFILE    0x09
#define CURRENT_POSITION    0x0A
#define ERROR               0x0B
#define PWM_PID             0x0C
#define ON_MOVING           0x0D
#define MENO_POSITION       0x0E
#define MAX_POSITION        0x0F
#define ZERO_POSITION       0x10
#define LAST_ERROR_OCCURRED 0xFF
/* End params definitions */

/* CMD DEFINITIONS */
#define INIT_POSITION_CMD   0x00
#define EXECUTE_MOVEMENT_CMD    0x01
#define STOP_MOVEMENT_CMD   0x02
#define GET_ERRORS_CMD      0x03
```

## Library index
* ### [SerialCommunication](SerialCommunication.md)
* ### [UartUtils](UartUtils.md)

-------------
[Home](../README.md)