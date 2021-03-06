#include <stdio.h>      // libreria estandar
#include <stdlib.h>     // para usar exit y funciones de la libreria standard
#include <string.h>
#include <pthread.h>    // para usar threads
#include <semaphore.h>  // para usar semaforos
#include <unistd.h>

//ANSI colores
#define ANSI_COLOR_RED     "\x1b[31m" 
#define ANSI_COLOR_GREEN   "\x1b[32m" 
//reseteo
#define ANSI_COLOR_RESET   "\x1b[0m"

#define LIMITE 50

pthread_mutex_t salero = PTHREAD_MUTEX_INITIALIZER; //salero

pthread_mutex_t pan = PTHREAD_MUTEX_INITIALIZER; //horno de pan

pthread_mutex_t sarten = PTHREAD_MUTEX_INITIALIZER; //Mutex de sarten

pthread_mutex_t listo1 = PTHREAD_MUTEX_INITIALIZER; //Para saber si esta listo 

pthread_mutex_t escribir = PTHREAD_MUTEX_INITIALIZER; //Para escribir sobre ejecucion

//pthread_mutex_t m_lectura = PTHREAD_MUTEX_INITIALIZER; //Para leer la receta

int panes = 0; // cantidad de panes

int ganador = 0; //Equipo ganador

FILE* ejecucion; //Archivo de ejecucion

FILE* receta;


//creo estructura de semaforos 
struct semaforos {

    sem_t sem_mezclar;
    sem_t sem_salar;
    sem_t sem_pan;
    sem_t sem_verduras;
    sem_t sem_cocinar;
    sem_t sem_armar;
    sem_t sem_listo;


	//poner demas semaforos aqui
};

//creo los pasos con los ingredientes
struct paso {
   char accion [LIMITE];
   char ingredientes[4][LIMITE];
   
};

//creo los parametros de los hilos 
struct parametro {
 int equipo_param;
 struct semaforos semaforos_param;
 struct paso pasos_param[8];
};

//funcion para imprimir las acciones y los ingredientes de la accion
void* imprimirAccion(void *data, char *accionIn) {

	pthread_mutex_lock(&escribir);

	ejecucion = fopen("ejecucion.txt", "a"); //Abro archivo, modo append

	if (ejecucion == NULL){

		printf("Error al abrir archivo\n");

	}



	struct parametro *mydata = data;
	//calculo la longitud del array de pasos 
	int sizeArray = (int)( sizeof(mydata->pasos_param) / sizeof(mydata->pasos_param[0]));
	//indice para recorrer array de pasos 
	int i;
	for(i = 0; i < sizeArray; i ++){
		//pregunto si la accion del array es igual a la pasada por parametro (si es igual la funcion strcmp devuelve cero)
		if(strcmp(mydata->pasos_param[i].accion, accionIn) == 0){
		
		printf("  \n");
		fprintf(ejecucion, "    ");

		printf("\tEquipo %d - accion %s \n " , mydata->equipo_param, mydata->pasos_param[i].accion);
		fprintf(ejecucion, "\tEquipo %d - accion %s \n ", mydata->equipo_param, mydata->pasos_param[i].accion);

		//calculo la longitud del array de ingredientes
		int sizeArrayIngredientes = (int)( sizeof(mydata->pasos_param[i].ingredientes) / sizeof(mydata->pasos_param[i].ingredientes[0]) );
		//indice para recorrer array de ingredientes
		int h;
		printf("\tEquipo %d -----------ingredientes : ----------\n",mydata->equipo_param); 

		fprintf(ejecucion, "\tEquipo %d -----------ingredientes : ----------\n", mydata->equipo_param);

			for(h = 0; h < sizeArrayIngredientes; h++) {
				//consulto si la posicion tiene valor porque no se cuantos ingredientes tengo por accion 
				if(strlen(mydata->pasos_param[i].ingredientes[h]) != 0) {
							printf("\tEquipo %d ingrediente  %d : %s \n",mydata->equipo_param,h,mydata->pasos_param[i].ingredientes[h]);

							fprintf(ejecucion, "\tEquipo %d ingrediente  %d : %s \n",mydata->equipo_param,h,mydata->pasos_param[i].ingredientes[h]);
				}
			}
		}
	}

	printf(" \n"); //Espacio

	fprintf(ejecucion, "   \n" ); //Espacio

	fflush(ejecucion); //Libero memoria

	fclose(ejecucion); //cierro archivo

	pthread_mutex_unlock(&escribir);
}

