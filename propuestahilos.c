#include <stdio.h>
#include <pthread.h>
#include <string.h>

//Estructura de una compra
typedef struct {
    char *software;
    int cantidad;
} Compra;

//Inventario inicial
int stock_ubuntu = 20;
int stock_windows = 15;
int stock_linux = 30;

//Declaración de mutex: permite que solo un hilo a la vez acceda a una sección
pthread_mutex_t mutex;

//Función para procesar el pago
void *procesar_pago(void *arg) {
    Compra *compra = (Compra *)arg;

    printf("Cliente:\n\n");
    printf("\tProducto: %d unidades de %s\n", compra->cantidad, compra->software);
    printf("\tProcesando pago.\n");

    return NULL;
}

//Función que actualiza el inventario
void *actualizar_inventario(void *arg) {
    Compra *compra = (Compra *)arg;

    //Bloquear el mutex
    pthread_mutex_lock(&mutex);

    if (strcmp(compra->software, "Ubuntu") == 0) {
        stock_ubuntu -= compra->cantidad;
        printf("\tActualizado inventario de Ubuntu. Quedan %d unidades.\n", stock_ubuntu);
    } else if (strcmp(compra->software, "Windows") == 0) {
        stock_windows -= compra->cantidad;
        printf("\tActualizado inventario de Windows. Quedan %d unidades.\n", stock_windows);
    } else if (strcmp(compra->software, "Linux") == 0) {
        stock_linux -= compra->cantidad;
        printf("\tActualizado inventario de Linux. Quedan %d unidades.\n", stock_linux);
    }

    //Desbloquear el mutex
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main() {
    //Inicializar el mutex
    pthread_mutex_init(&mutex, NULL);

    //Declarar hilos para los 3 clientes
    pthread_t hilos_pago[3], hilos_inventario[3];
    
    //Estructura para 3 clientes
    Compra compras[3] = {
        {"Ubuntu", 5},
        {"Windows", 3},
        {"Linux", 4}
    };

    //Ciclo para crear hilos
    for (int i = 0; i < 3; i++) {
        pthread_create(&hilos_pago[i], NULL, procesar_pago, (void *)&compras[i]);
        pthread_create(&hilos_inventario[i], NULL, actualizar_inventario, (void *)&compras[i]);
    }

    //Que todos los hilos terminen
    for (int i = 0; i < 3; i++) {
        pthread_join(hilos_pago[i], NULL);
        pthread_join(hilos_inventario[i], NULL);
    }
    
    //Destruir el mutex
    pthread_mutex_destroy(&mutex);

    return 0;
}
