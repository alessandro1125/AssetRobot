#ifndef TCP_COMMANDS
#define TCP_COMMANDS

#ifdef __cplusplus
extern "C" {
#endif

#include "TcpUtils.h"
#include "RobotStructure.h"
#include "ErrorUtils.h"
#include "SerialCommunicationUtils.h"

/**
    Classe di esecuzione comandi rihciesti da Tcp
*/


void getFreeMemory(uint8_t cmd);
void getNumArms(uint8_t cmd);
void getErrorState(uint8_t cmd);
void getParamTcp(uint8_t cmd, uint8_t armId, uint16_t paramCode);
void getAllParams(uint8_t CMD, uint8_t armId);
int addToBuffer(char * dataBuffer, uint32_t param, int len);


#ifdef __cplusplus
}
#endif

#endif