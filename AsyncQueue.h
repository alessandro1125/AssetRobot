/**
 * Libreria per la gestione di eventi asyncroni (movimenti e azioni), i quali 
 * necessitano di essere eseguiti asyncronamente in base a una priorità definita.
 */

#ifndef DISPLAY_INTERFACE
#define DISPLAY_INTERFACE

#ifdef __cplusplus
extern "C" {
#endif

#include "NextionUtils.h"
#include "MotorController.h"

/* int32_t Position */
#define ACTION_EXECUTE_MOVMENT 0x01

#define ACTION_STOP_MOVMENT 0x02

#define ACTION_CHECK_MOVING 0x03

#define ACTION_GET_LAST_ERROR 0x04

#define ACTION_GET_POSITIOIN 0x05

/* uint16_t address */
#define ACTION_GET_PARAM 0x06



void registerAction(int priority, void ( * action)(void * parameters));

#ifdef __cplusplus
}
#endif

#endif