#include <stdio.h>      // libreria estandar
#include <stdlib.h>     // para usar exit y funciones de la libreria standard
#include <string.h>
#include <pthread.h>    // para usar threads
#include <semaphore.h>  // para usar semaforos
#include <unistd.h>

#define LIMITE 50

struct paso {
   char acciones[4][LIMITE];
   char ingredientes[50][LIMITE];
   
};



int main()
{
	FILE *archivo;

	struct paso prueba;

	archivo = fopen ("hola2.txt","r");

	if (archivo == NULL){

		printf("Pronlemas en el archivo. \n");

	}

	//printf("LLego\n");

	//int feof(FILE *archivo);

	int accion;

	//char palabra[LIMITE];

	char nueva[LIMITE];

	fgets(nueva, 20, archivo);

	int numero = 0; //Se debe renovar en cada ciclo, revisar en sub

	while(feof(archivo) == 0){

			if(strcmp(nueva, "accion\n") == 0){

			//printf("Llego primer if\n");

			fgets(nueva, 20, archivo);

			strcpy(prueba.acciones[accion], nueva);

			printf("accion %s \n", prueba.acciones[accion]);

			fgets(nueva, 20, archivo);

			accion++;

			//printf("hasta aca\n");

		}

		else if(strcmp(nueva, "ingredientes\n") == 0){

			fgets(nueva, 20, archivo);

			//printf("else if\n");

			while(strcmp(nueva, "ingredientes\n") != 0  && strcmp(nueva, "accion\n") != 0 && feof(archivo) == 0){

				//printf("segundo while\n");


				strcpy(prueba.ingredientes[numero], nueva);

				printf("ingrediente %s \n", prueba.ingredientes[numero] );

				numero++;

				fgets(nueva, 20, archivo);


			}

			

			
		}

	
	}

	fclose(archivo);

	return 0;
}
