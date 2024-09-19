/* Equipo: Sin nombre    Fecha:18/09/24   Version 2 */
/* Le quitamos un for repetido, y algunos errores de sintaxis */
/* Analisis: intenta invocar argumentos de otro programa que se llama execprog2 y si no logra invocarlo se manda un mensaje de error */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main (int argc, char *argv[]){
    int i;
    printf("Ejecutando el programa invocador (execprog1). Sus argumentos son: \n");
    for (i = 0; i < argc; i++){  // Cambié 'i <= argc' a 'i < argc'
        printf("argv[%d]: %s\n", i, argv[i]);  // Cambié 'print' a 'printf'
        sleep(10);
    }
    strcpy(argv[0], "execprog2");  // Moví esta línea fuera del bucle
    if (execv("./execprog2", argv) < 0){  // Cambié '•' a '.'
        printf("Error en la invocacion a execprog2 \n");
        exit(1);
    }
    exit(0);
}
