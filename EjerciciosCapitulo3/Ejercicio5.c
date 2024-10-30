/* Equipo: Sin nombre    Fecha: 29-10-24   */
/* Este código simula la traducción de direcciones virtuales a direcciones físicas usando una tabla de páginas en un entorno con memoria paginada. */
  
#include <stdio.h>
#include <stdint.h>

#define TAMANO_PAGINA 4096     // Tamaño de página en bytes (4 KB)
#define ESPACIO_VIRTUAL 32     // Espacio de direcciones virtuales en bits
#define ESPACIO_FISICO 16      // Espacio de direcciones físicas en bits

// Definicion de la tabla de paginas
struct entrada_tabla_pagina {
    int bit_presencia;
    int bit_modificado;
    int marco_o_swap;
} tabla_paginas[8] = {
    {1, 0, 1},
    {0, 0, 8},
    {1, 1, 9},
    {1, 0, 14},
    {1, 0, 5},
    {1, 0, 7},
    {0, 0, 25},
    {0, 1, 16}
};

// Función para calcular la dirección física
int calcular_direccion_fisica(uint32_t direccion_virtual) {
    int numero_pagina = (direccion_virtual >> 12) & 0xFFF; // 20 bits para el número de página
    int desplazamiento = direccion_virtual & 0xFFF;        // 12 bits para el desplazamiento

    // Verificamos si la página está presente
    if (tabla_paginas[numero_pagina].bit_presencia == 1) {
        int numero_marco = tabla_paginas[numero_pagina].marco_o_swap;
        int direccion_fisica = (numero_marco << 12) | desplazamiento;
        return direccion_fisica;
    } else {
        printf("La página no está presente en la memoria fisica.\n");
        return -1;
    }
}

int main() {
    // Punto 1: Mostrar el formato de dirección virtual
    printf("Formato de dirección virtual:\n");
    printf("Número de página: 20 bits | Desplazamiento: 12 bits\n");

    // Punto 2: Calcular la dirección física correspondiente a la dirección virtual 0x00005b83
    uint32_t direccion_virtual = 0x00005b83;
    int direccion_fisica = calcular_direccion_fisica(direccion_virtual);

    if (direccion_fisica != -1) {
        printf("La dirección física correspondiente a la dirección virtual 0x%08X es 0x%08X\n", direccion_virtual, direccion_fisica);
    }

    // Punto 3: Tamaño del espacio de direcciones virtuales del programa
    uint64_t tamano_espacio_virtual = (uint64_t)1 << ESPACIO_VIRTUAL;
    printf("Tamaño del espacio de direcciones virtuales: %lu bytes\n", tamano_espacio_virtual);

    return 0;
}
