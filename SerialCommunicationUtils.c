/* 
    Created by Alessandro Giordano on 03/11/2018
*/

#include "SerialCommunicationUtils.h"


/* Protocol decoding variables */
uint8_t decodingState = 0;
size_t  trasmissionLen;
uint8_t bytesRecived[BUFFER_PACKET_RECIVED];
size_t  bytesRecivedCount = 0;

param_t recivedPackets[BUFFER_PACKET_RECIVED];
size_t recivedPacketsCount = 0;

int timeoutFlag = 1;

uint16_t lastAddressCalled = 0xFFFF;
uint16_t lastAddressRecieved = 0xFFFF;


TaskHandle_t timeOutHandler = NULL;

int serialBusy = 0;

int serialFree(void) {
    return serialBusy;
}

void uart1_handler(uint8_t * recivedData, size_t len) {
    /*
    * Decodifico il pacchetto
    * Il pacchetto decodificato viene salvato nell'array param_t recivedPakets[1024]
    * una volta ricevuto un paccketto viene attivato un flag
    */
    int i,j;
    param_t param;
    for(i = 0; i < len; i++) {
        //ESP_LOGI("RX_TASK_TAG", "Read 1 bytes: '%d'", recivedData[i]);
        switch(decodingState) {
            case 0:
                if(recivedData[i] == 0x02) {
                    // Trasmission started
                    decodingState = 4;
                    //ESP_LOGI("RX_TASK_TAG","Trasmission Started");
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
                    if (checkByte == bytesRecived[trasmissionLen - 1] && lastAddressCalled == lastAddressRecieved) // Packet salvato
                        saveParam(param);
                    else { // IL packet viene scartato
                        ESP_LOGI("RX_TASK_TAG","Packet wrong checkByte: %d", checkByte);
                    }
                    /*ESP_LOGI("RX_TASK_TAG","Trasmission Decoded: address %d - value: %d", param.address
                            , param.value);*/
                }
                decodingState = 0;
                bytesRecivedCount = 0;
                //ESP_LOGI("RX_TASK_TAG","Trasmission ended");
                break;
            default:break;
        }
    }
}

void timeoutTask(void * pvParameters) {
    vTaskDelay(READING_TIMEOUT_MS / portTICK_PERIOD_MS);
    timeoutFlag = 0;
    vTaskDelete(NULL);
}


int readParam(param_t * dest, uint8_t address, uint16_t armAddress){
    if(serialBusy == 0) {
        serialBusy = 1;
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
        // Create timeout task
        timeoutFlag = 1;
        xTaskCreate(timeoutTask, "timeout_task", 1024, NULL, configMAX_PRIORITIES, &timeOutHandler);
        while(getParam(dest, address) == -1) {
            if(timeoutFlag == 0) {
                ESP_LOGI("RX_TASK_TAG","Reading timout failed");
                serialBusy = 0;
                return -1;
            }
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
        serialBusy = 0;
        if( timeOutHandler != NULL ) {
            vTaskDelete(timeOutHandler);
        }
        return 0;
    }
    return -2;
}

int updateParam(param_t param, uint16_t armAddress) {
    if(serialBusy == 0) {
        serialBusy = 1;
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
        // Create timeout task
        timeoutFlag = 1;
        xTaskCreate(timeoutTask, "timeout_task", 1024, NULL, configMAX_PRIORITIES, &timeOutHandler);
        param_t returnParam;
        while(getParam(&returnParam, 0xFE) == -1) {
            if(timeoutFlag == 0) {
                ESP_LOGI("RX_TASK_TAG","Cmd timout failed");
                serialBusy = 0;
                return -1;
            }
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
        serialBusy = 0;
        if( timeOutHandler != NULL ) {
            vTaskDelete(timeOutHandler);
        }
        return returnParam.value;
    }
    return -2;
}

int executeCmd(uint8_t cmd, uint16_t armAddress) {
    if(serialBusy == 0) {
        serialBusy = 1;
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
        // Create timeout task
        timeoutFlag = 1;
        xTaskCreate(timeoutTask, "timeout_task", 1024, NULL, configMAX_PRIORITIES, &timeOutHandler);
        param_t returnParam;
        while(getParam(&returnParam, 0xFE) == -1 && getParam(&returnParam, 0xFF) == -1) { // 0xFE, address to eedback
            if(timeoutFlag == 0) {
                ESP_LOGI("RX_TASK_TAG","Cmd timout failed");
                serialBusy = 0;
                return -1;
            }
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
        serialBusy = 0;
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