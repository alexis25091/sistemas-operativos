/* Equipo: Sin nombre      Fecha: 2-10-24      Version: 1 */
/* ⁠Este programa simula la interacción de 1000 soldados que pasan por un comedor, gestionando el acceso a los recursos compartidos como mostradores de comida, 
abridores de botellas, y mostradores de postres mediante el uso de hilos (threads) y semáforos.  */


#include <stdio.h>//Para la entrada y salida estandar
#include <stdlib.h>//Para el uso de funciones de manejo de memoria dinamica y otras utilidades.
#include <pthread.h>//Para crear y gestionar hilos.
#include <semaphore.h>//Para el manejo de semaforos, que se utilizan para controlar el acceso a recursos compartidos.
#include <unistd.h>//Para funciones como sleep, que detiene la ejecucion durante un tiempo.

#define SOLDADOS 1000//Numero de soldados que van a pasar por el comedor.
#define MOSTRADORES_COMIDA 5//Numero de mostradores de comida disponibles
#define ABRIDORES 60//Numero de abridores de botellas disponibles
#define MOSTRADORES_POSTRES 5//Numero de mostradores de postres disponibles

sem_t sem_mostradores_comida;//Controla el acceso a los mostradores de comida
sem_t sem_abre_botellas;//Controla el acceso a los abridores de botellas
sem_t sem_mostradores_postres;//Controla el acceso a los mostradores de postres

void* soldado(void* arg) {
//simula las acciones de un soldado cuando entra al comedor. Se ejecuta en un hilo separado para cada soldado.
    int id = ((int)arg);
    free(arg); 
    printf("Soldado %d entra al comedor.\n", id);
    sem_wait(&sem_mostradores_comida);//Espera su turno para acceder a un mostrador de comida
    printf("Soldado %d toma una bandeja de comida.\n", id);
    sleep(1);//Una vez que tiene su comida, se pausa por 1 segundo

    if (rand() % 2) { 
/*Decide de forma aleatoria si toma agua o un refresco (con un 50% de probabilidad). 
Si toma un refresco, usa un abridor, controlado por el semaforo sem_abre_botellas.*/
        sem_wait(&sem_abre_botellas);
        printf("Soldado %d toma un botellín de refresco y un abridor.\n", id);
        sleep(1); 
        sem_post(&sem_abre_botellas); 
    } else {
        printf("Soldado %d toma un vaso de agua.\n", id);
    }
    //Espera su turno para acceder al mostrador de postres con:
    sem_wait(&sem_mostradores_postres);
    printf("Soldado %d va al mostrador de postres.\n", id);
    sleep(1); 
    /*Despues de tomar su postre, sale del comedor, liberando los recursos 
    (mostradores de comida y postres) usando sem_post.*/
    sem_post(&sem_mostradores_postres); 
    printf("Soldado %d finaliza su comida y sale del comedor.\n", id);
    sem_post(&sem_mostradores_comida); 
    return NULL;
}

int main() {

    //Inicializa los semaforos con valores que indican cuantos recursos estan disponibles. 
    srand(time(NULL)); 
    pthread_t soldados[SOLDADOS];

    sem_init(&sem_mostradores_comida, 0, MOSTRADORES_COMIDA);
    sem_init(&sem_abre_botellas, 0, ABRIDORES);
    sem_init(&sem_mostradores_postres, 0, MOSTRADORES_POSTRES);

    for (int i = 0; i < SOLDADOS; i++) {
        //A cada hilo se le pasa un identificador unico (ID) que se asigna dinamicamente usando malloc.
        int* id = malloc(sizeof(int)); 
        *id = i + 1; 
        pthread_create(&soldados[i], NULL, soldado, id);
    }

    for (int i = 0; i < SOLDADOS; i++) {
        //Espera a que todos los hilos (soldados) terminen su ejecucion usando pthread_join.
        pthread_join(soldados[i], NULL);
    }
    
    /*Despues de que todos los soldados han comido, destruye 
    los semaforos y muestra un mensaje final indicando que todos los soldados han terminado.*/
    sem_destroy(&sem_mostradores_comida);
    sem_destroy(&sem_abre_botellas);
    sem_destroy(&sem_mostradores_postres);

    printf("Todos los soldados han comido y han salido del comedor.\n");
    return 0;
}
