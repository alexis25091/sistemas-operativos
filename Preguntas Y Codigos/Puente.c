/* Equipo: Sin nombre    Fecha: 14-10-24   Version: 1*/
/* Codigo 3: Este código simula el cruce de vehículos en un puente de un solo carril donde los vehículos se desplazan en dos sentidos: A y B. 
Se utilizan hilos para representar 100 vehículos, con 50 vehículos en cada sentido. 
Los vehículos deben esperar si hay otros vehículos cruzando en el sentido opuesto o si otros vehículos están esperando para cruzar.
Para sincronizar el cruce, el código usa un mutex que garantiza que los vehículos crucen el puente en un sentido a la vez, evitando colisiones. 
Los vehículos esperan su turno basándose en la cantidad de vehículos cruzando o esperando en el sentido contrario. */

#include <stdio.h>      												// Para printf()
#include <stdlib.h>     												// Funciones para rand(), malloc(), free(), etc.
#include <pthread.h>    												// Para el manejo de hilos
#include <semaphore.h>  												// Para el uso de semáforos
#include <unistd.h>     												// Funcion como sleep() y usleep()


#define NUM_VEHICULOS 100          										// Número total de vehículos (hilos)
#define NUM_VEHICULOS_SENTIDO 50   										// Número de vehículos por cada sentido (A y B)


sem_t puente;										                    // Semaforo para controlar acceso al puente
int CarSentidoA = 0;										            // Vehiculos esperando en sentido A
int CarSentidoB = 0;            										// Vehiculos esperando en sentido B
int cruzando_A = 0;             										// Vehiculos cruzando en sentido A
int cruzando_B = 0;											            // Vehiculos cruzando en sentido B
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  					// Mutex para modificar variables

void *vehiculo_sentido_A(void *arg) {
    int id = *(int*)arg;
    sleep(rand() % 20); 												 // Tiempo aleatorio antes de intentar cruzar

    // El carro intenta entrar al puente
    
    pthread_mutex_lock(&mutex);
    CarSentidoA++;
    while (cruzando_B > 0 || (CarSentidoB > 0 && cruzando_A == 0)) {
    	
        // Aqui espera mientras haya vehiculos del sentido B cruzando o esperando
        
        pthread_mutex_unlock(&mutex);
        sleep(2);  // Pequeña espera para reducir CPU
        pthread_mutex_lock(&mutex);
	}
    
    CarSentidoA--;
    cruzando_A++;
    pthread_mutex_unlock(&mutex);

    // Cruza el puente
    
    printf("Vehiculo %d del sentido A esta cruzando el puente.\n", id);
    sleep(5);  // Simulacion de cruce de 1 segundo
    printf("Vehiculo %d del sentido A ha dejado el puente.\n", id);

    // Termina de cruzar
    
	pthread_mutex_lock(&mutex);
    cruzando_A--;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

void *vehiculo_sentido_B(void *arg) {
	
    int id = *(int*)arg;
    sleep(rand() % 20);  // Tiempo aleatorio antes de intentar cruzar

    // El vehiculo intenta entrar al puente
    
	pthread_mutex_lock(&mutex);
    CarSentidoB++;
    
    while (cruzando_A > 0 || (CarSentidoA > 0 && cruzando_B == 0)) {
    	
        // Espera aqui mientras haya vehiculos del sentido A cruzando o esperando
        pthread_mutex_unlock(&mutex);
        sleep(2);  // Pequeña espera para reducir CPU
        pthread_mutex_lock(&mutex);
    }
    CarSentidoB--;
    cruzando_B++;
    pthread_mutex_unlock(&mutex);

    // Cruza el puente
    
	printf("Vehiculo %d del sentido B esta cruzando el puente.\n", id);
    sleep(5);  
    printf("Vehiculo %d del sentido B ha dejado el puente.\n", id);

    // Termina de cruzar
    
    pthread_mutex_lock(&mutex);
    cruzando_B--;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main() {
    
	pthread_t hilos[NUM_VEHICULOS];
    int ids[NUM_VEHICULOS];
    

    // Crear 50 hilos para el sentido A y 50 para el sentido B
    
    
    for (int i = 0; i < NUM_VEHICULOS_SENTIDO; i++) {
        ids[i] = i;
        pthread_create(&hilos[i], NULL, vehiculo_sentido_A, &ids[i]);
    }
    for (int i = NUM_VEHICULOS_SENTIDO; i < NUM_VEHICULOS; i++) {
        ids[i] = i;
        pthread_create(&hilos[i], NULL, vehiculo_sentido_B, &ids[i]);
    }

    // Esperar que todos los hilos terminen
    for (int i = 0; i < NUM_VEHICULOS; i++) {
        pthread_join(hilos[i], NULL);
    }

    printf("Todos los vehiculos han cruzado el puente.\n");

    return 0;
}
