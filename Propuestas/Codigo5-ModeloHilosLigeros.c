/* Equipo: Sin nombre    Fecha: 16-09-24   */
/* Ejemplo del Codigo 5. Modelo de Hilos Ligeros */

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
                                /*hilos pesados*/
            /*tiene varias tareas en la cocina en la que se ejecuta 1 hilo por cada preparacion*/
                              /*en este se hace uso de hilos ligeros*/

                                            /*Función para simular todoas las preparaciones*/
void *preparar(void *arg) {
    int id = *((int *) arg);
    switch(id) {
        case 1:
            printf("Cortando verduras...\n");
            sleep(2);                       /*Simula un tiempo en el que se cortan las verduras*/
            printf("Verduras cortadas.\n");
            break;
        case 2:
            printf("Cocinando el arroz...\n");
            sleep(3);                       /*Simula un tiempo en el que se cocina el arroz*/
            printf("Arroz cocido.\n");
            break;
        case 3:
            printf("Preparando la salsa...\n");
            sleep(1);                       /*Simula un tiempo en el que se hace la salsa*/
            printf("Salsa preparada.\n");
            break;
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t hilos[3];                     
    int ids[] = {1, 2, 3};                  /*IDs de las tareas*/

                                            /*Crea los hilos para preparar cada cosa*/
    for (int i = 0; i < 3; i++) {
        pthread_create(&hilos[i], NULL, preparar, (void *) &ids[i]);
    }

                                            /*Esperar la finalización de los hilos*/
    for (int i = 0; i < 3; i++) {
        pthread_join(hilos[i], NULL);
    }

    printf("Todas las preparaciones en la cocina han sido completadas.\n");
    return 0;
}