//funcion para tomar de ejemplo
void* cortar(void *data) {
	//creo el nombre de la accion de la funcion 
	char *accion = "cortar\n";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	usleep( 600000 );
	//doy la se??al a la siguiente accion (cortar me habilita mezclar)
    sem_post(&mydata->semaforos_param.sem_mezclar);

    pthread_exit(NULL);
}

void* guardaGanador(void *data){

	struct parametro *mydata = data;

	sem_wait(&mydata->semaforos_param.sem_listo);

	pthread_mutex_lock(&listo1);

	if (ganador == 0){

		ganador += *((int *) &mydata->equipo_param);

	}

	pthread_mutex_unlock(&listo1);

	pthread_exit(NULL);
}

void* prepararSandwich(void *data){

	char *accion = "preparar sandwich\n";

	struct parametro *mydata = data;

	//Semaforos necesarios para la accion

	sem_wait(&mydata->semaforos_param.sem_verduras);

	sem_wait(&mydata->semaforos_param.sem_pan);

	sem_wait(&mydata->semaforos_param.sem_armar);

	imprimirAccion(mydata, accion);

	//escritura en el archivo

	pthread_mutex_lock(&escribir);

	ejecucion = fopen("ejecucion.txt", "a");

	printf("Equipo: %d preparando sandwich!\n", mydata->equipo_param);

	fprintf(ejecucion, "Equipo: %d preparando sandwich!\n", mydata->equipo_param);

	fclose(ejecucion);

	pthread_mutex_unlock(&escribir);

	//cierre

	usleep(60000);

	sem_post(&mydata->semaforos_param.sem_listo);

	pthread_exit(NULL);

}

void* cocinarMilanesa(void *data){

	char *accion = "cocinar milanesa\n";

	struct parametro *mydata = data;

	//semaforo necesario
	sem_wait(&mydata->semaforos_param.sem_cocinar);

	pthread_mutex_lock(&sarten);

	imprimirAccion(mydata, accion);

	pthread_mutex_lock(&escribir);

	ejecucion = fopen("ejecucion.txt", "a");

	fprintf(ejecucion, "Equipo: %d cocinando la milanesa!!!! \n", mydata->equipo_param);

	printf(ANSI_COLOR_RED "Equipo: %d cocinando la milanesa!!!! \n" ANSI_COLOR_RESET, mydata->equipo_param);

	fclose(ejecucion);

	pthread_mutex_unlock(&escribir);

	usleep(600000);

	pthread_mutex_unlock(&sarten);

	sem_post(&mydata->semaforos_param.sem_armar);

	pthread_exit(NULL);
	
}

void* cortarVerduras(void *data){ //Terminar verduras, revisar

	char *accion = "cortar verduras\n";

	struct parametro *mydata = data;

	imprimirAccion(mydata, accion);

	usleep(60000);

	sem_post(&mydata->semaforos_param.sem_verduras); //Listo verduras

	pthread_exit(NULL);
}



