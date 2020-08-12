#include "TcpCommands.h"

#define TAG "TCP_COMMANDS"

extern Motor motors[32];
extern int motorsCount;
extern int serialBusy;

void getFreeMemory(uint8_t cmd) {
    uint32_t memFree = xPortGetFreeHeapSize();
    //ESP_LOGE(TAG, "Free mem %d", memFree);
    char dataSend[5];

    dataSend[0] = cmd;
    dataSend[1] = (char)memFree;
    dataSend[2] = (char)(memFree >> 8);
    dataSend[3] = (char)(memFree >> 16);
    dataSend[4] = (char)(memFree >> 24);

    sendTcpData(dataSend, 5);
}

void getNumArms(uint8_t cmd) {
    char dataSend[2];

    dataSend[0] = cmd;
    dataSend[1] = (char)motorsCount;

    sendTcpData(dataSend, 2);
}

void getErrorState(uint8_t cmd) {
    char dataSend[2];

    dataSend[0] = cmd;
    dataSend[1] = (char)errorState;

    sendTcpData(dataSend, 2);
}

void getParamTcp(uint8_t cmd, uint8_t armId, uint16_t paramCode) {
    if(armId<motorsCount) {
        char dataSend[5];
        Motor mc = motors[armId-1];
        uint32_t param = mc.getParam(paramCode);

        //ESP_LOGI("MAIN","Param: %d, arm:  %d", paramCode, armId); // TODO

        dataSend[0] = cmd;
        dataSend[1] = (char)param;
        dataSend[2] = (char)(param >> 8);
        dataSend[3] = (char)(param >> 16);
        dataSend[4] = (char)(param >> 24);

        sendTcpData(dataSend, 5);
    }
}

