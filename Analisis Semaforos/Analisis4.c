/* Equipo: Sin nombre    Fecha: 27-09-24   */
/* Analsis del codigo 4 */

/* Apartado de bibliotecas */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>      /* Para funciones de entrada/salida */
#include <stdlib.h>     /* Para funciones relacionadas con la asignación de memoria */
#include <unistd.h>     /* Para usar funcion sleep */

#define SHMKEY 75       /* Define clave de la memoria compartida */
#define TRUE 1          /* Define constante 1 */

/* Prototipos de las funciones */
int region_critica(int pid);
int region_no_critica(int pid); 
int esperando_turno(int pid);

/* Funcion proporcional */
int main() {
    int shmid, pid;             /* Se definen: shmid es el identificador de la memoria compartida, pid el id del proceso */
    char *addr, *turno;         /* Declaran dos punteros */
    pid = getpid();             /*  Se obtiene el identificador del proceso que se está ejecutando */
    shmid = shmget(SHMKEY, 10, 0777 | IPC_CREAT);       /* Utilizando shmkey crea o accede a un segmento de 10 bytes, 0777 otorga a todos los usarios accesos de lectura, escritura y ejecución */
                                                        /* IPC_CREAT:Si el segmento de memoria compartida no existe, el s.o debe crearlo */
    addr = shmat(shmid, 0, 0);          /* Conecta el segmento de memoria compartida identificado por shmid al proceso que lo necesita */
                                        /* El primer 0: permite al sistema elegir una dirección adecuada para adjuntar el segmento, el segundo no activa nada */
    turno = (char *) addr;              /* Asigna el puntero de memoria compartida addr a la variable turno */
    
    printf("\n [l] Variables Declaradas");
    sleep (5);              /* Se espera 5 segundos */
    (*turno) = '0';         /* Inicializa el turno, estableciendo que el proceso que tiene el turno es el proceso "0". */
    
    printf("\n [l] Iniciando proceso de turnos con turno inicial: %c", (*turno));

    while (TRUE) {                      /* ciclo que se ejecuta infinitamente */
        while((*turno) != '0') {        /* Verifica si el turno actual no es del proceso "0"*/
            esperando_turno(pid);           
        }
        region_critica(pid);            /* Llama a la funcion region_critica */
        sleep(1);                       /* Se espera 1 segundo */
        (*turno) = '1';                 /* Cuando el proceso "0" termina su sección crítica, cambia el turno al proceso "1" */
        region_no_critica(pid);         /* Llama a la funcion region_no_critica */
    }
    
    shmdt(addr);                        /* Desasocia el segmento de memoria compartida del espacio de direcciones del proceso */
    
    shmctl(shmid, IPC_RMID, 0);         /* Al segmento de memoria compartida le marca cuando no se esta utilizado por ningún proceso.*/
    return 0;
}
    
/* Función que se ejecuta cuando el proceso está esperando su turno para acceder a la sección crítica */
int esperando_turno(int pid) {
    printf("\n[l]Proceso %d y esperando turno :p", pid);
    sleep(2);
    return 0;
}

/* Funcion que simula la sección crítica. Se imprimen mensajes que indican que el proceso está ejecutando tareas */
int region_critica(int pid) {
    printf("\n\n [O] Cambiando Variables");
    for (int i = 0; i < 5; i++) {
        printf("\n [O] Turno %d", i + 1);
        sleep(1);
    }
    return 0;
}

/* Función que simula la sección no crítica, aqui el proceso no está haciendo operaciones que afecten a la memoria compartida */
int region_no_critica(int pid){
    printf("\n [0] Proceso %d yturno tomado:)\n",pid);
    return 0;
}
