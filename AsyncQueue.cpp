#include "AsyncQueue.h"

/* Lista di funzioni da eseguire */
static function_t functions[255];
static size_t functionsCount = 0;

int initialized = 0;

static void queue_task(void * pvParameters) {
    while(1) {
        if (functionsCount > 0) {
            for(size_t i = 0; i < functionsCount; i++)
            {
                functions[i].action(&functions[i].params);
            }
            functionsCount = 0;
        }
    }
    vTaskDelete(NULL);
}

void registerAction(int priority, void (* action)(queueParameters_t * parameters), queueParameters_t * parameters) {
    function_t func;
    func.priority = priority;
    func.action = action;
    func.params = ( * parameters);
    functions[functionsCount] = func;
    functionsCount++;
}

void queueInitialize() {
    if(initialized == 0) {
        initialized = 1;
        xTaskCreate(queue_task, "queue_handler", 1024*10, NULL, configMAX_PRIORITIES, NULL);
    }
}