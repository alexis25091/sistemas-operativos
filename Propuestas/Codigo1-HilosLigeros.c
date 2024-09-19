/* Equipo: Sin nombre    Fecha: 16-09-24   */
/* Ejemplo del Código 1. Hilos ligeros.*/

#include <stdio.h>
#include <pthread.h>

// Esta función de un recordatorio de tomar agua
void* recordatorio_agua(void* arg) {
    for (int i = 1; i <= 3; ++i) {  // Repite el recordatorio 3 veces
        printf("Recordatorio: ¡Es hora de tomar agua! (%d)\n", i);
    }
    return NULL;
}

// Aquí es otra función para el recordatorio de descanso
void* recordatorio_descanso(void* arg) {
    for (int i = 1; i <= 3; ++i) {  // Repite el recordatorio 3 veces
        printf("Recordatorio: ¡Es hora de tomar un descanso! (%d)\n", i);
    }
    return NULL;
}

// Esta es una  funcion de recordatorio para hacer ejercicio
void* recordatorio_ejercicio(void* arg) {
    for (int i = 1; i <= 3; ++i) {  // Repite el recordatorio 3 veces
        printf("Recordatorio: ¡Es hora de hacer ejercicio! (%d)\n", i);
    }
    return NULL;
}


int main() {
    // Declaración de identificadores de hilos
    pthread_t hilo1, hilo2, hilo3;

    // Creación de hilos para cada recordatorio
    pthread_create(&hilo1, NULL, recordatorio_agua, NULL);
    pthread_create(&hilo2, NULL, recordatorio_descanso, NULL);
    pthread_create(&hilo3, NULL, recordatorio_ejercicio, NULL);

    // Espera a que todos los hilos terminen
    pthread_join(hilo1, NULL);
    pthread_join(hilo2, NULL);
    pthread_join(hilo3, NULL);

    printf("Todos los recordatorios del día han sido enviados.\n");

    return 0;
}
