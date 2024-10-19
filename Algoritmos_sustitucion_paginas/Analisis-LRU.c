/* Equipo: Sin nombre    Fecha: 18-10-24   */
/* Analsis del codigo 2: Este código implementa la gestión de páginas en memoria física utilizando el algoritmo 
LRU (Least Recently Used) para el reemplazo de páginas. */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NUM_FRAMES 4   // Numero de frames (paginas fisicas en memoria)
#define NUM_PAGES 10   // Numero total de pC!ginas virtuales

// Estructura para un frame de pagina en memoria fC-sica
typedef struct Frame {
	int page;           // Numero de pagina almacenada en el frame (valor -1 si esta vacio)
	bool valid;         // Indica si el frame esta ocupado (true) o vacio (false)
	struct Frame *prev; // Puntero al frame previo (para lista doblemente enlazada)
	struct Frame *next; // Puntero al frame siguiente (para lista doblemente enlazada)
} Frame;

// Estructura para la lista de frames en memoria fisica
typedef struct FrameList {
	int numFrames;      // Numero de frames actualmente ocupados
	Frame *head;        // Puntero al primer frame de la lista
	Frame *tail;        // Puntero al ultimo frame de la lista
} FrameList;

// Funcion para crear un nuevo frame
Frame* createFrame() {
	Frame *frame = (Frame *)malloc(sizeof(Frame));
	if (frame != NULL) {
		frame->page = -1;   // Inicialmente no hay pagina asignada
		frame->valid = false;
		frame->prev = NULL;
		frame->next = NULL;
	}
	return frame;
}

// Funcion para inicializar la lista de frames en memoria fisica
FrameList* createFrameList() {
	FrameList *frameList = (FrameList *)malloc(sizeof(FrameList));
	if (frameList != NULL) {
		frameList->numFrames = 0;
		frameList->head = NULL;
		frameList->tail = NULL;
	}
	return frameList;
}

// Funcion para insertar un frame al frente de la lista (mas recientemente usado)
void insertFrame(FrameList *frameList, Frame *frame) {
	if (frameList->head == NULL) {
		// Lista vacia
		frameList->head = frame;
		frameList->tail = frame;
	} else {
		// Insertar al frente de la lista
		frame->next = frameList->head;
		frameList->head->prev = frame;
		frameList->head = frame;
	}
	frameList->numFrames++;
}

// Funcion para eliminar un frame de la lista (menos recientemente usado)
void removeFrame(FrameList *frameList, Frame *frame) {
	if (frame->prev != NULL) {
		frame->prev->next = frame->next;
	} else {
		frameList->head = frame->next;
	}
	if (frame->next != NULL) {
		frame->next->prev = frame->prev;
	} else {
		frameList->tail = frame->prev;
	}
	frameList->numFrames--;
	free(frame);
}

// Funcion para buscar un frame especifico por numero de pagina
Frame* findFrame(FrameList *frameList, int page) {
	Frame *current = frameList->head;
	while (current != NULL) {
		if (current->page == page) {
			return current;
		}
		current = current->next;
	}
	return NULL;
}

// Funcion para simular la carga de una pagina a memoria fisica
void loadPage(FrameList *frameList, int page) {
	Frame *frame = createFrame();
	frame->page = page;
	frame->valid = true;

	// Si la lista de frames ya esta llena, reemplazar la pagina menos recientemente usada (LRU)
	if (frameList->numFrames == NUM_FRAMES) {
		Frame *lruFrame = frameList->tail;
		removeFrame(frameList, lruFrame);
	}

	insertFrame(frameList, frame);
}

// Funcion para imprimir el estado actual de la lista de frames (solo para fines de depuracion)
void printFrameList(FrameList *frameList) {
	printf("Estado actual de la lista de frames:\n");
	Frame *current = frameList->head;
	while (current != NULL) {
		printf("PC!gina: %d, ", current->page);
		if (current->valid) {
			printf("Estado: Ocupado\n");
		} else {
			printf("Estado: VacC-o\n");
		}
		current = current->next;
	}
	printf("\n");
}

int main() {
	FrameList *frameList = createFrameList();

	// Simular la carga de varias paginas a memoria fisica
	loadPage(frameList, 1);
	loadPage(frameList, 2);
	loadPage(frameList, 3);
	loadPage(frameList, 4);
	printFrameList(frameList);  // Deberia imprimir el estado actual de los frames

	// Intentar cargar otra pagina cuando todos los frames estan ocupados
	loadPage(frameList, 5);
	printFrameList(frameList);  // Deberia imprimir el estado actual despues de la sustitucion

	// Liberar la memoria utilizada por la lista de frames
	free(frameList);

	return 0;
}






