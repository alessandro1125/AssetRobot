#include "AsyncQueue.h"

/* Lista di funzioni da eseguire */
//void ( * action)(void)[255];

void registerAction(int priority, void ( * action)(void * parameters)) {

}