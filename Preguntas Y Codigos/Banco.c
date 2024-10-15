/* Equipo: Sin nombre    Fecha: 14-10-24   Version: 1*/
/* Codigo 1: Simulacion donde se realizan transacciones bancarias concurrentes utilizando hilos. 
Se efectua un retiro y deposito de manera simultánea sobre una cuenta bancaria con un saldo inicial de $1000. 
El programa crea hilos para cada operación (retiro y depósito), protege las transacciones con el mutex y muestra el saldo actualizado tras cada operación.*/


/* Bibliotecas */
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int saldo = 1000;                               /* Saldo inicial en el banco */
pthread_mutex_t mutex_saldo;                    /* Definimos el mutex*/

void *retirar(void *cantidad) {                 /* Función para simular un retiro */
    int monto = *(int*)cantidad;                /*  */

    pthread_mutex_lock(&mutex_saldo);           /* Bloquear el acceso con mutex */
    
    if (saldo >= monto) {                       /* Realizar el retiro si se tiene el saldo */
        printf("\n\tRetirando $%d...\n", monto);
        sleep(2);
        saldo -= monto;
        printf("Saldo actual: $%d\n\n", saldo);
    } else {
        printf("No hay suficiente saldo para retirar.\n\n");
    }

    pthread_mutex_unlock(&mutex_saldo);         /* Liberar mutex */

    return NULL;
}


void *depositar(void *cantidad) {               /* Función para simular un deposito */
    
    int monto = *(int*)cantidad;

    pthread_mutex_lock(&mutex_saldo);           /* Bloquear el acceso con mutex */
    printf("\n\tDepositando $%d...\n", monto);
    saldo += monto;
    printf("Saldo actual: $%d\n\n", saldo);

    pthread_mutex_unlock(&mutex_saldo);         /* Liberar mutex */

    return NULL;
}


int main() {
    pthread_t hilo_retiro, hilo_deposito;       /* Inicializamos hilos */

    int monto_retiro = 200;                     /* Cantidad a retirar */
    int monto_deposito = 150;                   /* Cantidad a depositar */

    pthread_mutex_init(&mutex_saldo, NULL);     /* Inicializamos el mutex */

    /* Creamos los hilos para ejecutar el retiro y deposito */
    pthread_create(&hilo_retiro, NULL, retirar, &monto_retiro);
    pthread_create(&hilo_deposito, NULL, depositar, &monto_deposito);

    /* Esperamos a que los hilos terminen su ejecucion */
    pthread_join(hilo_retiro, NULL);
    pthread_join(hilo_deposito, NULL);

    pthread_mutex_destroy(&mutex_saldo);           /* Destruimos el mutex */

    return 0;
}
