## Sistemas operativos y redes I ##
# Trabajo Practico año 2021 #
### "Threads y Semaforos"  ###

### Alumno: Cristian Granero ###

***Introducción***

El siguiente trabajo practico tiene como objetivo la implementacion y la sincronizacion de semaforos e hilos. A partir de ello, se propone una simulacion de una competencia de cocinar sandwichs de milanesa.

En este sentido, se puede decir que cada ***equipo*** (hilos), ejecutaran a la vez todas las acciones necesarias para preparar un sandwich de milanesa. Teniendo en cuenta que algunas acciones no pueden ser realizadas al mismo tiempo que otro equipo, por ejemplo: _usar el salero_, _cocinar el pan_, _utlizar la sarten_. Se emplea el uso de semaforos para sincronizar estas acciones especiales.

***Orden de ejecución***

Para comprender mejor esta solución, resulta importante mostrar cada una de las acciones necesarias para preparar un sandwich.

1. __Cortar__, cortar ajo y perejil.
2. __Mezclar__, mezclar el ajo, el perejil y el huevo con la carne.
3. __Empanar__, ademas, de agregar un poco de sal (contamos con un salero), se van a empanar las milanesas. 
4. __Cocinar pan__, obtendremos un pan recien horneado, aunque nuestro horno solo puede cocinar dos panes a la vez.
5. __Cortar verduras__, verduras frescas tales como lechuga y tomate son imprescindibles para un sandwich.
6. __Cocinar milanesa__, cocinara la milanesa en aceite caliente, sin embargo tener en cuenta que solo hay un sartén disponible.
7. __Preparar sandwich__, como ya se han realizado todos los pasos anteriores, solo queda armar el sandwich.
8. __Seleccionar al ganador__, por último, y no por ello menos importante, se debe seleccionar al equipo ganador.

_En el codigo:_

```c
void cortar();
void mezclar();
void empanar();
void cocinarPan();
void cortarVerduras();
void cocinarMilanesa();
void prepararSandwich();
void seleccionarGanador();
```
Por otra parte, se emplean semaforos para sincronizar las acciones. Esto es debido a que la mayoria de las acciones requieren de algun paso previo. Es decir, no se podría cocinar una milanesa sin antes haberla empanado.
Semaforos empleados:

```c
    sem_t sem_mezclar; //Habilita a salar
    sem_t sem_salar; //Salar y empanar, habilita a cocinar
    sem_t sem_pan; //Habilita a armar
    sem_t sem_verduras; //Cortadas las verduras, habilita a armar
    sem_t sem_cocinar; //Habilita a armar
    sem_t sem_armar; //Si todas las acciones, fueron realizadas entonces se puede armar el sandwich. Habilita a listo
    sem_t sem_listo; //Una vez todo finalizado, se podra entre otras cosas seleccionar al ganador
    
    //Por otra parte los mutex
    
    pthread_mutex_t salero = PTHREAD_MUTEX_INITIALIZER; //Para conocer si el salero esta en uso

    pthread_mutex_t pan = PTHREAD_MUTEX_INITIALIZER; //Para conocer si algun equipo esta cocinando pan

    pthread_mutex_t sarten = PTHREAD_MUTEX_INITIALIZER; //Al momento de cocinar, se requiere que la sarten no este en uso
```

***Salida***

El programa generara dos salidas, en primera instancia se mostraran 
  

  

Trabajo Práctico Semáforos primer semestre año 2021
