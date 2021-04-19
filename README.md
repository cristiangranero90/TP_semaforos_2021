![Universidad Nacional de General Sarmiento](https://github.com/cristiangranero90/TP_semaforos_2021/blob/main/UNGS.jpeg)

## Sistemas operativos y redes I ##
# Trabajo Practico 3, año 2021 #
### "Threads y Semaforos"  ###

### Alumno: Cristian Granero ###

### Comisión: 1 noche ###




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
    sem_t sem_listo; //Una vez que todo ha finalizado, se podra entre otras cosas seleccionar al ganador
    
    //Por otra parte los mutex
    
    pthread_mutex_t salero = PTHREAD_MUTEX_INITIALIZER; //Para conocer si el salero esta en uso

    pthread_mutex_t pan = PTHREAD_MUTEX_INITIALIZER; //Para conocer si algun equipo esta cocinando pan

    pthread_mutex_t sarten = PTHREAD_MUTEX_INITIALIZER; //Al momento de cocinar, se requiere que la sarten no este en uso
```

***Salida***

El programa generara dos salidas, en primera instancia se mostrará por pantalla las acciones que van realizando los equipos independientemente. Un ejemplo de ello es, "Equipo 1, cocinando pan". Nótese, que en la salida todas las secciones criticas seran mostradas por pantalla en color rojo. Otras acciones, como "Equipo 2, se lleva el ultimo pan", se mostraran en verde, indicando, de alguna manera, que en la proxima habrá que cocinar más pan. Y por último aquellas acciones, básicas, se mostrarán en color blanco.

En segunda instancia, todos los pasos mostrados anteriormente, serán guardados en un archivo, en este caso sera el archivo _"ejecucion.txt"_. En este archivo sin embargo, no se guardaran los datos en color, ya que no pueden ser procesados.
  
En ambos casos, al final se mostrará al **ganador** de la competencia.

***Conclusiones***

Si bien, el programa propuesto requirió de determinado tiempo, se logró sincronizar todas las acciones con éxito. Esta solución, sin embargo, no debe considerarse como única. Puesto a que hay muchas soluciones viables.
Además, en este programa se utilizó, lectura y escritura de datos en archivos, y *prints* con colores en la consola, tener en cuenta que sólo será viable en equipos que ejecuten Linux. 

***Otras aclaraciones***

En esta sección se desea ser lo mas claro posible con los archivos provistos en el repositorio.

- **subwayArgento.c**, es el archivo fuente de la competencia.
- **pruebaLectura.c**, archivo fuente con una muestra de lectura de archivos, utilizando *structs* y *arrays*.
- **receta.txt**, contiene todos los pasos a seguir en la receta.
- **ejecuccion.txt**, contiene todos los pasos realizados por cada equipo y por último al ganador de la competencia.
- **hola2.txt**, archivo de prueba para pruebaLectura.c.

Para compilar: 

```bash
gcc subwayArgento.c -o ejec -pthread
```
Para ejecutar:

```bash
./ejec
```

**Tener en cuenta:** en algunas ejecuciones, puede producirse el error _Violación de segmento_, esto es debido a una falla en la lectura de un archivo, el cual se agrega a un arreglo. Por desgracia, y sabiendo que no es una falla recurrente, no se ha podido determinar ni solucionar el inconveniente.








  

Trabajo Práctico Semáforos primer semestre año 2021
