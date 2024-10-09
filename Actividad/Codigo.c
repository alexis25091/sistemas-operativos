/* Equipo: Sin nombre    Fecha: 27-09-24   Version: 1 */
/* Descripcion: Este programa simula un restaurante donde un número limitado de camareros atiende a varios clientes concurrentemente. 
Utiliza un semáforo para controlar la disponibilidad de los camareros. Cada cliente se representa como un hilo que espera su turno para ser atendido. 
Cuando un camarero está disponible, atiende al cliente y luego libera el semáforo para que otros clientes puedan ser atendidos. 
El programa finaliza una vez que todos los clientes han sido atendidos. */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_CLIENTES 10       		/* Numero de clientes que hay en el restaurante */
#define NUM_CAMAREROS 5       		/* Numero de camareros disponibles */
#define TIEMPO_ATENCION 3     		/* Tiempo que tarda cada camarero en atender a un cliente */

					/* Se hace el semaforo para controlar la cantidad de camareros disponibles */
sem_t semaforoCamareros;

					/* Función que simula cuando un camarero atiende a un cliente */
void* atenderCliente(void* id) {
    int clienteID = *((int*) id);  	/* Se obtiene el id del cliente y se pasa como argumento */
    
   					/* Cuando no hay camareros disponibles el cliente se espera */
    printf("Cliente %d llega al restaurante y espera para ser atendido...\n", clienteID);
    sem_wait(&semaforoCamareros);  	/* El cliente espera hasta que un camarero este disponible */

    					/* El cliente es atendido por un camarero */
    printf("Cliente %d está siendo atendido por un camarero...\n", clienteID);
    sleep(TIEMPO_ATENCION);  		/* se simula lo que tarda en atender el camarero */

    					/* El camarero termina de atender y libera el semaforo */
    printf("Cliente %d ha terminado de ser atendido.\n", clienteID);
    sem_post(&semaforoCamareros);  	/* se libera el camarero para atender otro cliente */

    return NULL;  			/* termina el hilo*/
}

int main() {
    pthread_t hilosClientes[NUM_CLIENTES];  					/* Arreglo de hilos para los clientes */
    int ids[NUM_CLIENTES]; 							/* se guardan los id de los clientes en el arreglo*/

    										/* Se inicializa el semaforo con el numero de camareros disponibles */
    sem_init(&semaforoCamareros, 0, NUM_CAMAREROS);  				/* Valor inicial del semaforo es el numero de camareros disponibles */

			    							/* Se crean los hilos para representar a todos los clientes */
    for (int i = 0; i < NUM_CLIENTES; i++) {
        ids[i] = i + 1;  							/* Se le asigna un identificador a cada cliente (los id)*/
        pthread_create(&hilosClientes[i], NULL, atenderCliente, (void*) &ids[i]);  /* Se crea un hilo para atender al cliente */
    }

    										
    for (int i = 0; i < NUM_CLIENTES; i++) {
        pthread_join(hilosClientes[i], NULL);  					/* esperando a que todos los hilos de los clientes terminen para poder seguir */
    }

    										/* ya atendidos a todos los clientes han sido atendidos se destruye el semaforo */
    sem_destroy(&semaforoCamareros);  						/* Se destruye el semaforo por que ya se terminanron todos los hilos (clientes) */

    return 0;  									/* finaliza el programa */
}
