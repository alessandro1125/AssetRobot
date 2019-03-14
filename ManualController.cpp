#include "ManualController.h"

extern Motor motors[32];

void InitializeManualController(void) {
    Motor * mc;
    mc = &motors[0];
    DefineController(0x0101, "jpos1", mc, 0);
    DefineController(0x0102, "jpos1", mc, 1);
    mc = &motors[1];
    DefineController(0x0105, "jpos2", mc, 0);
    DefineController(0x0106, "jpos2", mc, 1);
    mc = &motors[2];
    DefineController(0x0109, "jpos3", mc, 0);
    DefineController(0x0110, "jpos3", mc, 1);
    mc = &motors[3];
    DefineController(0x0113, "jpos4", mc, 0);
    DefineController(0x0114, "jpos4", mc, 1);
    mc = &motors[4];
    DefineController(0x0117, "jpos5", mc, 0);
    DefineController(0x0118, "jpos5", mc, 1);
    mc = &motors[5];
    DefineController(0x0121, "jpos6", mc, 0);
    DefineController(0x0122, "jpos6", mc, 1);
    mc = &motors[6];
    DefineController(0x0123, "jpos7", mc, 0);
    DefineController(0x0124, "jpos7", mc, 1);
}
