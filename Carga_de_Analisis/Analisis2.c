/* Equipo: Sin nombre    Fecha: 18/09/24   Version 2 */
/* Se modifico error de sintaxis en una de las declaraciones */
/* Analisis: muestra la comunicacion entre el proceso padre e hijo, el padre solicita una cadena de hasta 10 caracteres,
se lo envia al hijo el cual lo imprime de vuelta, termina el hijo y luego el padre */

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

void imprimeDePipe(int leePipe);  //Funcionalidad para el hijo
void enviaAPipe(int escribePipe); //Funcionalidad para el padre

int main() {
    pid_t procHijo;
    int pipeFileDescriptors[2]; //Descriptores de ambos extremos
    
    if (pipe(pipeFileDescriptors) == -1) { //Genera pipe
        printf("Error al crear pipe\n");
        exit(1);
    }

    procHijo = fork(); //Genera proceso hijo

    if (procHijo < 0) {
        int errnum = errno; //Preservamos código de error      //Error de espacio en interrnum
        printf("Error %d al generar proceso hijo con: fork\n", errnum);
        exit(1);
    }

    if (procHijo == 0) {
        //Es el hijo, cierra pipe de envío y procede
        close(pipeFileDescriptors[1]);
        imprimeDePipe(pipeFileDescriptors[0]);
    }

    if (procHijo > 0) {
        //Es el padre, cierra pipe de recepción y procede
        close(pipeFileDescriptors[0]);
        enviaAPipe(pipeFileDescriptors[1]);
    }

    return 0;
}

/* Funcionalidad para el hijo: lee del pipe la cadena enviada, imprime el contenido
a STDOUT, cierra su extremo del pipe (lectura) y termina su proceso. */
void imprimeDePipe(int leePipe) {
    char buf; // Carácter de buffer
    printf("Proceso hijo, esperando cadena... \n");
    while (read(leePipe, &buf, 1) > 0) {
        write(STDOUT_FILENO, &buf, 1);
    }
    write(STDOUT_FILENO, "\n", 1); // Fin de línea
    close(leePipe); // Cerrar el pipe después de terminar de leer
    printf("Proceso hijo, finalizando\n");
    exit(0);
}

/* Funcionalidad para el padre: lee una cadena de hasta 10 caracteres,
   la escribe a su extremo del pipe, cierra su extremo y espera a que
   el proceso hijo termine antes de finalizar. */
void enviaAPipe(int escribePipe) {
    char buf[10]; // Buffer de hasta 10 caracteres
    printf("Proceso padre, ingresa una cadena de hasta 10 caracteres y enter: \n");
    scanf("%10s", buf);
    printf("\n\n"); // Separa la entrada de las futuras salidas.
    write(escribePipe, buf, strlen(buf)); // Enviar los datos al pipe
    close(escribePipe); // Presenta EOF al proceso hijo
    wait(NULL); // Espera terminación del hijo
    printf("Hijo terminado, terminando proceso padre\n");
    exit(0);
}
