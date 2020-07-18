#include "RobotStructure.h"

Motor motors[32];
int motorsCount = 1;

void InitializeRobotStructure(void) {
    // Define motors
    for(size_t i = 0; i < motorsCount; i++)
    {
        motors[i] = Motor();
        motors[i].setArmId(i+1);
    }
}