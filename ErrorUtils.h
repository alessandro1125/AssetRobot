#ifndef ERROR_UTILS
#define ERROR_UTILS

#ifdef __cplusplus
extern "C" {
#endif

#include "esp_log.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "MotorController.h"

#define ERROR_LINE_TAG "ERROR_LINE"

#define ERROR_LINE (GPIO_NUM_14)

/**
 * ERRORS CODES:
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

 * ___ 0x2xx => MOTION STATES ___
 * 
 * - 0x200 -> MOTOR_NOT_MOVING
 * - 0x201 -> MOTOR_MOVING
 *
 *
 * ___ 0x3xx => ERROR INTERFACE BUS ___
 * 
 * - 0x300 -> LINE_ERRORS_EMPTY
 * - 0x301 -> LINE_ERRORS_MOV_NRANGE
 * - 0x302 -> LINE_ERRORS_ON_MOVING
 * - 0x303 -> LINE_ERRORS_ARM_BLOCK
 * 
 */

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


/**
 * Definisco una struttutra di gestione degli errori con i loro codici univoci
 * es: 0x0112 -> MOVEMENT_ERROR
*/

void setUpErrorLineBus();
void handleErrorIsr();

static int errorState = 0;

#ifdef __cplusplus
}
#endif

#endif