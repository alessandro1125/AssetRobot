#include "RobotStructure.h"

Motor * motors;
int motorsCount = 7;

void InitializeRobotStructure(void) {
    // Define motors
    Motor mcs[32];
    for(size_t i = 0; i < motorsCount; i++)
    {
        mcs[i] = Motor();
        mcs[i].setArmId(i+1);
    }
    motors = mcs;
}