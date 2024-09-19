/* Equipo: Sin nombre    Fecha: 16-09-24   */
/* Ejemplo del Codigo 4. Modelo de Hilos Pesados */

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

                                /*hilos pesados*/
             /*simula la separacion de ropa en blanca, de color y delicada*/

/*Función que será ejecutada por cada hilo para lavar ropa*/
void *lavar_ropa(void *tipo_ropa) {
    char *ropa = (char *) tipo_ropa;
    printf("Lavando %s...\n", ropa);
    sleep(3); 
    printf("%s está lista!\n", ropa);
    pthread_exit(NULL);
}

int main() {
    pthread_t hilos[3];
    char *tipos_ropa[] = {"Ropa blanca", "Ropa de color", "Ropa delicada"};

    /*Crear hilos para lavar diferentes tipos de ropa*/
    for (int i = 0; i < 3; i++) {
        pthread_create(&hilos[i], NULL, lavar_ropa, (void *) tipos_ropa[i]);
    }

    /*Esperar a que todos los hilos terminen*/
    for (int i = 0; i < 3; i++) {
        pthread_join(hilos[i], NULL);
    }

    printf("Toda la ropa está lista!\n");
    return 0;
}
