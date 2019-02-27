/**
 * Libreria per la gestione di eventi asyncroni (movimenti e azioni), i quali 
 * necessitano di essere eseguiti asyncronamente in base a una priorità definita.
 * 
 * Essa si basa sull'esecuzione prioritaria di funzioni registrabili
 */

#ifndef ASYNC_QUEUE
#define ASYNC_QUEUE

#ifdef __cplusplus
extern "C" {
#endif

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "DisplayInterface.h"

typedef struct {
    int priority;
    void (* action)(queueParameters_t * parameters);
    queueParameters_t params;
}function_t;

void registerAction(int priority, void (* action)(queueParameters_t * parameters), queueParameters_t * parameters);
void queueInitialize();

#ifdef __cplusplus
}
#endif

#endif