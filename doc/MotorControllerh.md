# MotorController.h
Classe di interfaccia e gestione di singolo Asset ARM device, funzioni per chiamate ad alto livello con gestione automatica delle librerie sottostanti

## Definizioni

*Classe imoplementata*: classe implementata dalla libreria
```c++
class Motor 
```

<br>

*Definizione id device*: definisco l'id dell'Asset ARM device
```c++
void Motor::setArmId(uint16_t armId);
```

<br>

*Inizializzazione posizione*: Porto l'asse controllato a posizione zero con riferimento finecorsa
```c++
int initPosition();
```

<br>

*Controllo stato di movimento*: ritorna le costanti ```MOTOR_MOVING``` o ```MOTOR_NOT_MOVING``` definite nella *Error Library*
```c++
int checkMoving();
```

<br>

*Invio comando di movimento*: aggiorna la *target position* e comanda l'inizio del movimento, se completato ritorna ```COMMAND_EXECUTED```
```c++
int executeMovement(int32_t position);
```

<br>

*Invio comando di stop movimento*: ferma l'esecuzione del movimento, se correttamente eseguito ritorna ```COMMAND_EXECUTED```
```c++
int stopMovement();
```

<br>

*Richiesta lettura errori*: leggo l'errore interno al device, se presente, lo slot errore viene resettato
```c++
int getLastError();
```

<br>

*Lettura posizione attuale*: ritorna la posizione attuale dell'Asset ARM device
```c++
uint32_t getCurrentPosition();
```

<br>

*Get parameter*: legge il valore del parametro specificato definito in [SerialCommunication.h](SerialCommunication.md)
```c++
uint32_t getCurrentPosition();
```

<br>

*Update parameter*: aggiorna il valore del parametro specificato definito in [SerialCommunication.h](SerialCommunication.md)
```c++
int updateMcParam(uint16_t address, uint32_t value);
```

<br>

*Callback di gestione errori*: gestione errori generati dalla linea seriale per lo stato di errore definito in [ErrorUtils.h](ErrorUtils.md)
```c++
void handleErrorEvent(uint32_t errorCode);
```
-----------
[Home](motorcontroller.md)