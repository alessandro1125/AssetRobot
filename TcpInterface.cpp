#include "TcpInterface.h"

static void tcp_rx_handler(char * buffer, size_t len) {

    if(len >= 2) {

        switch(buffer[0]) {
            case GET_ESP32_INFORMATIONS : 
                switch(buffer[1]) {
                    case FREE_MEMORY :
                        getFreeMemory(GET_ESP32_INFORMATIONS);
                        break;
                    case ARM_COUNT :
                        getNumArms(GET_ESP32_INFORMATIONS);
                        break;
                    case ERROR_STATE :
                        getErrorState(GET_ESP32_INFORMATIONS);
                        break;
                    default:break;
                }
                break;
            case GET_DEVICE_PARAM : 
                if(len>=3)
                    getParamTcp(GET_DEVICE_PARAM, buffer[2], buffer[1]);
                break;
            case GET_ALL_PARAMS : 
                if(len>=2)
                    getAllParams(GET_ALL_PARAMS, buffer[1]);
                break;
            default:break;
        }
    }
}

void InitializeTcpInterface() {
    tcpServerConnect();
    registerRxCallback(tcp_rx_handler);
}