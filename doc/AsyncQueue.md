# AsyncQueue.h

### Libreria per l'esecuzione asincrona organizzata di funzioni, l'esempio applicatipo più rilevante rappresente la schedulazione di movimenti dal task di recezione eventi del display (esecuzione movimenti, ...)

## Definizioni

*Funzione*: struttura che identifica l'istanza di esecuzione
```c
typedef struct {
    int priority;
    int executed = 1;
    void (* action)(queueParameters_t * parameters);
    queueParameters_t params;
}function_t;
```

<br>

*Inizializzazione libreria*
```c++
void queueInitialize();
```

<br>

*Schedula azione*
```c++
void registerAction(int priority, void (* action)(queueParameters_t * parameters), queueParameters_t * parameters);
```
-----------
[Home](generics.md)