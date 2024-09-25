/*Realizar análisis de codigo y ver que realiza, 
deberan entregar documento de Word explicando paso a paso
*/ 

/*Creacion de librerias*/

 #include<stdio.h> /*Libreria estandar de entrada y salida*/
 #include<stdlib.h> 
 #include<pthread.h> 
 #include<semaphore.h> 
 #include<unistd.h>

 #define MAX_CLIENTES 100 
 #define CLIENTES_POR_FURGON 20

// Definir semaforos y variables de control 
sem_t clientes_en_sucursal; 
sem_t operacion_en_curso; 
pthread_mutex_t mutex;
int clientes = 0;

void* llegada_cliente(void* arg)
{
while (1)
{
    usleep(rand() % 3000000);
    pthread_mutex_lock(&mutex);
    clientes++;
    printf("Cliente entro a la sucursal \nClientes en la sucursal: %d\n",clientes);
    sem_post(&clientes_en_sucursal);
    if (clientes % CLIENTES_POR_FURGON == 0 && clientes <= MAX_CLIENTES)
    {
    printf("\n El furgon blindado esta llegando! La sucursal se esta vaciando. \n\n");
    sem_wait(&operacion_en_curso); 
    for (int i = clientes; i > 0; i--)
    {
        printf("Cliente salio de la sucursal Clientes en la sucursal: %d\n",i-1);
        usleep(500000);
        printf("\nEl furgon blindado puede realizar la operacion ahora. \n");
        sem_post(&operacion_en_curso);
    } 
        pthread_mutex_unlock(&mutex);
        sem_post(&clientes_en_sucursal);
    }
}

}

void* operacion_furgon(void* arg)
{
    while (1){
        sem_wait(&clientes_en_sucursal);
        if(clientes % CLIENTES_POR_FURGON == 0 && clientes <= MAX_CLIENTES)
        {
            sem_wait(&operacion_en_curso);
            printf("Furgon blindado realizando la operacion de carga/descarga de dinero...");
            usleep(rand()%2000000);
            sem_post(&operacion_en_curso);
            printf("Furgon blindado completo la operacion\n");
        }
    }
}





int main()

{
    /*Crea el hilo de clientes y furgón*/
    pthread_t hilo_clientes;
    pthread_t hilo_furgon;
    sem_init(&clientes_en_sucursal, 0,0);/*Inicializa el semaforo clientes en sucursal*/
    sem_init(&operacion_en_curso, 0,1);//Inicializa el semaforo de furgon
    pthread_mutex_init(&mutex, NULL);/*Inicializa mutex*/

    /*Crea un proceso con los hilos clientes/furgon y genera un proceso*/
    pthread_create(&hilo_clientes, NULL, llegada_cliente, NULL);
    pthread_create(&hilo_furgon, NULL, operacion_furgon, NULL);
    /*Espera que termine el hilo de clientes y furgon*/
    pthread_join(hilo_clientes, NULL);
    pthread_join(hilo_furgon, NULL);
    sem_destroy(&clientes_en_sucursal);
    sem_destroy(&operacion_en_curso);
    pthread_mutex_destroy(&mutex);
    return(0);

}

