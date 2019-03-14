#ifndef DISPLAY_INTERFACE
#define DISPLAY_INTERFACE

#ifdef __cplusplus
extern "C" {
#endif

#include "NextionUtils.h"
#include "MotorController.h"
#include "RobotStructure.h"


#define RX_BUFFER_LEN 255

typedef struct {
    uint16_t componentId;
    Motor * motor;

    /**
     * 0 -> freccia left
     * 1 -> freccia ricght
     * 2 -> text label
     * 3 -> progress bar
     */
    int type;
    const char * progressBarId;
}controller_t;

typedef struct {
    int actionType;
    int evType;
    controller_t * controller;
    component_t component;
}queueParameters_t;

void InitializeDisplay(void);
void DefineController(int displayId, const char * progressBarId, Motor * mc, int type);

/* TODO handle cambiamento pagina */

#include "AsyncQueue.h"

#ifdef __cplusplus
}
#endif

#endif