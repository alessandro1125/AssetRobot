#ifndef ERROR_UTILS
#define ERROR_UTILS

#ifdef __cplusplus
extern "C" {
#endif

/**
 * ERRORS:
 * 
 * - 0xFFF -> GENERIC_ERROR
 * 
 * ___ 0x1xx => SERIAL ERRORS ___
 * 
 * __ 0x10x => GENERIC ERRORS __
 * - 0x100 -> SERIAL_TIMEOUT_ERROR
 * 
 * __ 0x11x => executeCmd() ERRORS __
 * - 0x110 -> COMMAND_EXECUTED
 * - 0x111 -> ALREADING_MOVING (error attempting executing movement)
 * 
 * __ 0x12x => updateAddress() ERRORS __
 * - 0x120 -> UPDATE_COMPLETED
 * 
 * __ 0x13x => readAddress() ERRORS __
 * - 0x130 -> READ_COMPLETED
 * 
 * ___ 0x200 => MOTION STATES ___
 * 
 * - 0x200 -> MOTOR_NOT_MOVING
 * - 0x201 -> MOTOR_MOVING
 * 
 */

#define     GENERIC_ERROR           0xFFF

//___ 0x1xx => SERIAL ERRORS ___
#define     SERIAL_TIMEOUT_ERROR    0x100
#define     COMMAND_EXECUTED        0x110
#define     ALREADING_MOVING        0x111
#define     UPDATE_COMPLETED        0x120
#define     READ_COMPLETED          0x130

//___ 0x200 => MOTION STATES ___
#define     MOTOR_NOT_MOVING        0x200
#define     MOTOR_MOVING            0x201



/**
 * Definisco una struttutra di gestione degli errori con i loro codici univoci
 * es: 0x0112 -> MOVEMENT_ERROR
*/


#ifdef __cplusplus
}
#endif

#endif