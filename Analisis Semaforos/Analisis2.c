/* Equipo: Sin nombre    Fecha: 27-09-24   */
/* Analsis del codigo 2 */

/* Sección de bibliotecas */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define MAX_CLIENTES 100
#define CLIENTES_POR_FURGON 20

// Definir semaforos y variables de control 
sem_t clientes_en_sucursal;     /*Semaforo para clientes*/
sem_t operacion_en_curso;       /*Semaforo para operaciones*/
pthread_mutex_t mutex;          /*Protege secciones del codigo*/

int clientes = 0;

void* llegada_cliente(void* arg) {
    
    while (clientes < MAX_CLIENTES) {
        usleep(rand () % 3000000);              /*Cada cliente tarda un tiempo aleatorio en entrar*/
        pthread_mutex_lock(&mutex);             /*Bloquea acceso para otros hilos*/
        clientes++;                             /*Aumenta el numero de clientes*/
        printf("Cliente entro a la sucursal. Clientes en la sucursal: %d\n", clientes);
        
        /*Si el número de clientes es un múltiplo de 20 y no ha superado el límite de 100*/
        if(clientes % CLIENTES_POR_FURGON == 0 && clientes <= MAX_CLIENTES) {       
            printf("\n¡El furgon blindado esta llegando! La sucursal se esta vaciando...\n\n");
            
            /*Bloquea la operacion*/
            sem_wait(&operacion_en_curso);      /*SO- Para reducir el valor a 0, bloqueado operaciones hasta que*/
            int limite=clientes;
            /*Salida de los clientes, uno por uno*/
            for (int i = limite; i > limite - CLIENTES_POR_FURGON; i--){
                printf("Cliente salio de la sucursal. Clientes en la sucursal: %d\n", i);
                usleep (500000);
            }
            
            printf("\nEl furgon blindado puede realizar la operacion ahora.\n");
            /*Ya que esta vacio, se reanuda la operacion*/
            sem_post(&operacion_en_curso);      /*SO- Libera semaforo, permite nuevas operaciones*/
            
            
        }
        /*Se desbloquea mutes, se puede modificar clientes*/
        pthread_mutex_unlock(&mutex);           /*Desbloquea el acceso */
        sem_post(&clientes_en_sucursal);        /*SC- Incrementa el contador del semaforo. Indica otro cliente*/
    }
}


void* operacion_furgon(void* arg) {
    
    while (clientes <= MAX_CLIENTES) {
        /*Espera hasta que el semáforo clientes tenga un valor positivo, lo que indica que hay clientes 
        en la sucursal listos para ser procesados*/
        sem_wait (&clientes_en_sucursal);       /*SC- Espera que haya suficientes clientes. Si hay procede a la operacion*/
       
       /*Si el número de clientes es un múltiplo de 20 y no se ha superado el límite de 100, el furgón puede operar*/
       if(clientes % CLIENTES_POR_FURGON == 0 && clientes <= MAX_CLIENTES) {
            sem_wait (&operacion_en_curso);     /*SO- Para reducir el valor a 0, bloqueado operaciones hasta que termine*/
            printf("Furgon blindado realizando la operacion de carga/descarga de dinero...\n");
            usleep(rand() % 2000000);
            
            /*Cuando el furgón termina, libera el semáforo operacion, permitiendo que otra operación pueda comenzar si es necesario*/
            sem_post(&operacion_en_curso);      /*SO- Libera semaforo, permite nuevas operaciones*/
            printf("Furgon blindado completo la operacion. \n");
        }
        
        // Salir del bucle si se han alcanzado los 100 clientes
        if (clientes >= MAX_CLIENTES) {
            break;
        }
    }
}

int main() {
    /*Inicializa los 2 hilos*/
    pthread_t hilo_clientes, hilo_furgon;

    sem_init(&clientes_en_sucursal, 0, 0);      /*SC- Inicializacion del semaforo de clientes*/
    /*Se inicializa a 0, el furgón no puede operar hasta que haya al menos un cliente.*/
    sem_init(&operacion_en_curso, 0, 1);        /*SO- Inicializacion del semaforo para la operacion*/
    /*Se inicializa a 1, el furgón puede operar cuando sea necesario.*/
    pthread_mutex_init(&mutex, NULL);           /*Inicializa mutex antes que el hilo*/
    
    /*Se crean los 2 hilos*/
    pthread_create(&hilo_clientes, NULL, llegada_cliente, NULL); 
    pthread_create(&hilo_furgon, NULL, operacion_furgon, NULL);

    /*Espera que termine los 2 hilos*/
    pthread_join(hilo_clientes, NULL);
    pthread_join(hilo_furgon, NULL);

    /*destruye los semaforos*/
    sem_destroy(&clientes_en_sucursal);
    sem_destroy(&operacion_en_curso);
    
    /*destruye el mutex*/
    pthread_mutex_destroy(&mutex);
    printf("si llega");
    return 0;
}


/*Imprime que entran clientes
Furgon llegando
Salen los clientes
Furgon puede realizar operacion 
Carga-descarga
Furgon completado */


/* bucle infinito (while (1))
 while (clientes < MAX_CLIENTES)*/
