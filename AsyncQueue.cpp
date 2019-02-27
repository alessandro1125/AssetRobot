#include "AsyncQueue.h"

/* Lista di funzioni da eseguire */
static function_t functions[256];
static size_t functionsCount = 0;

int initialized = 0;

static void queue_task(void * pvParameters) {
    wdtSubscribe(NULL);
    while(1) {
        if(functionsCount > 0) {
            const int fCount = functionsCount;
            for(size_t i = 0; i < fCount; i++)
            {
                if(functions[i].executed == 0){
                    functions[i].action(&functions[i].params);
                    functions[i].executed = 1;
                }
            }
            int deltaCount = functionsCount - fCount;
            if(deltaCount > 0) {
                for(size_t i = 0; i < deltaCount; i++)
                {
                    functions[i] = functions[i + fCount];
                }
                functionsCount = deltaCount;
            }else {
                functionsCount = 0;
            }
        }
        wdtReset();
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

int searchMaxPriority(function_t * functions, int len) {
    int max = 0;
    for(size_t i = 0; i < len; i++)
    {
        if((functions[i]).executed == 0 && (functions[i]).priority > max) {
            max = (functions[i]).priority;
        }
    }
    return max;
}

void registerAction(int priority, void (* action)(queueParameters_t * parameters), queueParameters_t * parameters) {
    function_t func;
    func.priority = priority;
    func.action = action;
    func.params = ( * parameters);
    functions[functionsCount] = func;
    functions[functionsCount].executed = 0;
    functionsCount++;
}

void queueInitialize() {
    if(initialized == 0) {
        initialized = 1;
        xTaskCreate(queue_task, "queue_handler", 1024*10, NULL, configMAX_PRIORITIES, NULL);
    }
}