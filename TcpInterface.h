#ifndef TCP_INTERFACE
#define TCP_INTERFACE

#ifdef __cplusplus
extern "C" {
#endif

#include "TcpUtils.h"
#include "TcpCommands.h"


/**
    Definizione protocollo di tramsissione dati TCP

    data tx type: | H | L |

    RECIVING packets:

    | 0x03 | LEN | BUFFER <...> | 0x02 |     -  NON NECESSARIAMENTE IMPLEMENTATO (NON IMPLEMENTATO)


    BUFFER definitions: 

    | CMD uint8_t | DATA <...> |


    == CMD ===

    - GET_ESP32_INFORMATIONS    -> 0x01
    - GET_DEVICE_PARAM          -> 0x02
    - GET_ALL_PARAMS            -> 0x03
    
    

    === DATA ===

    ? GET_ESP32_INFORMATIONS

    | DATA CODE uint8_t |
    
    - FREE_MEMORY       0x01 (uint32)
    - EXECUTION_STATE   0x02 (uint8)
    - ARM_COUNT         0x03 (uint8)
    - ERROR_STATE       0x04 (uint8)


    ? GET_DEVICE_PARAM

    | DATA CODE uint8_t | DEVICE ID uint8_t |
    
    - ACELERATION             0x01 (uint32)
    - MAX_SPEED               0x02 (uint32)
    - DECELERATION            0x03 (uint32)
    - ERROR_TOLLERANCE        0x04 (uint32)
    - KP                      0x05 (uint32)
    - KI                      0x06 (uint32)
    - TIME_TO_ERROR_TOLL      0x07 (uint32)
    - TARGET_POSITION         0x08 (uint32)
    - POSITION_PROFILE        0x09 (uint32)
    - CURRENT_POSITION        0x0A (uint32)
    - ERROR                   0x0B (uint32)
    - PWM_PID                 0x0C (uint32)
    - ON_MOVING               0x0D (uint32)
    - MENO_POSITION           0x0E (uint32)
    - MAX_POSITION            0x0F (uint32)
    - ZERO_POSITION           0x10 (uint32)
    - LAST_ERROR_OCCURRED     0xFF (uint32)



    ? GET_ALL_PARAMS



    SENDING packets:

    | CMD requested | data <..> |



*/

// == CMD ===
#define GET_ESP32_INFORMATIONS  0x01
#define GET_DEVICE_PARAM        0x02
#define GET_ALL_PARAMS          0x03


// === DATA ===
#define FREE_MEMORY             0x01
#define EXECUTION_STATE         0x02
#define ARM_COUNT               0x03
#define ERROR_STATE             0x04


void InitializeTcpInterface();

#ifdef __cplusplus
}
#endif

#endif