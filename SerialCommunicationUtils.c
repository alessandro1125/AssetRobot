/* 
    Created by Alessandro Giordano on 03/11/2018
*/

#include "SerialCommunicationUtils.h"


/* Protocol decoding variables */
/*
uint8_t decodingState = 0;
size_t  trasmissionLen;
uint8_t bytesRecived[BUFFER_PACKET_RECIVED];
size_t  bytesRecivedCount = 0;
*/


param_t recivedPackets[BUFFER_PACKET_RECIVED];
size_t recivedPacketsCount = 0;




/* decoding packet */
packet_t handlePacketsArray[20];
int handlePacketsCount = 0;




int timeoutFlag = 1;

uint16_t lastAddressCalled = 0xFFFF;
uint16_t lastAddressRecieved = 0xFFFF;


TaskHandle_t timeOutHandler = NULL;

extern int serialBusy = 0;

static int serialMode = 0;

int serialFree(void) {
    return serialMode;
}

/*void clearSerial() {
    serialBusy = 0;
}*/

void uart1_handler(uint8_t * recivedData, size_t len) {

    /*
    * Decodifico il pacchetto
    * Il pacchetto decodificato viene salvato nell'array param_t recivedPakets[1024]
    * una volta ricevuto un paccketto viene attivato un flag
    */
    int i,j;
    param_t param;
    for(i = 0; i < len; i++) {

        if(recivedData[i] == 0x02) {
            packet_t packet;
            packet.decodingState = -1;
            packet.dataRecievedCount = 0;
            packet.activePacket = 1;
            if(handlePacketsCount >= 20 )
                handlePacketsCount = 0;
            handlePacketsArray[handlePacketsCount++] = packet;
        }
        handlePackets(recivedData[i]);
        /*
        //ESP_LOGI("RX_TASK_TAG", "Read 1 bytes: '%d'", recivedData[i]);
        switch(decodingState) {
            case 0:
                if(recivedData[i] == 0x02) {
                    // Trasmission started
                    decodingState = 4;
                    //ESP_LOGI("RX_TASK_TAG","Trasmission Started");
                    //serialBusy = 1;
                }
                break;
            case 4:
                lastAddressRecieved = (uint16_t)recivedData[i];
                lastAddressRecieved = (lastAddressRecieved << 8);
                decodingState++;
                break;
            case 5:
                lastAddressRecieved = lastAddressRecieved | (uint16_t)recivedData[i];
                decodingState = 1;
                break;
            case 1:
                // Get trasmission len
                trasmissionLen = recivedData[i];
                decodingState++;
               // ESP_LOGI("RX_TASK_TAG","Trasmission len: %d", trasmissionLen);
                break;
            case 2:
                // Get trasmission bytes
                if(bytesRecivedCount < trasmissionLen) {
                    bytesRecived[bytesRecivedCount++] = recivedData[i];
                    //ESP_LOGI("RX_TASK_TAG","++BytesRecivedAt %d: %d", bytesRecivedCount-1, recivedData[i]);
                }
                if(bytesRecivedCount == trasmissionLen)
                    decodingState++;
                //ESP_LOGI("RX_TASK_TAG","Trasmission byte: %d", bytesRecived[bytesRecivedCount-1]);
                break;
            case 3:
                // Trasmission end
                if(recivedData[i] == 0x03) {
                    // Save buffer to variable
                    param.address = bytesRecived[0];
                    // Controllo la check
                    uint8_t checkByte = bytesRecived[0];
                    for(j = 1; j < trasmissionLen - 1; j++) { 
                        checkByte += bytesRecived[j];
                        param.value = param.value << 8;
                        param.value |= bytesRecived[j];
                        //ESP_LOGI("RX_TASK_TAG","Var state %d: %d  HEX: %x",j , param.value, param.value);
                    }
                    if (checkByte == bytesRecived[trasmissionLen - 1] && lastAddressCalled == lastAddressRecieved) {// Packet salvato
                        saveParam(param);
                        //serialBusy = 0;
                    }else { // IL packet viene scartato
                        ESP_LOGI("RX_TASK_TAG","Packet wrong checkByte: %d", checkByte);
                    }
                    
                }
                decodingState = 0;
                bytesRecivedCount = 0;
                //serialBusy = 0;
                //ESP_LOGI("RX_TASK_TAG","Trasmission ended");
                break;
            default:break;
        }*/
    }
}