void* cocinarPan(void *data){ //Terminar el pan

	char *accion = "cocinar pan\n";

	struct parametro *mydata = data;

	imprimirAccion(mydata, accion);

	if (panes == 0){

		pthread_mutex_lock(&pan);

		printf("  \n"); //Espacio

		pthread_mutex_lock(&escribir);

		ejecucion = fopen("ejecucion.txt", "a");

		printf(ANSI_COLOR_RED "Equipo: %d cocinando panes.\n" ANSI_COLOR_RESET, mydata->equipo_param);

		fprintf(ejecucion, "\nEquipo: %d cocinando panes.\n" , mydata->equipo_param);

		fprintf(ejecucion, "Equipo: %d se lleva un pan recien horneado!. \n", mydata->equipo_param);

		fclose(ejecucion);

		pthread_mutex_unlock(&escribir);

		printf("  \n"); //Espacio

		panes++;

		printf(ANSI_COLOR_GREEN "Equipo: %d se lleva un pan recien horneado!.\n" ANSI_COLOR_RESET, mydata->equipo_param);

		}

	else{

		panes = 0;		

		pthread_mutex_lock(&escribir);

		ejecucion = fopen("ejecucion.txt", "a");

		fprintf(ejecucion, "Equipo %d, se lleva el ultimo pan horneado :D!\n", mydata->equipo_param);

		printf("  \n"); //Espacio

		printf(ANSI_COLOR_GREEN "Equipo %d, se lleva el ultimo pan horneado :D!\n" ANSI_COLOR_RESET, mydata->equipo_param);

		printf("  \n"); //Espacio

		fclose(ejecucion);

		pthread_mutex_unlock(&escribir);

		usleep(200000);

		pthread_mutex_unlock(&pan);

		}

	sem_post(&mydata->semaforos_param.sem_pan); //listo pan

	pthread_exit(NULL);

}

void* mezclar(void *data){

	char *accion = "mezclar\n";

	struct parametro *mydata = data;

	sem_wait(&mydata->semaforos_param.sem_mezclar);

	imprimirAccion(mydata, accion);

	usleep(600000);

	sem_post(&mydata->semaforos_param.sem_mezclar);

	sem_post(&mydata->semaforos_param.sem_salar); //puede salar

	pthread_exit(NULL);
}

void* salar (void *data){

	char *accion = "empanar\n";

	struct parametro *mydata = data;

	sem_wait(&mydata->semaforos_param.sem_salar);

	pthread_mutex_lock(&salero);

	pthread_mutex_lock(&escribir);

	ejecucion = fopen("ejecucion.txt", "a");

	fprintf(ejecucion, "Equipo %d, usando el salero!\n", mydata->equipo_param);

	printf("  \n"); //Espacio

	printf(ANSI_COLOR_RED "Equipo %d, usando el salero!\n" ANSI_COLOR_RESET, mydata->equipo_param);

	printf("  \n"); //Espacio

	fclose(ejecucion);

	pthread_mutex_unlock(&escribir);

	imprimirAccion(mydata, accion);

	usleep(600000);

	pthread_mutex_unlock(&salero);


	sem_post(&mydata->semaforos_param.sem_salar);
	
	sem_post(&mydata->semaforos_param.sem_cocinar); //habilita cocinar

	pthread_exit(NULL);
}

void* cargarReceta(void *data){

	struct parametro *mydata = data;

	receta = fopen("receta.txt", "r");

	if (receta == NULL){

		printf("Error al abrir la receta. \n");

	}

	int caracteres = 50;

	int contAccion;

	int contIngredientes;

	char palabra[caracteres];

	fgets(palabra, caracteres, receta);

	while(feof(receta) == 0){

		contIngredientes = 0;

		if(strcmp(palabra, "accion\n") == 0){

			fgets(palabra, caracteres, receta);

			strcpy((mydata->pasos_param[contAccion].accion), palabra);

			fgets(palabra, caracteres, receta);

		}

		else if (strcmp(palabra, "ingredientes\n") == 0){

			fgets(palabra, caracteres, receta);

			while(strcmp(palabra, "ingredientes\n") != 0  && strcmp(palabra, "accion\n") != 0 && feof(receta) == 0){

				strcpy((mydata->pasos_param[contAccion].ingredientes[contIngredientes]), palabra);

				fgets(palabra, caracteres, receta);

				contIngredientes++;

			}

			contAccion++;
		}

	}

	fclose(receta);
}


