#include "ManualController.h"

extern Motor motors[32];

void InitializeManualController(void) {
    Motor * mc;
    mc = &motors[0];
    DefineController(1, "jpos1", mc, 0);
    DefineController(2, "jpos1", mc, 1);
    mc = &motors[1];
    DefineController(5, "jpos2", mc, 0);
    DefineController(6, "jpos2", mc, 1);
    mc = &motors[2];
    DefineController(9, "jpos3", mc, 0);
    DefineController(10, "jpos3", mc, 1);
    mc = &motors[3];
    DefineController(13, "jpos4", mc, 0);
    DefineController(14, "jpos4", mc, 1);
    mc = &motors[4];
    DefineController(17, "jpos5", mc, 0);
    DefineController(18, "jpos5", mc, 1);
    mc = &motors[5];
    DefineController(21, "jpos6", mc, 0);
    DefineController(22, "jpos6", mc, 1);
    mc = &motors[6];
    DefineController(26, "jpos7", mc, 0);
    DefineController(28, "jpos7", mc, 1);
}
