/* Equipo: Sin nombre    Fecha: 29-10-24   */
/* Este código calcula los tiempos de acceso en caso de acierto, fallo y el tiempo medio usando una TLB, 
además de la distribución de bits en una dirección virtual de 3 niveles para tablas de páginas. */

#include <stdio.h>
#include <math.h>

/* Funcion para calcular los tiempos de acceso */
void calcularTiemposAcceso(int tiempoTLB, int tiempoMemoria, float probAcierto, float probFallo, int *tiempoAcierto, int *tiempoFallo, float *tiempoMedio) {
    *tiempoAcierto = tiempoTLB + tiempoMemoria;                                      /* Calcular tiempo de acierto */
    *tiempoFallo = tiempoTLB + (4 * tiempoMemoria);                                  /* Calcular tiempo de fallo */
    *tiempoMedio = (probAcierto * (*tiempoAcierto)) + (probFallo * (*tiempoFallo));  /* Calcular tiempo medio de acceso */
}

/* Funcion para calcular los bits asignados a cada nivel y el desplazamiento */
void calcularBitsDireccion(int bitsDireccionVirtual, int capacidadKB, int *bitsDesplazamiento, int *bitsNivel1, int *bitsNivel2, int *bitsNivel3) {
    int tamanoPagina = capacidadKB * 1024;                              /* Convertir KB a bytes */
    *bitsDesplazamiento = (int)log2(tamanoPagina);                      /* Calcular bits para el desplazamiento */
    int bitsParaPaginas = bitsDireccionVirtual - *bitsDesplazamiento;   /* Bits restantes para tablas de paginas */

    int totalNiveles = 3;                                               /* Calcular bits para cada nivel */
    int bitsPorNivel = bitsParaPaginas / totalNiveles;                  /* Asignar bits equitativamente */

    *bitsNivel1 = bitsPorNivel;                                         /* Bits para el primer nivel */
    *bitsNivel2 = bitsPorNivel;                                         /* Bits para el segundo nivel */
    *bitsNivel3 = bitsParaPaginas - (*bitsNivel1 + *bitsNivel2);        /* Bits restantes para el tercer nivel */
}

int main() {
    int bitsDireccionVirtual = 36;                  /* Bits de la direccion virtual */
    int tiempoTLB = 8;                              /* Tiempo de acceso a la TLB en ns */
    int capacidadKB = 8;                            /* Capacidad de la pagina en KB */
    int bitsDesplazamiento, bitsNivel1, bitsNivel2, bitsNivel3; /* Variables para bits de cada nivel y desplazamiento */

    /* Calcular los bits para cada nivel y el desplazamiento */
    calcularBitsDireccion(bitsDireccionVirtual, capacidadKB, &bitsDesplazamiento, &bitsNivel1, &bitsNivel2, &bitsNivel3);

    printf("Descomposicion de la direccion virtual:\n");
    printf("Primer nivel de la tabla de paginas: %d bits\n", bitsNivel1);
    printf("Segundo nivel de la tabla de paginas: %d bits\n", bitsNivel2);
    printf("Tercer nivel de la tabla de paginas: %d bits\n", bitsNivel3);
    printf("Desplazamiento dentro de la pagina: %d bits\n\n", bitsDesplazamiento);

    int tiempoMemoria = 70;                         /* Tiempo de acceso a la memoria en ns */
    float probAciertoTLB = 0.9;                     /* 90% de aciertos en la TLB */
    float probFalloTLB = 0.1;                       /* 10% de fallos en la TLB */

    /* Variables para almacenar los tiempos de acierto, fallo y promedio */
    int tiempoAcierto, tiempoFallo;
    float tiempoMedio;

    /* Calcular los tiempos de acceso */
    calcularTiemposAcceso(tiempoTLB, tiempoMemoria, probAciertoTLB, probFalloTLB, &tiempoAcierto, &tiempoFallo, &tiempoMedio);

    printf("Tiempo de acceso en caso de acierto en la TLB: %d ns\n", tiempoAcierto);
    printf("Tiempo de acceso en caso de fallo en la TLB: %d ns\n", tiempoFallo);
    printf("El tiempo medio de acceso a memoria es: %.2f ns\n", tiempoMedio);

    return 0;
}




