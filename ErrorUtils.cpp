#include "ErrorUtils.h"

extern Motor motors[32];
extern int motorsCount;

/**
* File facoltativo per l'implmentazione di eventuali funzioni
*/

static xQueueHandle gpio_evt_queue = NULL;


static void IRAM_ATTR error_isr_handler(void* arg) {
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

static void error_task_event_handler(void* arg)
{
    uint32_t io_num;
    for(;;) {
        if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            errorState = 1;
            ESP_LOGI(ERROR_LINE_TAG, "Errore rilevato su linea di BUS_ERROR");
            handleErrorIsr();
        }
    }
}

void setUpErrorLineBus() {
    gpio_config_t io_conf;
    //disable interrupt
    io_conf.intr_type = GPIO_INTR_NEGEDGE;
    //set as output mode
    io_conf.mode = GPIO_MODE_INPUT;
    //bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pin_bit_mask = (1ULL << ERROR_LINE);
    /*//disable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 0;*/
    //configure GPIO with the given settings
    gpio_config(&io_conf);

    //create a queue to handle gpio event from isr
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    //start gpio task
    xTaskCreate(error_task_event_handler, "error_line_event", 4096, NULL, 10, NULL);

    // insall isr interrupt
    gpio_install_isr_service(0);
    gpio_isr_handler_add(ERROR_LINE, error_isr_handler, (void*) ERROR_LINE);

}

void handleErrorIsr() {

    while(serialFree()!=0);
    
    // Trovo arm in stato di errore
    int i = 0;
    for(i = 0; i < motorsCount; i++) {
        Motor mc = motors[i];
        uint32_t error = mc.getLastError();
        if(error != LINE_ERRORS_EMPTY) {
            mc.handleErrorEvent(error);
        }
    }
    errorState = 0;
}