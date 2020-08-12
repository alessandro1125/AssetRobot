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
#include "TcpUtils.h"
#include "TcpInterface.h"

extern Motor motors[32];

extern int serialBusy;


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

        int pos = 0;;

        while(errorState == 0) {
            
            /*if(mc.checkMoving() == MOTOR_NOT_MOVING) {
                mc.executeMovement(8000);

                vTaskDelay(8000 / portTICK_PERIOD_MS);

                ESP_LOGI("MAIN","Error: %d", mc.getLastError());

                vTaskDelay(2000 / portTICK_PERIOD_MS);

                ESP_LOGI("MAIN","Error: %d", mc.getLastError());

            }else
                ESP_LOGI("MAIN","Moving: %d", mc.checkMoving());

            //ESP_LOGI("MAIN","Meno position: %d", mc.getParam(MENO_POSITION));*/

            
            

            while(serialBusy!=0);
            serialBusy = 1;
            
            if(mc.checkMoving() == MOTOR_NOT_MOVING && pos==0) {
                pos = 1;
                mc.executeMovement(120);
                
            }

            if(mc.checkMoving() == MOTOR_NOT_MOVING && pos==1) {
                pos = 2;
                mc.executeMovement(240);
                
            }

            if(mc.checkMoving() == MOTOR_NOT_MOVING && pos==2) {
                pos = 0;
                mc.executeMovement(0);
               
            }
            ESP_LOGI("MAIN","Meno position: %d, %d",  mc.getParam(CURRENT_POSITION), mc.getParam(POSITION_PROFILE));
            serialBusy = 0;

            /*
            vTaskDelay(5000 / portTICK_PERIOD_MS);

            ESP_LOGI("MAIN","Max speed: %d", mc.getParam(MAX_POSITION));
            //mc.stopMovement();*/

            /*
            while(serialBusy!=0);
            serialBusy = 1;
            mc.stopMovement();
            serialBusy = 0;

            while(serialBusy!=0);
            serialBusy = 1;
            mc.executeMovement(-2000);
            serialBusy = 0;

            vTaskDelay(5000 / portTICK_PERIOD_MS);

            while(serialBusy!=0);
            serialBusy = 1;
            mc.stopMovement();
            serialBusy = 0;
            */
            

            vTaskDelay(2000 / portTICK_PERIOD_MS);
        }
        
        
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        wdtReset();
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
    InitializeTcpInterface();
}
