/* Equipo: Sin nombre    Fecha: 18-10-24   */
/* Conversion del algoritmo FIFO al LRU */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NUM_FRAMES 4   // Número de frames (páginas físicas en memoria)
#define NUM_PAGES 10   // Número total de páginas virtuales

// Estructura para un frame de página en memoria física
typedef struct Frame {
    int page;           // Número de página almacenada en el frame (valor -1 si está vacío)
    bool valid;         // Indica si el frame está ocupado (true) o vacío (false)
    struct Frame *prev; // Puntero al frame previo (para lista doblemente enlazada)
    struct Frame *next; // Puntero al frame siguiente (para lista doblemente enlazada)
} Frame;

// Estructura para la lista de frames en memoria física
typedef struct FrameList {
    int numFrames;      // Número de frames actualmente ocupados
    Frame *head;        // Puntero al primer frame de la lista
    Frame *tail;        // Puntero al último frame de la lista
} FrameList;

// Función para crear un nuevo frame
Frame* createFrame() {
    Frame *frame = (Frame *)malloc(sizeof(Frame));
    if (frame != NULL) {
        frame->page = -1;   // Inicialmente no hay página asignada
        frame->valid = false;
        frame->prev = NULL;
        frame->next = NULL;
    }
    return frame;
}

// Función para inicializar la lista de frames en memoria física
FrameList* createFrameList() {
    FrameList *frameList = (FrameList *)malloc(sizeof(FrameList));
    if (frameList != NULL) {
        frameList->numFrames = 0;
        frameList->head = NULL;
        frameList->tail = NULL;
    }
    return frameList;
}

// Función para mover un frame al frente de la lista (más recientemente usado)
void moveFrameToFront(FrameList *frameList, Frame *frame) {
    if (frame == frameList->head) {
        // Ya está en el frente, no hacer nada
        return;
    }

    // Desvincular el frame de su posición actual
    if (frame->prev != NULL) {
        frame->prev->next = frame->next;
    } else {
        frameList->head = frame->next; // Si es el primero, mover la cabeza
    }

    if (frame->next != NULL) {
        frame->next->prev = frame->prev;
    } else {
        frameList->tail = frame->prev; // Si es el último, mover la cola
    }

    // Mover el frame al frente de la lista
    frame->prev = NULL;
    frame->next = frameList->head;
    if (frameList->head != NULL) {
        frameList->head->prev = frame;
    }
    frameList->head = frame;

    // Si era el único frame, actualizar también la cola
    if (frameList->tail == NULL) {
        frameList->tail = frame;
    }
}



// Función para insertar un frame al frente de la lista (más recientemente usado)
void insertFrame(FrameList *frameList, Frame *frame) {
    frame->next = frameList->head;
    if (frameList->head != NULL) {
        frameList->head->prev = frame;
    }
    frameList->head = frame;

    if (frameList->tail == NULL) {
        frameList->tail = frame;
    }

    frameList->numFrames++;
}

// Función para eliminar un frame de la lista
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

// Función para eliminar el frame menos recientemente usado (LRU)
void removeLRUFrame(FrameList *frameList) {
    if (frameList->tail != NULL) {
        Frame *lruFrame = frameList->tail;
        removeFrame(frameList, lruFrame);
    }
}

// Función para buscar un frame específico por número de página
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

// Función para simular la carga de una página a memoria física utilizando LRU
void loadPage(FrameList *frameList, int page) {
    Frame *existingFrame = findFrame(frameList, page);
    
    if (existingFrame != NULL) {
        // Si la página ya está en un frame, moverla al frente (más recientemente usada)
        moveFrameToFront(frameList, existingFrame);
        return;
    }

    // Si la lista de frames ya está llena, eliminar el frame menos recientemente usado (LRU)
    if (frameList->numFrames == NUM_FRAMES) {
        removeLRUFrame(frameList);
    }

    // Insertar una nueva página como el frame más recientemente usado
    Frame *frame = createFrame();
    frame->page = page;
    frame->valid = true;
    insertFrame(frameList, frame);
}

// Función para imprimir el estado actual de la lista de frames
void printFrameList(FrameList *frameList) {
    printf("Estado actual de la lista de frames:\n");
    Frame *current = frameList->head;
    while (current != NULL) {
        printf("Página: %d, ", current->page);
        if (current->valid) {
            printf("Estado: Ocupado\n");
        } else {
            printf("Estado: Vacío\n");
        }
        current = current->next;
    }
    printf("\n");
}

int main() {
    FrameList *frameList = createFrameList();

    // Simular la carga de varias páginas a memoria física
    loadPage(frameList, 1);
    loadPage(frameList, 2);
    loadPage(frameList, 3);
    loadPage(frameList, 4);
    printFrameList(frameList);  // Debería imprimir el estado actual de los frames

    // Intentar cargar otra página cuando todos los frames están ocupados
    loadPage(frameList, 5);
    printFrameList(frameList);  // Debería imprimir el estado actual después de la sustitución

    // Cargar una página ya existente para moverla al frente (LRU)
    loadPage(frameList, 3);
    printFrameList(frameList);

    // Liberar la memoria utilizada por la lista de frames
    free(frameList);

    return 0;
}
