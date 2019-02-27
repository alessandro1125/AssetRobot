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

void librariesInitializer(void);

extern "C" void app_main()
{
    wdtInit(WDT_TIMEOUT_S);
    wdtSubscribe(NULL);

    librariesInitializer();

    /*Motor mc;
    mc.setArmId(1);
    mc.executeMovement(4000);*/

    int count = 0;

    while(1) {

        if(count < 100)
            count++;
        else
        {
            count = 0;
        }
        
        /*char buffer[255];
        sprintf(buffer, "jpos1.val=%03d", count);
        sendDisplayStatement(buffer, 13);*/


        
        /*
        if(mc.checkMoving() == MOTOR_NOT_MOVING) {
            uint32_t mpos = mc.getParam(MENO_POSITION);
            ESP_LOGI("MAIN","Meno position: %d", mpos);
            mc.executeMovement(4500);
            vTaskDelay(4000 / portTICK_PERIOD_MS);
            mc.stopMovement();
            vTaskDelay(2000 / portTICK_PERIOD_MS);
            mc.executeMovement(8000);
            ESP_LOGI("MAIN","Error: %d", mc.getLastError());
            vTaskDelay(13000 / portTICK_PERIOD_MS);
            mc.executeMovement(0);
            vTaskDelay(13000 / portTICK_PERIOD_MS);
        }else
            ESP_LOGI("MAIN","Moving: %d", mc.checkMoving());
*/

        vTaskDelay(500 / portTICK_PERIOD_MS);


        wdtReset();
    }
}

void librariesInitializer(void) {
    serialCommunicationInit();
    InitializeRobotStructure();
    InitializeManualController();
    InitializeDisplay();
}
