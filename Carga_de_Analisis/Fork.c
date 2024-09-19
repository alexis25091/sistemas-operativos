/* Equipo: Sin nombre    Fecha: 18/09/24   Version 2 */
/* No se le modifico nada al codigo */
/* Analisis: se crean ambos hilos padre e hijo una vez creados el del padre se queda 20 segundos en espera y el hijo 30 segundos,
una vez terminados esos dos tiempos se imprimen dos mensajes correspondientes de que cada hilo finalizo */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
int main(){
    pid_t rf;
    rf = fork();
    switch (rf) {
        case -1:
            printf ("No he podido crear el proceso hijo \n");
        break;
        case 0:
            printf("Soy el hijo, mi PID es %d y mi PPID es %d\n", getpid(), getppid());
            sleep(20);  /*suspende el proceso 20 segundos*/
        break;
        default:
            printf("Soy el padre, mi PID es %d y el PID de mi hijo es %d\n", getpid(), rf);
            sleep(30);  /*suspende el proceso 30 segundos. acaba antes el hijo*/
        }
        printf("Final de ejecucion de %d \n", getpid());
    exit(0);
}
