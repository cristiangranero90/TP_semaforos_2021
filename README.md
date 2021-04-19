## Sistemas operativos y redes I ##
# Trabajo Practico año 2021 #
### "Threads y Semaforos"  ###

### Alumno: Cristian Granero ###

***Introducción***

El siguiente trabajo practico tiene como objetivo la implementacion y la sincronizacion de semaforos e hilos. A partir de ello, se propone una simulacion de una competencia de cocinar sandwichs de milanesa.

En este sentido, se puede decir que cada ***equipo*** (hilos), ejecutaran a la vez todas las acciones necesarias para preparar un sandwich de milanesa. Teniendo en cuenta que algunas acciones no pueden ser realizadas al mismo tiempo que otro equipo, por ejemplo: _usar el salero_, _cocinar el pan_, _utlizar la sarten_. Se emplea el uso de semaforos para sincronizar estas acciones especiales.

***Orden de ejecución***

Para comprender mejor esta solución, resulta importante mostrar cada una de las acciones necesarias para preparar un sandwich.

1.__cortar__, cortar ajo y perejil.
2.__mezclar__, mezclar el ajo, el perejil y el huevo con la carne.
3.__empanar__, ademas, de agregar un poco de sal (contamos con un salero), se van a empanar las milanesas. 
4.__cocinar pan__, obtendremos un pan recien horneado, aunque nuestro horno solo puede cocinar dos panes a la vez.
5.__cortar verduras__, verduras frescas tales como lechuga y tomate son imprescindibles para un sandwich.
6.__cocinar milanesa__, cocinara la milanesa en aceite caliente, sin embargo tener en cuenta que solo hay un sartén disponible.
7.__preparar sandwich__, como ya se han realizado todos los pasos anteriores, solo queda armar el sandwich.
8.__seleccionar al ganador__, por último, y no por ello menos importante, debemos seleccionar al equipo ganador.

  

  

Trabajo Práctico Semáforos primer semestre año 2021