void handlePackets(uint8_t dataByte) {

    //ESP_LOGE("TEST_MAX_SPEED", "%x, %x", address, Motor::asses_id);
    
    //ESP_LOGE("TEST_MAX_SPEED", "byte log %x", dataByte);

    for(int k = 0; k < handlePacketsCount; k++) {
        switch(handlePacketsArray[k].decodingState) {
            case-1: handlePacketsArray[k].decodingState = 0;break;
            case 0:
                handlePacketsArray[k].armIdH = dataByte;
                handlePacketsArray[k].decodingState = 1;
                break;
            case 1:
                handlePacketsArray[k].armIdL = dataByte;
                handlePacketsArray[k].armId = handlePacketsArray[k].armIdH | handlePacketsArray[k].armIdL;
                lastAddressRecieved = handlePacketsArray[k].armId;
                handlePacketsArray[k].decodingState = 2;
                break;
            case 2:
                handlePacketsArray[k].dataLen = dataByte;
                handlePacketsArray[k].decodingState = 3;
                if(dataByte>128) {
                    handlePacketsArray[k].activePacket = 0;
                    freePackets();
                }
                break;
            case 3:
                /*ESP_LOGI("RX_TASK_TAG","packet: %d, state: %d, count %d, len %d", k, handlePacketsArray[k].decodingState, 
                            handlePacketsArray[k].dataRecievedCount, handlePacketsArray[k].dataLen);*/
                if(handlePacketsArray[k].dataRecievedCount < handlePacketsArray[k].dataLen) {
                    handlePacketsArray[k].dataRecieved[handlePacketsArray[k].dataRecievedCount++] = dataByte;
                }
                if(handlePacketsArray[k].dataRecievedCount == handlePacketsArray[k].dataLen) {
                    handlePacketsArray[k].decodingState = 4;
                }
                break;
            case 4:
                if(dataByte == 0x03) {
                    param_t param;

                    param.address = handlePacketsArray[k].dataRecieved[0];
                    param.value = 0;

                    uint8_t checkByte = handlePacketsArray[k].dataRecieved[0];
                    
                    for(int j = 1; j < handlePacketsArray[k].dataLen - 1; j++) { 
                        checkByte += handlePacketsArray[k].dataRecieved[j];
                        param.value = param.value << 8;
                        param.value |= handlePacketsArray[k].dataRecieved[j];
                        //ESP_LOGI("RX_TASK_TAG","Var state %d: %d  HEX: %x",j , param.value, param.value);
                    }
                    
                    if (checkByte == handlePacketsArray[k].dataRecieved[handlePacketsArray[k].dataLen - 1] /*&& lastAddressCalled == lastAddressRecieved*/) {// Packet salvato
                        saveParam(param);
                    }else { // IL packet viene scartato
                        ESP_LOGI("RX_TASK_TAG","Packet wrong checkByte: %x, %x", checkByte, handlePacketsArray[k].dataRecieved[handlePacketsArray[k].dataLen - 1]);
                    }
                }
            default : 
                handlePacketsArray[k].activePacket = 0;
                freePackets();
        }
        //ESP_LOGI("RX_TASK_TAG","packet: %d, state: %d", k, handlePacketsArray[k].decodingState);
    }
}

void freePackets() {
    packet_t tmpPackets[20];
    int tmpCountPackets = 0;
    
    for(int k = 0; k < handlePacketsCount; k++) {
        if(handlePacketsArray[k].activePacket == 1) {
            tmpPackets[tmpCountPackets++] = handlePacketsArray[k];
        }
    }
    //ESP_LOGI("RX_TASK_TAG","packet: %d, %d", handlePacketsCount, tmpCountPackets);
    for(int k = 0; k < tmpCountPackets; k++) {
        handlePacketsArray[k] = tmpPackets[k];
    }
    handlePacketsCount = tmpCountPackets;
}

void timeoutTask(void * pvParameters) {
    vTaskDelay(READING_TIMEOUT_MS / portTICK_PERIOD_MS);
    timeoutFlag = 0;
    vTaskDelete(NULL);
}


