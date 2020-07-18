/* 
    Created by Alessandro Giordano on 03/11/2018
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "MotorController.h"
#include "esp_task_wdt.h"
#include "WdtUtils.h"
#include "DisplayInterface.h"
#include "RobotStructure.h"
#include "ManualController.h"
#include "driver/gpio.h"
#include "freertos/queue.h"
#include "ErrorUtils.h"

extern Motor motors[32];



void librariesInitializer(void);

void execution_task(void * pvParameters) {

    Motor mc = motors[0];


    while(1) {
        /*char buffer[255];
        sprintf(buffer, "jpos1.val=%03d", count);
        sendDisplayStatement(buffer, 13);*/

        /*
        vTaskDelay(10000 / portTICK_PERIOD_MS);
        mc.executeMovement(4500);
        */

        //mc.updateMcParam(ACELERATION, 300);

        /*uint32_t param = mc.getParam(ACELERATION);
        
        ESP_LOGI("MAIN","Param: %d", param);

        param = mc.getParam(KP);
        
        ESP_LOGI("MAIN","Param: %d", param);

        param = mc.getParam(MAX_SPEED);
        
        ESP_LOGI("MAIN","Param: %d", param);

        param = mc.getParam(KI);
        
        ESP_LOGI("MAIN","Param: %d", param);
        vTaskDelay(2000 / portTICK_PERIOD_MS);*/

        while(errorState == 0) {


        
            if(mc.checkMoving() == MOTOR_NOT_MOVING) {
                /*uint32_t mpos = mc.getParam(MENO_POSITION);
                ESP_LOGI("MAIN","Meno position: %d", mpos);*/
                mc.executeMovement(8000);

                vTaskDelay(8000 / portTICK_PERIOD_MS);

                ESP_LOGI("MAIN","Error: %d", mc.getLastError());

                vTaskDelay(2000 / portTICK_PERIOD_MS);

                ESP_LOGI("MAIN","Error: %d", mc.getLastError());

                /*mc.executeMovement(0);

                
                vTaskDelay(4000 / portTICK_PERIOD_MS);
                mc.stopMovement();
                vTaskDelay(2000 / portTICK_PERIOD_MS);
                mc.executeMovement(3000);
                ESP_LOGI("MAIN","Error: %d", mc.getLastError());
                vTaskDelay(13000 / portTICK_PERIOD_MS);
                mc.executeMovement(0);
                vTaskDelay(13000 / portTICK_PERIOD_MS);*/
            }else
                ESP_LOGI("MAIN","Moving: %d", mc.checkMoving());
            

            vTaskDelay(8000 / portTICK_PERIOD_MS);
        }
    }
}

extern "C" void app_main()
{
    wdtInit(WDT_TIMEOUT_S);
    wdtSubscribe(NULL);

    librariesInitializer();


    vTaskDelay(4000 / portTICK_PERIOD_MS);

    xTaskCreate(execution_task, "execution_task", 32768, NULL, configMAX_PRIORITIES, NULL);

    while(1) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        size_t freeMem = heap_caps_get_minimum_free_size(MALLOC_CAP_32BIT);
        //ESP_LOGI("MAIN","Free mem: 0x%x", freeMem);

        wdtReset();
    }
}



void librariesInitializer(void) {
    serialCommunicationInit();
    setUpErrorLineBus();
    InitializeRobotStructure();
    InitializeManualController();
    InitializeDisplay();
}
