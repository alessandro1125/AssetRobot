# Motor Controller

Libreria di controllo dei motori ad alto livello, basata sull'ausilio della classe **Motor**.

L'interfaccia *ManualController* permette il controllo manuale tramite display . 

#### NOTE: Utilizzare le funzioni di questa libreria in un apposito task, utilizzarle nel task prinicpale comporterebbe il non funzionamento della gestione sincrona della *linea esterna d'errore* (fino ad introduzione della linea di *serial-clear*)

##### TODO: Necessario implementare sistema di inizializzazione finecorsa e ulteriori funzioni di controllo *device ARM*

-------------------

## Library index
* ### [MotorController](MotorController.md)
* ### [RobotStructure](RobotStructure.md)
* ### [ManualController](ManualController.md)

## Library Fratures

- ### Identificazione ARM devices con *armId*
- ### Gestione automatica integrata con la [Serial library](serial.md)
- ### Gestione automatica a callback integrata con la [Error library](error.md)

-------------
[Home](../README.md)