# SerialCommunication.h
Libreria di definizione riferimenti e comandi seriai e di gestione operazioni seriali di livello intermedio

## Definizioni

*Inizializzione libreria*: inizializzazione periferica uart e task di recezione
```c
void serialCommunicationInit(void);
```

<br>

*Handler recezione*: gestisce la recezione ad ogni byte della UART1
```c
void uart1_handler(uint8_t * recivedData, size_t len);
```

<br>

*Update Parameter*: richiesta di invio comando *UPDATE PARAM 0x01* 

Return Values:
* 0 -> update completato
* -1 -> timeout di lettura FEEDBACK
```c
int updateParam(param_t param, uint16_t armAddress);
```

<br>

*Read Parameter*: richiesta di invio comando *READ PARAM 0x02* 

Return Values:
* 0 -> lettura completata
* -1 -> timeout di lettura FEEDBACK
```c
int readParam(param_t * dest, uint8_t address, uint16_t armAddress);
```

<br>

*Execute CMD*: richiesta di invio comando *EXECUTE CMD 0x03* 

Return Values:
 * 0 -> Comando eseguito
 * 1 -> Errore: controllare protocollo
 * -1 -> Timeout di lettura FEEDBACK
```c
int executeCmd(uint8_t cmd, uint16_t armAddress);
```

<br>

*Seriale Libera*: ritorna la disponibilità della processo di trasmissione

Return Values:
 * 0 -> free
 * 1 -> not free
```c
int serialFree(void);
```

<br>

*Clear*: blocca eventuali trasmissioni e libera la seriale, **!attenzione**: può
inceppare il ricevitore, usare con cautela

Return Values:
 * 0 -> free
 * 1 -> not free
```c
void clearSerial();
```
-----------
[Home](README.md)