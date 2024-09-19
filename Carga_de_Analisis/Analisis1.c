/* Equipo: Sin nombre    Fecha: 18/09/24   version 2*/
/*Se agregaron librerias se modifico la funcion pid a getpid y se quito una libreria que no se usa*/
/*Analisis: crea proceso padre e hijo, se le asignan sus PID de acuerdo a su estatus y los imprime */

//#include <fcntl.h> //Biblioteca para trabajar con operaciones de archivos

#include <stdio.h>
#include <stdlib.h> //Biblioteca para la función exit.
#include <unistd.h> //Biblioteca para funciones como fork y getpid
#include <sys/wait.h> //Biblioteca para funcion wait.

int main(){
	int pid, status;
	switch(pid=fork()){
		case -1: /*Gestion del error*/
			exit(1);
			break;
		case 0: /*Proceso hijo*/
			printf("\n Proceso hijo. PID=%d\n", getpid()); //Para obtener el PID del proceso hijo
			break;
		default: /*Proceso padre*/
			printf("\nProceso padre. PID=%d\n", getpid()); //Para obtener el PID del proceso padre
			wait(&status); /*espera al final del proceso hijo*/ 
			break;
		} /*fin del switch*/
		printf("\nFin de la ejecucion\n") ;
		return 0;
} /*fin de la funcion main*/