void* ejecutarReceta(void *i) {
	
	//variables semaforos
	sem_t sem_mezclar;
	//crear variables semaforos aqui
	sem_t sem_salar;

	sem_t sem_pan;

	sem_t sem_verduras;

	sem_t sem_cocinar;

	sem_t sem_armar;

	sem_t sem_listo;

	
	//variables hilos
	pthread_t p0; //cargar receta
	pthread_t p1; //cortar
	pthread_t p2; //mezclar
	pthread_t p3; //salar y empanar
	pthread_t p4; //cocinar pan
	pthread_t p5; //cortarVerduras
	pthread_t p6; //cocinar milanga
	pthread_t p7; //preparar sandwich
	pthread_t p8; //Selecciona ganadro

	//crear variables hilos aqui
	
	//numero del equipo (casteo el puntero a un int)
	int p = *((int *) i);
	
	printf("Ejecutando equipo %d \n", p);

	//reservo memoria para el struct
	struct parametro *pthread_data = malloc(sizeof(struct parametro));

	//seteo los valores al struct
	
	//seteo numero de grupo
	pthread_data->equipo_param = p;

	//seteo semaforos
	pthread_data->semaforos_param.sem_mezclar = sem_mezclar;
	//setear demas semaforos al struct aqui
	pthread_data->semaforos_param.sem_salar = sem_salar;

		pthread_data->semaforos_param.sem_pan = sem_pan;

			pthread_data->semaforos_param.sem_verduras = sem_verduras;

				pthread_data->semaforos_param.sem_cocinar = sem_cocinar;

								pthread_data->semaforos_param.sem_armar = sem_armar;

									pthread_data->semaforos_param.sem_listo = sem_listo;

	//seteo las acciones y los ingredientes (Faltan acciones e ingredientes) ??Se ve hardcodeado no? ??Les parece bien?
     	//cargarReceta(pthread_data);

     	//printf(" %s \n", pthread_data->pasos_param[0].accion );
     	//printf(" %s \n", pthread_data->pasos_param[0].ingredientes[0]);

	   //strcpy(pthread_data->pasos_param[4].accion, "cocinar milanesa");



	
	
	//inicializo los semaforos

	sem_init(&(pthread_data->semaforos_param.sem_mezclar),0,0);
	//inicializar demas semaforos aqui
	sem_init(&(pthread_data->semaforos_param.sem_salar), 0, 0);

	sem_init(&(pthread_data->semaforos_param.sem_pan), 0, 0);
	sem_init(&(pthread_data->semaforos_param.sem_verduras), 0, 0);
	sem_init(&(pthread_data->semaforos_param.sem_cocinar), 0, 0);
	sem_init(&(pthread_data->semaforos_param.sem_armar), 0, 0);
	sem_init(&(pthread_data->semaforos_param.sem_listo), 0, 0);





	//creo los hilos a todos les paso el struct creado (el mismo a todos los hilos) ya que todos comparten los semaforos 
    int rc;
    rc = pthread_create(&p0,                           //identificador unico
                            NULL,                          //atributos del thread
                                cargarReceta,             //funcion a ejecutar
                                pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia
	//crear demas hilos aqui

    rc = pthread_create(&p1,                           //identificador unico
                            NULL,                          //atributos del thread
                                cortar,             //funcion a ejecutar
                                pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia
	//crear demas hilos aqui

	rc = pthread_create(&p2,                           //identificador unico
                            NULL,                          //atributos del thread
                                mezclar,             //funcion a ejecutar
                                pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia
	//crear demas hilos aqui

	rc = pthread_create(&p3,                           //identificador unico
                            NULL,                          //atributos del thread
                                salar,             //funcion a ejecutar
                                pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia
	//crear demas hilos aqui

	rc = pthread_create(&p4,                           //identificador unico
                            NULL,                          //atributos del thread
                                cocinarPan,             //funcion a ejecutar
                                pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia
	//crear demas hilos aqui

	rc = pthread_create(&p5,                           //identificador unico
                            NULL,                          //atributos del thread
                                cortarVerduras,             //funcion a ejecutar
                                pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia
	//crear demas hilos aqui

	rc = pthread_create(&p6,                           //identificador unico
                            NULL,                          //atributos del thread
                                cocinarMilanesa,             //funcion a ejecutar
                                pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia
	//crear demas hilos aqui

	rc = pthread_create(&p7,                           //identificador unico
                            NULL,                          //atributos del thread
                                prepararSandwich,             //funcion a ejecutar
                                pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia
	//crear demas hilos aqui

	rc = pthread_create(&p8,                           //identificador unico
                            NULL,                          //atributos del thread
                                guardaGanador,             //funcion a ejecutar
                                pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia
	//crear demas hilos aqui
	
	
	//join de todos los hilos
	pthread_join (p0,NULL);
	pthread_join (p1,NULL);
	//crear join de demas hilos
	pthread_join (p2, NULL);
	pthread_join (p3, NULL);
	pthread_join (p4, NULL);
	pthread_join (p5, NULL);
	pthread_join (p6, NULL);
	pthread_join (p7, NULL);
	pthread_join (p8, NULL);


	//valido que el hilo se alla creado bien 
    if (rc){
       printf("Error:unable to create thread, %d \n", rc);
       exit(-1);
     }

	 
	//destruccion de los semaforos 
	sem_destroy(&sem_mezclar);
	//destruir demas semaforos 
	sem_destroy(&sem_salar);

	sem_destroy(&sem_pan);

	sem_destroy(&sem_cocinar);

	sem_destroy(&sem_pan);

	sem_destroy(&sem_verduras);

	sem_destroy(&sem_listo);


	
	//salida del hilo
	 pthread_exit(NULL);
}


int main ()
{

	ejecucion = fopen("ejecucion.txt", "w");

	if (ejecucion == NULL){

		printf("Error al crear el archivo. \n");
	}

	fclose(ejecucion);
	//creo los nombres de los equipos 
	int rc;
	int *equipoNombre1 =malloc(sizeof(*equipoNombre1));
	int *equipoNombre2 =malloc(sizeof(*equipoNombre2));
//faltan equipos
	int *equipoNombre3 = malloc(sizeof(*equipoNombre3));
	
 
	*equipoNombre1 = 1;
	*equipoNombre2 = 2;
	*equipoNombre3 = 3;
	

	//creo las variables los hilos de los equipos
	pthread_t equipo1; 
	pthread_t equipo2;
	pthread_t equipo3;
	//faltan hilos
  
	//inicializo los hilos de los equipos
    rc = pthread_create(&equipo1,                           //identificador unico
                            NULL,                          //atributos del thread
                                ejecutarReceta,             //funcion a ejecutar
                                equipoNombre1); 

    rc = pthread_create(&equipo2,                           //identificador unico
                            NULL,                          //atributos del thread
                                ejecutarReceta,             //funcion a ejecutar
                                equipoNombre2);
  //faltn inicializaciones

    rc = pthread_create(&equipo3,                           //identificador unico
                            NULL,                          //atributos del thread
                                ejecutarReceta,             //funcion a ejecutar
                                equipoNombre3);
	    

   if (rc){
       printf("Error:unable to create thread, %d \n", rc);
       exit(-1);
     } 

	//join de todos los hilos
	pthread_join (equipo1,NULL);
	pthread_join (equipo2,NULL);
//.. faltan joins
	pthread_join (equipo3, NULL);
	
	//Imprime al ganador	
	printf(ANSI_COLOR_GREEN "EL GANADOR ES EL EQUIPO:  %d !!! FELICIDADES \n" ANSI_COLOR_RESET, ganador);

	pthread_mutex_lock(&escribir);

	ejecucion = fopen("ejecucion.txt", "a");

	fprintf(ejecucion, "EL GANADOR ES EL EQUIPO:  %d !!! FELICIDADES \n", ganador);

	fclose(ejecucion);

	pthread_mutex_unlock(&escribir);

    pthread_exit(NULL);
}


//Para compilar:   gcc subwayArgento.c -o ejecutable -lpthread
//Para ejecutar:   ./ejecutable
