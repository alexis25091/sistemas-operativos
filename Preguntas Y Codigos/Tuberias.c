/* Equipo: Sin nombre    Fecha: 14-10-24   Version: 1*/
/* Codigo 2: Este código lee un archivo, separa sus líneas en pares e impares, y luego filtra esas líneas usando dos palabras clave proporcionadas por el usuario. 
Las líneas filtradas se ordenan y se muestran en pantalla. Utiliza tuberías y procesos para gestionar la comunicación entre diferentes tareas y comandos externos como grep y sort. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define LONGITUD_MAXIMA_LINEA 1000

void ejecutar_comando(char *comando[], int entrada, int salida) {
    if (entrada != -1) {
        dup2(entrada, STDIN_FILENO);  /* Redirigir stdin */
    }
    if (salida != -1) {
        dup2(salida, STDOUT_FILENO);  /* Redirigir stdout */
    }
    execvp(comando[0], comando);
    perror("Error al ejecutar el comando");
    exit(1);
}

void tarea1(const char *archivo, int pipe_par[], int pipe_impar[]) {
    FILE *f = fopen(archivo, "r");
    if (!f) {
        perror("Error al abrir el archivo");
        exit(1);
    }

    char linea[LONGITUD_MAXIMA_LINEA];  
    int contador = 0;

    while (fgets(linea, sizeof(linea), f)) {
        if (contador % 2 == 0) {
            write(pipe_par[1], linea, strlen(linea));  /* Linea impar */
        } else {
            write(pipe_impar[1], linea, strlen(linea));  /* Linea par */
        }
        contador++;
    }

    fclose(f);
    close(pipe_par[1]);  /* Cerrar el descriptor de escritura de la tuberia par */
    close(pipe_impar[1]);  /* Cerrar el descriptor de escritura de la tuberia impar */
}

void tarea2(int pipe_par[], const char *palabra) {
    pid_t pid = fork();
    if (pid == 0) {
        /* Hijo: ejecutar grep en la tubería 1 (líneas pares) */
        char *comando[] = {"grep", (char *)palabra, NULL};
        close(pipe_par[1]);  /* Cerrar la escritura de la tuberia */
        ejecutar_comando(comando, pipe_par[0], STDOUT_FILENO);  /* Leer desde tuberia, escribir a stdout */
    } else {
        wait(NULL);  /* Esperar que termine el proceso hijo */
    }
}

void tarea3(int pipe_impar[], const char *palabra) {
    pid_t pid = fork();
    if (pid == 0) {
        /* Hijo: ejecutar grep en la tubería 2 (líneas impares) */
        char *comando[] = {"grep", (char *)palabra, NULL};
        close(pipe_impar[1]);  /* Cerrar la escritura de la tuberia */
        ejecutar_comando(comando, pipe_impar[0], STDOUT_FILENO);  /* Leer desde tuberia, escribir a stdout */
    } else {
        wait(NULL);  /* Esperar que termine el proceso hijo */
    }
}

void tarea4(int pipe_tres[]) {
    pid_t pid = fork();
    if (pid == 0) {
        /* Hijo: ejecutar sort en la tubería 3 */
        char *comando[] = {"sort", NULL};
        close(pipe_tres[1]);  /* Cerrar la escritura de la tuberia */
        ejecutar_comando(comando, pipe_tres[0], STDOUT_FILENO);  /* Leer desde tuberia, escribir a stdout */
    } else {
        wait(NULL);  /* Esperar que termine el proceso hijo */
    }
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Uso: %s <archivo> <palabra_1> <palabra_2> <output>\n", argv[0]);
        exit(1);
    }

    int pipe_par[2], pipe_impar[2], pipe_tres[2];

    /* Crear las tuberias */
    if (pipe(pipe_par) == -1 || pipe(pipe_impar) == -1 || pipe(pipe_tres) == -1) {
        perror("Error al crear las tuberias");
        exit(1);
    }

    /* Tarea 1: Leer el archivo y distribuir entre las tuberias */
    tarea1(argv[1], pipe_par, pipe_impar);

    /* Tarea 2: Filtrar las líneas pares usando grep */
    tarea2(pipe_par, argv[2]);

    /* Tarea 3: Filtrar las líneas impares usando grep */
    tarea3(pipe_impar, argv[3]);

    /* Tarea 4: Ordenar las líneas filtradas por grep en la tuberia 3 */
    tarea4(pipe_tres);

    return 0;
}
