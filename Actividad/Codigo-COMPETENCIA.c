/* Equipo: Sin nombre    Fecha: 10-10-24   Version: 3 */
/* Este código simula la llegada de clientes a una sucursal y la operación de un furgón blindado que retira dinero 
cuando hay una cantidad específica de clientes. Utiliza hilos (threads), semáforos, mutex, swap, tuberías , 
proceso(fork) y memoria compartida para sincronizar las acciones entre los clientes y el furgón. */

#include <stdio.h>     
#include <stdlib.h> 
#include <pthread.h> 
#include <semaphore.h> 
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>   /* PARA USAR EN MEMORIA COMPARTIDA */
#include <sys/shm.h>    /* PARA USAR EN MEMORIA COMPARTIDA */
#include <sys/types.h>  /* PARA FORK () */

// Declaración de semáforos y variables globales
/* SEMAFOROS */
sem_t clientes_en_sucursal;       // Semáforo para contar clientes en la sucursal
sem_t operacion_en_curso;         // Semáforo para controlar cuándo el furgón puede operar
/* MUTEX */
pthread_mutex_t mutex;            // Mutex para asegurar que solo un hilo modifique el número de clientes a la vez

/* VARIABLE PARA USAR MEMORIA COMPARTIDA */
int *clientes = 0;                // Contador global de clientes en la sucursal

/* VARIABLES SWAP */
int MAX_CLIENTES =5;
int CLIENTES_POR_FURGON =20;

/* DECLARACION DE TUBERIA */
int tuberias[2];   

/* FUNCION SWAP */
void swap(int* x, int* y) {
    int temp = *x;
    *x = *y;
    *y = temp;
}

// Función que simula la llegada de clientes a la sucursal
void* llegada_cliente(void* arg) {
    char mensaje[100]; /* MENSAJE DE TUBERIA */
    
    while (*clientes < MAX_CLIENTES) { /* MODIFICAMOS EL CICLO */
        usleep(rand() % 3000000);       // Pausa el hilo por un tiempo aleatorio hasta 3 segundos (simula tiempo de llegada de cliente)

        pthread_mutex_lock(&mutex);     // Bloquea el acceso a la variable "clientes" (para que otros hilos no la modifiquen al mismo tiempo)
        (*clientes)++;  /* SE PASA A CADA HILO PARA MODIFICARSE */
        printf("Cliente entró a la sucursal. Clientes en la sucursal: %d\n", *clientes); // Muestra cuántos clientes hay

        /* SNPRINTF ES PARA IMPRIMIR PROCESOS PADRES E HIJOS */
        /* PARA VER QUE ENTRE EN LA TUBERIA */
        snprintf(mensaje, sizeof(mensaje), "El cliente %d entro\n", *clientes);
        write(tuberias[1], mensaje, strlen(mensaje) + 1);  /* MENSAJE DE LA TUBERIA */
        
        // Si el número de clientes es múltiplo de CLIENTES_POR_FURGON (es decir, 20) y no supera el límite máximo
        if (*clientes % CLIENTES_POR_FURGON == 0 && *clientes <= MAX_CLIENTES) {
            printf("\n¡ El furgón blindado está llegando! La sucursal se está vaciando...\n\n");
            sem_wait(&operacion_en_curso); // Espera hasta que el furgón termine su operación actual
            
            int limite=*clientes;  /* ARREGLO */
            
            // Simula la salida de los clientes (uno por uno) mientras vacían la sucursal
            for (int i = limite; i > limite - CLIENTES_POR_FURGON; i--) {     /* MODIFICAMOS LA CONDICION */
                printf("Cliente salió de la sucursal. Clientes en la sucursal: %d\n", i); // Muestra el número de clientes restante
                usleep(500000); // Pausa medio segundo entre cada salida
            }

            printf("\nEl furgón blindado puede realizar la operación ahora.\n");
            sem_post(&operacion_en_curso); // Permite que el furgón haga su operación
        }

        pthread_mutex_unlock(&mutex); // Libera el bloqueo del mutex para que otros hilos puedan modificar "clientes"
        sem_post(&clientes_en_sucursal); // Incrementa el semáforo para indicar que un cliente más ha llegado a la sucursal
    }
}

