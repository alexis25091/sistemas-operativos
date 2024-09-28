/* Equipo: Sin nombre    Fecha: 27-09-24   */
/* Analsis del codigo 3 */

/* Bibliotecas */
#include <stdio.h>        
#include <stdlib.h>       
#include <pthread.h>      /* Incluye la librería para manejar hilos */

pthread_mutex_t mutex;   /* Declara un mutex para proteger el acceso a la variable compartida "saldo" */
int saldo = 0;           /* Inicializa "saldo" en 0 */

void* realizarRetiro(void* arg) {  /* Define la función para retiro del dinero */
    int monto = *(int*)arg;         /* para el monto a retirar */
    pthread_mutex_lock(&mutex);     /* Bloquea el mutex para asegurar que sólo un hilo acceda a "saldo" a la vez */
    printf("Realizando retiro de %d\n", monto);  /* Imprime el monto que se esta retirando */
    saldo -= monto;                  /* Resta el monto del saldo */
    printf("Saldo restante: %d\n", saldo); /* Imprime el saldo restante */
    pthread_mutex_unlock(&mutex);    /* Desbloquea el mutex para que otros hilos puedan acceder a "saldo" */
    pthread_exit(NULL);              /* Termina este hilo */
}

void* realizarDeposito(void* arg) { /* Define la función para el deposito del dinero */
    int monto = *(int*)arg;          /* para el monto a depositar */
    pthread_mutex_lock(&mutex);      /* Bloquea el mutex para asegurar acceso exclusivo a "saldo" */
    printf("Realizando deposito de %d\n", monto); /* Imprime el monto que se está depositando */
    saldo += monto;                  /* Suma el monto al saldo */
    printf("Saldo actual: %d\n", saldo); /* Imprime el nuevo saldo actual*/
    pthread_mutex_unlock(&mutex);    /* Desbloquea el mutex para que otros hilos puedan acceder a "saldo" */
    pthread_exit(NULL);              /* Termina este hilo */


int main() {
    pthread_t hiloRetiro, hiloDeposito; /* Declara dos variables de tipo pthread_t para los hilos */
    int montoRetiro = 100;               /* Inicializa el monto a retirar */
    int montoDeposito = 200;             /* Inicializa el monto a depositar */
    pthread_mutex_init(&mutex, NULL);    /* Inicializa el mutex antes de crear los hilos */
    pthread_create(&hiloRetiro, NULL, realizarRetiro, &montoRetiro); /* Crea el hilo para el retiro */
    pthread_create(&hiloDeposito, NULL, realizarDeposito, &montoDeposito); /* Crea el hilo para el depósito */
    pthread_join(hiloRetiro, NULL);      /* Espera a que el hilo de retiro termine */
    pthread_join(hiloDeposito, NULL);    /* Espera a que el hilo de depósito termine */
    pthread_mutex_destroy(&mutex);        /* Destruye el mutex después de que todos los hilos han terminado */
    return 0;                             /* Finaliza el main */
}
