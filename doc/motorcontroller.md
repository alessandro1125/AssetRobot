# Motor Controller

Libreria di controllo dei motori ad alto livello, basata sull'ausilio della classe **Motor**

#### NOTE: Utilizzare le funzioni di questa libreria in un apposito task, utilizzarle nel task prinicpale comporterebbe il non funzionamento della gestione sincrona della *linea esterna d'errore* (fino ad introduzione della linea di *serial-clear*)

##### TODO: Necessario implementare sistema di inizializzazione finecorsa e ulteriori funzioni di controllo *device ARM*

-------------------

## Library Fratures

- ### Identificazione ARM devices con *armId*
- ### Gestione automatica integrata con la [Serial library](serial.md)
- ### Gestione automatica a callback integrata con la [Error library](error.md)