// Función que simula la operación del furgón blindado
void* operacion_furgon(void* arg) {    
    char buffer[100];   /* BUFFER PARA LEER TUBERIA */
    
    while (*clientes <= MAX_CLIENTES) {    /* MODIFICAMOS EL CICLO */

        sem_wait(&clientes_en_sucursal); // Espera a que haya clientes en la sucursal (semáforo de clientes)
        
         /* LEER TUBERIA */
        read(tuberias[0], buffer, sizeof(buffer));
        printf("\t-> Furgon: Mensaje recibido -> %s", buffer); /* SE RECIBIO MENSAJE */
        
        // Si el número de clientes es múltiplo de CLIENTES_POR_FURGON y no supera el máximo
        if (*clientes % CLIENTES_POR_FURGON == 0 && *clientes <= MAX_CLIENTES) {
            sem_wait(&operacion_en_curso); // Bloquea cualquier otra operación en curso
            pthread_mutex_lock(&mutex);  /* BLOQUEAR ACCESO */

            printf("Furgón blindado realizando la operación de carga/descarga de dinero...\n");
            usleep(rand() % 2000000); // Pausa aleatoria para simular la duración de la operación (máximo 2 segundos)

            sem_post(&operacion_en_curso); // Libera el semáforo para permitir nuevas operaciones
            printf("Furgón blindado completó la operación.\n\n");
            pthread_mutex_unlock(&mutex);   /* DESBLOQUEA EL ACCESO */
        }

        /* CONDICION PARA SALIR DEL BUCLE */
        if (*clientes >= MAX_CLIENTES) {
            break;
        }
    }
}

int main() {
    pthread_t hilo_clientes, hilo_furgon; // Declaración de dos hilos: uno para los clientes y otro para el furgón
    
     /* TUBERIAS ENTRE PROCESOS */
    if (pipe(tuberias) == -1) {   /* EL USO DE PIPE CREA LA TUBERIA */ 
        perror("Error creando tubería");        /* ERROR SI NO SE CREA */
        exit(1);                                /* FINALIZA*/          
    }
    
    /* MEMORIA COMPARTIDA */
    int shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);   /* CREAMOS MEMORIA COMPARTIDA */
    clientes = (int*) shmat(shmid, NULL, 0);       /*ASIGNAMOS EL SEGMENTO A LA VARIABLE "clientes" */
    *clientes = 0;  /*INICIALIZA LOS CLIENTES*/
    
    
    // Inicializar los semáforos y el mutex
    sem_init(&clientes_en_sucursal, 0, 0); // Semáforo para contar los clientes
    sem_init(&operacion_en_curso, 0, 1);   // Semáforo para controlar el furgón (1 porque inicialmente puede operar)
    pthread_mutex_init(&mutex, NULL);      // Inicializar el mutex
    
    // Crear los hilos: uno para simular la llegada de clientes y otro para simular la operación del furgón
    pthread_create(&hilo_clientes, NULL, llegada_cliente, NULL);
    pthread_create(&hilo_furgon, NULL, operacion_furgon, NULL);
    
    /* FUNCION SWAP */
    printf("Antes del swap: MAX_CLIENTES = %d, CLIENTES_POR_FURGON = %d\n", MAX_CLIENTES, CLIENTES_POR_FURGON);
    swap(&MAX_CLIENTES, &CLIENTES_POR_FURGON);
    printf("Después del swap: MAX_CLIENTES = %d, CLIENTES_POR_FURGON = %d\n\n", MAX_CLIENTES, CLIENTES_POR_FURGON);

    /* PROCESO FORK */
    pid_t pid = fork();

    if (pid < 0) {
        perror("Error en fork");
        return 1;
    } else if (pid == 0) {
        // Proceso hijo
        printf("Clientes por furgón (Proceso hijo): %d\n", CLIENTES_POR_FURGON);
    } else {
        // Proceso padre
        printf("Max clientes (Proceso padre): %d\n", MAX_CLIENTES);
    }

    // Esperar a que ambos hilos terminen
    pthread_join(hilo_clientes, NULL);
    pthread_join(hilo_furgon, NULL);

    // Destruir los semaforos
    sem_destroy(&clientes_en_sucursal);
    sem_destroy(&operacion_en_curso);
    pthread_mutex_destroy(&mutex);
    
    /* FINALIZACION DE MEMORIA COMPARTIDA */
    shmdt(clientes); /* QUITAR LA MEMORIA COMPARTIDA */
    shmctl(shmid, IPC_RMID, NULL); /* ELIMINAR EL SEGMENTO*/
    

    return 0; 
}