void getAllParams(uint8_t cmd, uint8_t armId) {
    if(armId<motorsCount) {
        Motor mc = motors[armId-1];

        uint32_t p_ACELERATIONTmp        = mc.getParam(DECELERATION);

        /*
        uint32_t p_ACELERATION        = mc.getParam(ACELERATION        );
        uint32_t p_MAX_SPEED          = mc.getParam(MAX_SPEED          );
        uint32_t p_DECELERATION       = mc.getParam(DECELERATION       );
        uint32_t p_ERROR_TOLLERANCE   = mc.getParam(ERROR_TOLLERANCE   );
        uint32_t p_KP                 = mc.getParam(KP                 );
        uint32_t p_KI                 = mc.getParam(KI                 );
        uint32_t p_TIME_TO_ERROR_TOLL = mc.getParam(TIME_TO_ERROR_TOLL );
        uint32_t p_TARGET_POSITION    = mc.getParam(TARGET_POSITION    );
        uint32_t p_POSITION_PROFILE   = mc.getParam(POSITION_PROFILE   );
        uint32_t p_CURRENT_POSITION   = mc.getParam(CURRENT_POSITION   );
        uint32_t p_ERROR              = mc.getParam(ERROR              );
        uint32_t p_PWM_PID            = mc.getParam(PWM_PID            );
        uint32_t p_ON_MOVING          = mc.getParam(ON_MOVING          );
        uint32_t p_MENO_POSITION      = mc.getParam(MENO_POSITION      );
        uint32_t p_MAX_POSITION       = mc.getParam(MAX_POSITION       );
        uint32_t p_ZERO_POSITION      = mc.getParam(ZERO_POSITION      );
        */

        /* EDITOR
        devices.ACELERATION             = getParamDecode(response);
        devices.MAX_SPEED               = getParamDecode(response);
        devices.DECELERATION            = getParamDecode(response);
        devices.ERROR_TOLLERANCE        = getParamDecode(response);
        devices.KP                      = getParamDecode(response);
        devices.KI                      = getParamDecode(response);
        devices.TIME_TO_ERROR_TOLL      = getParamDecode(response);
        devices.TARGET_POSITION         = getParamDecode(response);
        devices.POSITION_PROFILE        = getParamDecode(response);
        devices.CURRENT_POSITION        = getParamDecode(response);
        devices.ERROR                   = getParamDecode(response);
        devices.PWM_PID                 = getParamDecode(response);
        devices.ON_MOVING               = getParamDecode(response);
        devices.MENO_POSITION           = getParamDecode(response);
        devices.MAX_POSITION            = getParamDecode(response);
        devices.ZERO_POSITION           = getParamDecode(response);
        */

        uint32_t p_ACELERATION        = mc.getParam(ACELERATION        );if(p_ACELERATION        == SERIAL_TIMEOUT_ERROR)p_ACELERATION        = mc.getParam(ACELERATION        );
        uint32_t p_MAX_SPEED          = mc.getParam(MAX_SPEED          );if(p_MAX_SPEED          == SERIAL_TIMEOUT_ERROR)p_MAX_SPEED          = mc.getParam(MAX_SPEED          );
        uint32_t p_DECELERATION       = mc.getParam(DECELERATION       );if(p_DECELERATION       == SERIAL_TIMEOUT_ERROR)p_DECELERATION       = mc.getParam(DECELERATION       );
        uint32_t p_ERROR_TOLLERANCE   = mc.getParam(ERROR_TOLLERANCE   );if(p_ERROR_TOLLERANCE   == SERIAL_TIMEOUT_ERROR)p_ERROR_TOLLERANCE   = mc.getParam(ERROR_TOLLERANCE   );
        uint32_t p_KP                 = mc.getParam(KP                 );if(p_KP                 == SERIAL_TIMEOUT_ERROR)p_KP                 = mc.getParam(KP                 );
        uint32_t p_KI                 = mc.getParam(KI                 );if(p_KI                 == SERIAL_TIMEOUT_ERROR)p_KI                 = mc.getParam(KI                 );
        uint32_t p_TIME_TO_ERROR_TOLL = mc.getParam(TIME_TO_ERROR_TOLL );if(p_TIME_TO_ERROR_TOLL == SERIAL_TIMEOUT_ERROR)p_TIME_TO_ERROR_TOLL = mc.getParam(TIME_TO_ERROR_TOLL );
        uint32_t p_TARGET_POSITION    = mc.getParam(TARGET_POSITION    );if(p_TARGET_POSITION    == SERIAL_TIMEOUT_ERROR)p_TARGET_POSITION    = mc.getParam(TARGET_POSITION    );
        uint32_t p_POSITION_PROFILE   = mc.getParam(POSITION_PROFILE   );if(p_POSITION_PROFILE   == SERIAL_TIMEOUT_ERROR)p_POSITION_PROFILE   = mc.getParam(POSITION_PROFILE   );
        uint32_t p_CURRENT_POSITION   = mc.getParam(CURRENT_POSITION   );if(p_CURRENT_POSITION   == SERIAL_TIMEOUT_ERROR)p_CURRENT_POSITION   = mc.getParam(CURRENT_POSITION   );
        uint32_t p_ERROR              = mc.getParam(ERROR              );if(p_ERROR              == SERIAL_TIMEOUT_ERROR)p_ERROR              = mc.getParam(ERROR              );
        uint32_t p_PWM_PID            = mc.getParam(PWM_PID            );if(p_PWM_PID            == SERIAL_TIMEOUT_ERROR)p_PWM_PID            = mc.getParam(PWM_PID            );
        uint32_t p_ON_MOVING          = mc.getParam(ON_MOVING          );if(p_ON_MOVING          == SERIAL_TIMEOUT_ERROR)p_ON_MOVING          = mc.getParam(ON_MOVING          );
        uint32_t p_MENO_POSITION      = mc.getParam(MENO_POSITION      );if(p_MENO_POSITION      == SERIAL_TIMEOUT_ERROR)p_MENO_POSITION      = mc.getParam(MENO_POSITION      );
        uint32_t p_MAX_POSITION       = mc.getParam(MAX_POSITION       );if(p_MAX_POSITION       == SERIAL_TIMEOUT_ERROR)p_MAX_POSITION       = mc.getParam(MAX_POSITION       );
        uint32_t p_ZERO_POSITION      = mc.getParam(ZERO_POSITION      );if(p_ZERO_POSITION      == SERIAL_TIMEOUT_ERROR)p_ZERO_POSITION      = mc.getParam(ZERO_POSITION      );

        /*
        uint32_t p_MAX_SPEED          = mc.getParam(MAX_SPEED          );
        uint32_t p_DECELERATION       = mc.getParam(DECELERATION       );
        uint32_t p_ERROR_TOLLERANCE   = mc.getParam(ERROR_TOLLERANCE   );
        uint32_t p_KP                 = mc.getParam(KP                 );
        uint32_t p_KI                 = mc.getParam(KI                 );
        uint32_t p_TIME_TO_ERROR_TOLL = mc.getParam(TIME_TO_ERROR_TOLL );
        uint32_t p_TARGET_POSITION    = mc.getParam(TARGET_POSITION    );
        uint32_t p_POSITION_PROFILE   = mc.getParam(POSITION_PROFILE   );
        uint32_t p_CURRENT_POSITION   = mc.getParam(CURRENT_POSITION   );
        uint32_t p_ERROR              = mc.getParam(ERROR              );
        uint32_t p_PWM_PID            = mc.getParam(PWM_PID            );
        uint32_t p_ON_MOVING          = mc.getParam(ON_MOVING          );
        uint32_t p_MENO_POSITION      = mc.getParam(MENO_POSITION      );
        uint32_t p_MAX_POSITION       = mc.getParam(MAX_POSITION       );
        uint32_t p_ZERO_POSITION      = mc.getParam(ZERO_POSITION      );*/

        //ESP_LOGI(ERROR_LINE_TAG, "first: %d, %d", p_ACELERATION, p_MAX_SPEED);

        char dataSend[4*16+1];
        dataSend[0] = cmd;
        int len = 1;

        len = addToBuffer(dataSend, p_ACELERATION       , len);
        len = addToBuffer(dataSend, p_MAX_SPEED         , len);
        len = addToBuffer(dataSend, p_DECELERATION      , len);
        len = addToBuffer(dataSend, p_ERROR_TOLLERANCE  , len);
        len = addToBuffer(dataSend, p_KP                , len);
        len = addToBuffer(dataSend, p_KI                , len);
        len = addToBuffer(dataSend, p_TIME_TO_ERROR_TOLL, len);
        len = addToBuffer(dataSend, p_TARGET_POSITION   , len);
        len = addToBuffer(dataSend, p_POSITION_PROFILE  , len);
        len = addToBuffer(dataSend, p_CURRENT_POSITION  , len);
        len = addToBuffer(dataSend, p_ERROR             , len);
        len = addToBuffer(dataSend, p_PWM_PID           , len);
        len = addToBuffer(dataSend, p_ON_MOVING         , len);
        len = addToBuffer(dataSend, p_MENO_POSITION     , len);
        len = addToBuffer(dataSend, p_MAX_POSITION      , len);
        len = addToBuffer(dataSend, p_ZERO_POSITION     , len);


        /*
        for(int i = 0; i < 4*16+1; i++) {
            ESP_LOGI(ERROR_LINE_TAG, "%d", dataSend[i]);
        }
        */

        //ESP_LOGI(ERROR_LINE_TAG, "%x", p_ACELERATION);
        

        sendTcpData(dataSend, 4*16+1);
    }
}

int addToBuffer(char * dataBuffer, uint32_t param, int len) {
    dataBuffer[len++] = (char)param;
    dataBuffer[len++] = (char)(param >> 8);
    dataBuffer[len++] = (char)(param >> 16);
    dataBuffer[len++] = (char)(param >> 24);
    //ESP_LOGI(ERROR_LINE_TAG, "%d Param1: %d, Param2: %d, Param3: %d, Param4: %d,", param, dataBuffer[len-4],dataBuffer[len-3], dataBuffer[len-2],dataBuffer[len-1]);
    return len;
}