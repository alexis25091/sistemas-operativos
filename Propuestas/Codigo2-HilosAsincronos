/* Equipo: Sin nombre    Fecha: 16-09-24   */
/* Ejemplo del Codigo 2 - Hilos asíncronos. */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // Para usar sleep()

/*Estructura para pasar múltiples argumentos a la función del hilo */
typedef struct {
    int thread_id;
    char* message;
} ThreadArgs;

/*Función que se ejecutará en el hilo de cereal*/
void* threadFunction1(void* args) {
    ThreadArgs* thread_args = (ThreadArgs*)args;
    printf("Hilo %d: %s...\n", thread_args->thread_id, thread_args->message);
    sleep(2); // Simula el tiempo que lleva verter el cereal
    printf("Hilo %d: Cereal vertido.\n", thread_args->thread_id);
    pthread_exit(NULL); // Terminar el hilo
}

/*Función que se ejecutará en el hilo de leche*/
void* threadFunction2(void* args) {
    ThreadArgs* thread_args = (ThreadArgs*)args;
    printf("Hilo %d: %s...\n", thread_args->thread_id, thread_args->message);
    sleep(2); // Simula el tiempo que lleva verter la leche
    printf("Hilo %d: Leche vertida.\n", thread_args->thread_id);
    pthread_exit(NULL); // Terminar el hilo
}

int main() {
    /*Definir la estructura de argumentos para cada hilo*/
    ThreadArgs args1 = {1, "Vertiendo cereal"};
    ThreadArgs args2 = {2, "Vertiendo leche"};

    /*Declarar las variables de hilo*/
    pthread_t thread1, thread2;

    // Crear los hilos y pasar las funciones junto con los argumentos
    if (pthread_create(&thread1, NULL, threadFunction1, (void*)&args1) != 0) {
        perror("No hay cereal"); 	/*Imprimir error si falla la creación del hilo*/
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&thread2, NULL, threadFunction2, (void*)&args2) != 0) {
        perror("No hay leche"); 	/*Imprimir error si falla la creación del hilo*/
        exit(EXIT_FAILURE);
    }

    // Esperar a que los hilos terminen su ejecución
    if (pthread_join(thread1, NULL) != 0) {
        perror("No se preparó el cereal"); /*Imprimir error si falla la creación del hilo*/
        exit(EXIT_FAILURE);
    }

    if (pthread_join(thread2, NULL) != 0) {
        perror("No se preparó el cereal"); /*Imprimir error si falla la creación del hilo*/
        exit(EXIT_FAILURE);
    }

    // Imprimir mensaje del hilo principal
    printf("Se preparó el cereal con éxito.\n");
    return 0;
}
