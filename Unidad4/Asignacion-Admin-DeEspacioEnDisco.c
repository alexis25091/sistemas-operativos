/* Equipo: Sin nombre       Fecha: 06-11-24   */
/* El programa simula un sistema de administración de espacio en un disco con bloques. Permite al usuario:
    Asignar bloques: Se buscan bloques consecutivos libres y se asignan.
    Liberar bloques: Se liberan bloques ocupados.
    Ver el estado del disco: Muestra que bloques estan ocupados (1) y cuales estan libres(0).
    Inicialmente el disco tiene 10 bloques, los cuales todos estan libres. */

#include <stdio.h>
#include <stdlib.h>

#define TAM 10                          /* Tamaño del disco en bloques */

int disco[TAM] = {0};                   /* Espacio de disco */

/* Función que muestra el estado del disco */
void estado_disco() {           
    printf("Estado del disco: ");
    for (int i = 0; i < TAM; i++) {
        printf("%d ", disco[i]);
    }
    printf("\n");
}

/* Función que asigna bloques de disco a un archivo */
int asignar_espacio(int cantidad) {
    int contador = 0;                   /* Contador de bloques libres consecutivos */

    /* Buscar bloques consecutivos libres */
    for (int i = 0; i < TAM; i++) {
        if (disco[i] == 0) {            /* Si el bloque está libre */
            contador++;
            if (contador == cantidad) {
                /* Asignar los bloques encontrados */
                for (int j = i; j >= i - cantidad + 1; j--) {
                    disco[j] = 1;       /* Marcar con 1 los bloques como ocupados */
                }
                printf("Se han asignado %d bloques de espacio.\n", cantidad);
                return 1;               /* Espacio asignado */
            }
        } else {
            contador = 0;               /* Si se encuentra un bloque ocupado, reiniciar el contador */
        }
    }

    printf("No hay suficiente espacio para %d bloques.\n", cantidad);
    return 0;                           /* No hay espacio para asignar */
}

/* Función para liberar bloques (sin necesidad de contigüidad) */
void liberar_espacio(int cantidad) {
    int bloques_libres = 0;

    /* Recorrer el disco buscando bloques ocupados */
    for (int i = 0; i < TAM; i++) {
        if (disco[i] == 1) {            /* Si el bloque está ocupado */
            disco[i] = 0;               /* Liberar el bloque */
            bloques_libres++;           /* Contamos el bloque liberado */
            if (bloques_libres == cantidad) {
                printf("Se han liberado %d bloques de espacio.\n", cantidad);
                return;
            }
        }
    }

    printf("No se encontraron suficientes bloques ocupados para liberar.\n");
}

int main() {
    int opcion, cantidad;

    while (1) {
        /* Mostrar menú */
        printf("\nOPCIONES:\n");
        printf("\t1. Asignar espacio\n");
        printf("\t2. Liberar espacio\n");
        printf("\t3. Ver estado del disco\n");
        printf("\t4. Salir\n");
        printf("Selecciona una opción: -> ");
        scanf("%d", &opcion);
        printf("\n");

        switch (opcion) {
            case 1:
                printf("¿Cuántos bloques quieres asignar? ");
                scanf("%d", &cantidad);
                asignar_espacio(cantidad);
                break;
            case 2:
                printf("¿Cuántos bloques quieres liberar? ");
                scanf("%d", &cantidad);
                liberar_espacio(cantidad);
                break;
            case 3:
                printf("Disco actual -> ");
                estado_disco();
                break;
            case 4:
                printf("Saliendo del programa...   c: \n");
                return 0;
            default:
                printf("Opción no válida. Intenta de nuevo.\n");
        }
    }
    return 0;
}
