# WdtUtils.h

### Libreria per il controllo del WathDog

## Definizioni

*Inizializzazione*: inizializzazione libreria con timeout
```c++
void wdtInit(int timeOutS);
```

<br>

*Subscrive*
```c++
void wdtSubscribe(TaskHandle_t handle);
```

<br>

*Subscrive*
```c++
void wdtUnSubscribe(TaskHandle_t handle);
```

<br>

*Unsubscrive*
```c++
void wdtUnSubscribe(TaskHandle_t handle);
```

<br>

*Deinit*
```c++
void wdtDeinit(void);
```

<br>

*Reset*
```c++
void wdtReset(void);
```

-----------
[Home](generics.md)