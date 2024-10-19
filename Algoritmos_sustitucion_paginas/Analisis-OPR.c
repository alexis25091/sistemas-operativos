/* Equipo: Sin nombre    Fecha: 18-10-24   */
/* Analsis del codigo 3: Este código implementa el algoritmo de reemplazo óptimo de páginas (OPR), que carga páginas en memoria física 
y reemplaza la página menos utilizada en el futuro cuando la memoria está llena. Usa una lista de frames para gestionar las páginas 
y simula el acceso futuro para determinar qué página reemplazar. */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NUM_FRAMES 4   // Numero de frames (paginas fisicas en memoria)
#define NUM_PAGES 10   // Numero total de paginas virtuales

// Estructura para un frame de pagina en memoria fisica
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
	if (frame != NULL) {                   /* Verifica si la memoria esta vacia */    
		frame->page = -1;                  // Inicialmente no hay pagina asignada
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
	if (frameList->head == NULL) {              /* Si la lista esta vacia, el frame es head y tail */
		frameList->head = frame;
		frameList->tail = frame;
	} else {                                    /* Si no, se inserta al frente de la lista */
		frame->next = frameList->head;
		frameList->head->prev = frame;
		frameList->head = frame;
	}
	frameList->numFrames++;
}

// Funcion para eliminar un frame de la lista (menos recientemente usado)
void removeFrame(FrameList *frameList, Frame *frame) {
	if (frame->prev != NULL) {                  /* Si tiene un anterior, actualiza al siguiente */
		frame->prev->next = frame->next;
	} else {                                    /* Si no tiene anterior, actualiza al head */
		frameList->head = frame->next;
	}
	
	if (frame->next != NULL) {                  /* Si tiene siguiente, actualiza al anterior*/
		frame->next->prev = frame->prev;
	} else {                                    /* Si no tiene anterior, actualiza al tail */
		frameList->tail = frame->prev;
	}
	frameList->numFrames--;
	free(frame);
}

// Funcion para buscar un frame especifico por numero de pagina
Frame* findFrame(FrameList *frameList, int page) {
	Frame *current = frameList->head;
	while (current != NULL) {
		if (current->page == page) {            /* Encontrar un frame cuyo número de página sea igual a page */
			return current;
		}
		current = current->next;
	}
	return NULL;
}

// Funcion para simular la carga de una pagina a memoria fisica utilizando OPR
void loadPage(FrameList *frameList, int page, int futureAccess[]) {
	Frame *existingFrame = findFrame(frameList, page);
    if (existingFrame != NULL) {
        printf("=> Pagina %d ya esta en memoria\n", page);
        return;
    }
    
	Frame *frame = createFrame();                   /* Crea nuevo frame */
	frame->page = page;                             /* Asigna el número de la página al frame creado */
	frame->valid = true;                            /* Marca el frame como ocupado */

	// Si la lista de frames ya esta llena, determinar la pagina optima a reemplazar
	if (frameList->numFrames == NUM_FRAMES) {       /* Verifica si toda la lista esta llena */
		Frame *optimalFrame = NULL;                 /* Variable que almacenará el frame que se debe reemplazar */
		int farthest = -1;                          /* Almacenara la mayor distancia */
        
        // Buscar el frame que contiene la pagina que no sera utilizada por mas tiempo
        Frame *current = frameList->head;           /* Comienza desde el primero (head) */
		while (current != NULL) {
            int j = 0;
            while (j < NUM_PAGES && futureAccess[j] != current->page) {
                j++;
            }
            if (j == NUM_PAGES) {                   /* Si la página no sera usada en el futuro */
                optimalFrame = current;
                break;
            }
            if (j > farthest) {                     /* Si se encuentra una pagina mas lejana en el futuro*/
                farthest = j;
                optimalFrame = current;
            }

            current = current->next;
        }

		// Remover el frame optimo encontrado
		if (optimalFrame != NULL) {                 /* Si se encontro frame que reemplazar */
            printf("=> Reemplazando pagina %d por pagina %d\n", optimalFrame->page, page);
            optimalFrame->page = page;          // Actualiza el contenido del frame en lugar de eliminarlo
		}
	} else {
        // Insertar el nuevo frame en la lista de frames
        insertFrame(frameList, frame);
    }
}

// Función para imprimir el estado actual de la lista de frames y el acceso futuro
void printFrameList(FrameList *frameList, int futureAccess[], int start, int num) {
    printf("Estado actual de la lista de frames:\n");
    Frame *current = frameList->head;
    while (current != NULL) {
        printf("\tPágina: %d\n", current->page);
        current = current->next;
    }
    
    printf("  Acceso futuro: ");
    for (int i = start; i < num; i++) {
        printf("%d ", futureAccess[i]);
    }
    printf("\n\n");
}

int main() {
	FrameList *frameList = createFrameList();

	// Simular el orden de accesos futuro a las paginas (simplificado)
	int futureAccess[NUM_PAGES] =  {7, 2, 5, 3, 4, 2, 3, 1, 5, 7};

	// Simular la carga de paginas a memoria fisica utilizando el algoritmo OPR
	for (int i = 0; i < NUM_PAGES; ++i) {
		loadPage(frameList, futureAccess[i], futureAccess + i + 1);
		printFrameList(frameList, futureAccess, i + 1, NUM_PAGES);
	}

	// Liberar la memoria utilizada por la lista de frames
	free(frameList);

	return 0;
}





