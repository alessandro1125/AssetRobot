# DisplayInterface.h

### Interfaccia per l'associazione di funzioni e gestione di eventi a pulsanti e label interni al display, strutture e tipi adatti ad identificare e distinguere tipologie di elementi e funzioni

## Definizioni

*Controller*: struttura che identifica la tipologia di controller
```c
typedef struct {
    uint16_t componentId;
    Motor * motor;

    /**
     * 0 -> freccia left
     * 1 -> freccia ricght
     * 2 -> text label
     * 3 -> progress bar
     */
    int type;
    const char * progressBarId;
}controller_t;
```

<br>

*Definizione controller*
```c++
void DefineController(int displayId, const char * progressBarId, Motor * mc, int type);
```

-----------
[Home](display.md)