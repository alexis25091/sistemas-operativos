/* Equipo: Sin nombre    Fecha: 27-09-24   */
/* Analsis del codigo 1 */

/*Declaracion de librerias*/
#include<stdio.h>/*Libreria estandar de entrada y salida*/
#include<pthread.h>/*Libreria para el uso de hilos*/
#include<semaphore.h>/*Libreria para el uso de semaforos*/

sem_t mutex;/*Declaracion de un semaforo como variable global*/


/*Funcion para el hilo*/
void* thread_func(void* arg) {
    sem_wait(&mutex);/*El semaforo espera a que el hilo este disponible para su uso*/
    printf("Sección crítica del hilo\n");/*Muestra el mensaje de operacion del hilo*/
    sem_post(&mutex);/*El hilo libera al semaforo permitiendo que se pueda haver uso nuevamente*/
    return NULL;/*El hilo termina su ejecucion y avisa a la funcion retornando NULL*/
}

int main() {/*Funcion principal*/
    pthread_t thread;/*Variable que almacena el identificador del hilo*/

    sem_init(&mutex, 0, 1);/*Inicializacion del semaforo llamado "mutex"*/

    pthread_create(&thread, NULL, thread_func, NULL);/*Crea un nuevo hilo y llama a la funcion thread_fun*/
    
    /*Empieza la seccion critica del hilo principal*/

    sem_wait(&mutex);/*El hilo llamado mutex espera a que el semaforo este disponible*/
    printf("Sección crítica del hilo principal\n");/*Mensaje de ejecucion del hilo principal*/
    sem_post (&mutex);/*El hilo principal libera el semaforo*/

    pthread_join (thread, NULL);/*Espera a que el hilo creado termine au ejecucion*/
    sem_destroy(&mutex);/*Libera o destruye el semaforo.*/
    return 0;/*Avisa a la funcion principal que concluyo con exito*/
}