int readParam(param_t * dest, uint8_t address, uint16_t armAddress){
    if(serialMode == 0) {
        serialMode = 1;
        //ESP_LOGE("TEST_MAX_SPEED", "Asking param: %x", address);
        gpio_set_level(TX1_ENABLE, 1);
        vTaskDelay(1 / portTICK_PERIOD_MS);
        lastAddressCalled = armAddress;
        uartSendByte(UART_NUM_1, 0x02);
        char armAddresl = (uint16_t)armAddress;
        armAddress = (armAddress >> 8);
        char armAddresh = armAddress;
        uartSendByte(UART_NUM_1, armAddresh);
        uartSendByte(UART_NUM_1, armAddresl);
        uartSendByte(UART_NUM_1, 0x02);
        uartSendByte(UART_NUM_1, address);
        uartSendByte(UART_NUM_1, address);
        uartSendByte(UART_NUM_1, 0x03);
        uartSendByte(UART_NUM_1, 0xFF);
        ESP_ERROR_CHECK(uart_wait_tx_done(UART_NUM_1, 100));
        gpio_set_level(TX1_ENABLE, 0);
        // Create timeout task
        timeoutFlag = 1;
        xTaskCreate(timeoutTask, "timeout_task", 1024, NULL, configMAX_PRIORITIES, &timeOutHandler);
        while(getParam(dest, address) == -1) {
            if(timeoutFlag == 0) {
                ESP_LOGE("RX_TASK_TAG","Reading timout failed");
                serialMode = 0;
                return -1;
            }
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
        serialMode = 0;
        if( timeOutHandler != NULL ) {
            vTaskDelete(timeOutHandler);
        }
        return 0;
    }
    return -2;
}

int updateParam(param_t param, uint16_t armAddress) {
    if(serialMode == 0) {
        serialMode = 1;
        gpio_set_level(TX1_ENABLE, 1);
        vTaskDelay(1 / portTICK_PERIOD_MS);
        lastAddressCalled = armAddress;
        char checkByte = param.address;
        uartSendByte(UART_NUM_1, 0x02);
        char armAddresl = (uint16_t)armAddress;
        armAddress = (armAddress >> 8);
        char armAddresh = armAddress;
        uartSendByte(UART_NUM_1, armAddresh);
        uartSendByte(UART_NUM_1, armAddresl);
        uartSendByte(UART_NUM_1, 0x01);
        uartSendByte(UART_NUM_1, param.address);
        uartSendByte(UART_NUM_1, param.value >> 24);
        uartSendByte(UART_NUM_1, param.value >> 16);
        uartSendByte(UART_NUM_1, param.value >> 8);
        uartSendByte(UART_NUM_1, param.value);
        checkByte += (param.value >> 24);
        checkByte += (param.value >> 16);
        checkByte += (param.value >> 8);
        checkByte += param.value;
        uartSendByte(UART_NUM_1, checkByte);
        uartSendByte(UART_NUM_1, 0x03);
        uartSendByte(UART_NUM_1, 0xFF);
        // Create timeout task
        ESP_ERROR_CHECK(uart_wait_tx_done(UART_NUM_1, 100));
        gpio_set_level(TX1_ENABLE, 0);
        timeoutFlag = 1;
        xTaskCreate(timeoutTask, "timeout_task", 1024, NULL, configMAX_PRIORITIES, &timeOutHandler);
        param_t returnParam;
        while(getParam(&returnParam, 0xFE) == -1) {
            if(timeoutFlag == 0) {
                ESP_LOGE("RX_TASK_TAG","Reading timout failed");
                serialMode = 0;
                return -1;
            }
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
        serialMode = 0;
        if( timeOutHandler != NULL ) {
            vTaskDelete(timeOutHandler);
        }
        return returnParam.value;
    }
    return -2;
}

int executeCmd(uint8_t cmd, uint16_t armAddress) {
    if(serialMode == 0) {
        serialMode = 1;
        gpio_set_level(TX1_ENABLE, 1);
        vTaskDelay(1 / portTICK_PERIOD_MS);
        lastAddressCalled = armAddress;
        uartSendByte(UART_NUM_1, 0x02);
        char armAddresl = (uint16_t)armAddress;
        armAddress = (armAddress >> 8);
        char armAddresh = armAddress;
        uartSendByte(UART_NUM_1, armAddresh);
        uartSendByte(UART_NUM_1, armAddresl);
        uartSendByte(UART_NUM_1, 0x03);
        uartSendByte(UART_NUM_1, cmd);
        uartSendByte(UART_NUM_1, cmd);
        uartSendByte(UART_NUM_1, 0x03);
        uartSendByte(UART_NUM_1, 0xFF);
        ESP_ERROR_CHECK(uart_wait_tx_done(UART_NUM_1, 1000));
        gpio_set_level(TX1_ENABLE, 0);
        // Create timeout task
        timeoutFlag = 1;
        xTaskCreate(timeoutTask, "timeout_task", 1024, NULL, configMAX_PRIORITIES, &timeOutHandler);
        param_t returnParam;
        while(getParam(&returnParam, 0xFE) == -1 && getParam(&returnParam, 0xFF) == -1) { // 0xFE, address to feedback
            if(timeoutFlag == 0) {
                ESP_LOGE("RX_TASK_TAG","Reading timout failed");
                serialMode = 0;
                return -1;
            }
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
        serialMode = 0;
        if( timeOutHandler != NULL ) {
            vTaskDelete(timeOutHandler);
        }
        return returnParam.value;
    }
    return -2;
}

void saveParam(param_t value) {
    recivedPackets[recivedPacketsCount] = value;
    recivedPacketsCount++;
    if(recivedPacketsCount >= BUFFER_PACKET_RECIVED)
        recivedPacketsCount = 0;
}

int getParam(param_t * dest, uint8_t address) {
    int founded = 0;
    for(int k = 0; k < recivedPacketsCount; k++) {
        if(founded == 1) {
            recivedPackets[k - 1] = recivedPackets[k];    
        }
        if(address == recivedPackets[k].address && founded == 0) {
            ( * dest) = recivedPackets[k];
            founded = 1;
        }
    }
    if(founded == 1) {
        recivedPacketsCount--;
        return 0;
    }
    return -1;
}

void serialCommunicationInit(void) {
    /* Uart initialization */
    uartInit(UART_NUM_1);
    startRxTask(UART_NUM_1, uart1_handler);
}