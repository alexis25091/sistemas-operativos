/* Equipo: Sin nombre      Fecha: 27-09-24      Version: 2 */
/* ⁠En una nave industrial existe una máquina de inyectar que deja cada pieza producida en una cinta transportadora de 
tamaño limitado. Existe un robot que recoge las piezas de la cinta (una cada vez) y las deja en las cajas de embalaje. 
Finalmente, tenemos un operario que, de vez en cuando, recoge 3 piezas para realizar el control de calidad, 
si no hay tres piezas en la cinta lo intentará más tarde. Resuelve el escenario anterior mediante semáforos */

/* Bibliotecas */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define CINTA_CAPACIDAD 20  /* Capacidad de la cinta transportadora */

/* Estructura para representar la cinta transportadora */
int cinta[CINTA_CAPACIDAD];                         /* Arreglo con tamano de CINTA_CAPACIDAD */
int inicio = 0, fin = 0, cantidad = 0;              /* Inicializacion de variables */
int piezas = 10;                                    /* Produce un numero de piezas aleatorias */

sem_t espacio_disponible;                           /* Semaforo para controlar el espacio disponible en la cinta */
sem_t piezas_disponibles;                           /* Semaforo que controla el numero de piezas disponibles de la cinta */
sem_t mutex_cinta;                                  /* Semaforo para controlar el acceso exclusivo hacia la cinta */


/* Funcion de la maquina de inyeccion */
void* maquina_inyectar(void* arg) {
    
    while (piezas >= 0) {
        sem_wait(&espacio_disponible);              /* Espera a que haya un espacio disponible */

        /* Acceso solo para la cinta */
        sem_wait(&mutex_cinta);
        cinta[fin] = piezas;
        fin = (fin + 1) % CINTA_CAPACIDAD;
        cantidad++;
        printf("\nMaquina : \n\tInyecta piezas %d. \n\tPiezas en cinta: %d\n", piezas, cantidad);
        piezas--;
        
        sem_post(&mutex_cinta);
        sem_post(&piezas_disponibles);              /* Indica si hay una pieza disponible */

        sleep(1);
    }
    return NULL;
}


/* Funcion del robot que recoge las piezas */
void* robot_recoger(void* arg) {

    while (1) {
        sem_wait(&piezas_disponibles);              /* Espera a que haya piezas disponibles */

        /* Otro acceso disponible para la cinta */
        sem_wait(&mutex_cinta);
        int piezas = cinta[inicio];
        inicio = (inicio + 1) % CINTA_CAPACIDAD;
        cantidad--;
        printf("\nRobot : \n\tRecoge pieza %d. \n\tPiezas en cinta: %d\n", piezas, cantidad);
        sem_post(&mutex_cinta);
        sem_post(&espacio_disponible);

        sleep(1);
        if (cantidad == 0) break;  /* Sale del bucle cuando no hay piezas en la cinta */
    }
    return NULL; 
}


/* Funcion del operario de control de calidad */
void* operario_control_calidad(void* arg) {
    while (1) {
        /* Necesita al menos 3 piezas para hacer el control de calidad */
        sem_wait(&piezas_disponibles);
        sem_wait(&piezas_disponibles);
        sem_wait(&piezas_disponibles);

        /* Acceso exclusivo a la cinta */
        sem_wait(&mutex_cinta);
        if (cantidad >= 3) {
            printf("\nOperario: \n\tRealiza control de calidad de 3 piezas. \n\tPiezas restantes en cinta: %d\n", cantidad - 3);
            inicio = (inicio + 3) % CINTA_CAPACIDAD;
            cantidad -= 3;
        }
        sem_post(&mutex_cinta);

        /* Indicar que hay espacio disponible para 3 piezas */
        sem_post(&espacio_disponible);
        sem_post(&espacio_disponible);
        sem_post(&espacio_disponible);

        sleep(2);  /* Simular tiempo de control de calidad */
        if (cantidad == 0) break;  /* Sale del bucle cuando no hay piezas en la cinta */
    }
    return NULL;
}

int main() {
    
    /* Inicializar semaforos */
    sem_init(&espacio_disponible, 0, CINTA_CAPACIDAD);
    sem_init(&piezas_disponibles, 0, 0);
    sem_init(&mutex_cinta, 0, 1);

    /* Inicializar los hilos */
    pthread_t hilo_maquina, hilo_robot, hilo_operario;

    /* Crear los hilos */
    pthread_create(&hilo_maquina, NULL, maquina_inyectar, NULL);
    pthread_create(&hilo_robot, NULL, robot_recoger, NULL);
    pthread_create(&hilo_operario, NULL, operario_control_calidad, NULL);
    
    /* Esperar a que los hilos terminen */
    pthread_join(hilo_maquina, NULL);
    pthread_join(hilo_robot, NULL);
    pthread_join(hilo_operario, NULL);
    printf("\n\naun no destruye");

    /* Destruir semaforos */
    sem_destroy(&espacio_disponible);
    sem_destroy(&piezas_disponibles);
    sem_destroy(&mutex_cinta);
    
    printf("\n\nLLego hasta fin");

    return 0;
}

