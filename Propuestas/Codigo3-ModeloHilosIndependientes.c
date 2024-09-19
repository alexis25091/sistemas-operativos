/* Equipo: Sin nombre    Fecha: 16-09-24   */
/* Ejemplo del Codigo 3 Modelo de Hilos Independientes (M:N Mapping) */

#include <stdio.h>
#include <pthread.h>

// En esta funcion lo que hace es simular como si estuviera tomando el pedido
void *tomar_pedido(void *arg) {
    printf("Tomando el pedido del cliente...\n");
    pthread_exit(NULL);
}

// Aqui lo que hace es para cocinar o preparar la comida que se haya pedido
void *cocinar(void *arg) {
    printf("Cocinando la comida...\n");
    pthread_exit(NULL);
}

// esta funcion lo que hace es para servir el pedido del cliente
void *servir(void *arg) {
    printf("Sirviendo el pedido al cliente...\n");
    pthread_exit(NULL);
}

// Esta funcion es para limpiar la mesa del cliente
void *limpiar(void *arg) {
    printf("Limpiando la mesa despues del cliente...\n");
    pthread_exit(NULL);
}

int main() {
    pthread_t hilos[4];  // Hilos para cada tarea

    // Crea los hilos para cada proceso
    
    pthread_create(&hilos[0], NULL, tomar_pedido, NULL);
    pthread_create(&hilos[1], NULL, cocinar, NULL);
    pthread_create(&hilos[2], NULL, servir, NULL);
    pthread_create(&hilos[3], NULL, limpiar, NULL);

    // Espera a que todos los hilos terminen
    for (int i = 0; i < 4; i++) {
        pthread_join(hilos[i], NULL);
    }


    printf("Orden completada y la mesa esta lista para el siguiente cliente.\n");

    return 0;
}
