# RobotStructure.h
Libreria unicamente specializzata e definita alla definizione della struttura hardware e fisica del sistema di applicazione, nel corrente caso vengono definiti gli assi e i rispettivi Asset ARM devices

## Definizioni

*Definizione istanze degli assi*
```c++
Motor motors[32];
int motorsCount = 7;
```

<br>

*Funzione di inizializzazione*
```c++
void InitializeRobotStructure(void);
```
-----------
[Home](motorcontroller.md)