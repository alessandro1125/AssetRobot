#ifndef DISPLAY_INTERFACE
#define DISPLAY_INTERFACE

#ifdef __cplusplus
extern "C" {
#endif

#include "NextionUtils.h"
#include "MotorController.h"

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
    char * progressBarId;
}controller_t;

void InitializeDisplay(void);
void DefineController(int displayId, char * progressBarId, Motor * mc, int type);

#ifdef __cplusplus
}
#endif

#endif