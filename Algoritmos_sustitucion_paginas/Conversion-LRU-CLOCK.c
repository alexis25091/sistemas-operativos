/* Equipo: Sin nombre    Fecha: 18-10-24   */
/* Conversion del algoritmo LRU al CLOCK */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NUM_FRAMES 4  // Número de frames (paginas físicas en memoria)
#define NUM_PAGES 10   // Número total de paginas virtuales


        // Estructura para un frame de pagina en memoria física
        typedef struct Frame {
            int page;                       // Número de pagina almacenada en el frame (valor -1 si esta vacío)
            bool valid;                     // Indica si el frame esta ocupado (true) o vacío (false)
            struct Frame *prev;             // Puntero al frame previo (para lista doblemente enlazada)
            struct Frame *next;             // Puntero al frame siguiente (para lista doblemente enlazada)
        } Frame;

        // Estructura para la lista de frames en memoria física
        typedef struct FrameList {
            int numFrames;                  // Número de frames actualmente ocupados
            Frame *head;                    // Puntero al primer frame de la lista
            Frame *current;                 // Puntero al frame actualmente en uso (para el ciclo CLOCK)
        } FrameList;

        // Función para crear un nuevo frame
        Frame* createFrame() {
            Frame *frame = (Frame *)malloc(sizeof(Frame));
            if (frame != NULL) {
                frame->page = -1;           // Inicialmente no hay pagina asignada
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
                frameList->current = NULL;  // Inicializa el puntero current
            }
        return frameList;
        }

        // Función para insertar un frame al frente de la lista (mas recientemente usado)
        void insertFrame(FrameList *frameList, Frame *frame) {
            if (frameList->head == NULL) {
                // Lista vacía
                frameList->head = frame;
            } else {
                // Insertar al frente de la lista
                frame->next = frameList->head;
                frameList->head->prev = frame;
                frameList->head = frame;
            }
            frameList->numFrames++;
        }

        // Función para eliminar un frame de la lista (menos recientemente usado)
        void removeFrame(FrameList *frameList, Frame *frame) {
            if (frame->prev != NULL) {
                frame->prev->next = frame->next;
            } else {
                frameList->head = frame->next;  // Actualiza head si se elimina el primer frame
            }
            if (frame->next != NULL) {
                frame->next->prev = frame->prev;
            }
            /*} else {
                frameList->tail = frame->prev;
            }
            */
            // Aqui de acuerdo al codigo LRU en el CLOCK no se necesita el tail ya que no va en orden sino se guia por el bit que tenga la pagina

            frameList->numFrames--;
            free(frame);
        }

        // Función para buscar un frame específico por número de pagina
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

        // Función para simular la carga de una pagina a memoria física usando el algoritmo CLOCK
        void loadPage(FrameList *frameList, int page) {

            // Busca el frame correspondiente a la pagina
            Frame *RestAlmcenado = findFrame(frameList, page);                //Lo que hace es buscar si la pagina esta ya en la memoria y se almacena en RestAlmcenado

            // Si la pagina ya esta en memoria, simplemente actualiza su estado
                if (RestAlmcenado != NULL) {                                //Verifica si se encontró un frame que ya contiene la página (es decir, si no es NULL)
                        
                    RestAlmcenado->valid = true;  // Actualiza el bit de referencia dando a entender que ya esta en uso
                    return;
                }

            
            if (frameList->numFrames == NUM_FRAMES) {                // Inicia el ciclo en el frame actual para verificar si no ha alcanzado el limite o sea no hay espacios para cargar la pagina
                    
                Frame *current = frameList->current ? frameList->current : frameList->head;   /*Si current o sea el puntero no es NULL, se usa; de lo contrario, se comienza desde el head (primer frame). 
                                                                                                Esto permite empezar desde donde se dejó en el último ciclo. */

                while (true) {
                // Si el bit de referencia es 0, se puede reemplazar
                    if (!current->valid) {
                        removeFrame(frameList, current);
                        frameList->current = current->next ? current->next : frameList->head;  // Actualiza current
                        break; // Salir del ciclo después de eliminar
                    } else {
                        // Si el bit es 1, se lo establece a 0 y se pasa al siguiente
                            current->valid = false;
                    }

                    // Mueve al siguiente frame en el ciclo
                    current = current->next ? current->next : frameList->head;
            }
        }

        // Crea e inserte el nuevo frame
        Frame *frame = createFrame();
            frame->page = page;
            frame->valid = true; // Establecer bit de referencia a 1
            insertFrame(frameList, frame);
            frameList->current = frame;  // Actualiza el puntero current al nuevo frame
        }

        // Función para imprimir el estado actual de la lista de frames (solo para fines de depuración)
        void printFrameList(FrameList *frameList) {
            printf("Estado actual de la lista de frames (%d frames ocupados):\n", frameList->numFrames);
            Frame *current = frameList->head;

            while (current != NULL) {
                printf("Pagina: %d, Estado: %s, Bit de referencia: %s\n",
                current->page,
                current->valid ? "Ocupado" : "Vacío",       //Dice vacio si tiene el 0 y ocupado si es 1
                current->valid ? "1" : "0"); // Muestra el bit de referencia ya sea 1 o 0
            current = current->next;
            }
            printf("\n");
        }

        int main() {
            FrameList *frameList = createFrameList();

            // Simular la carga de varias paginas a memoria física
            loadPage(frameList, 1);
            loadPage(frameList, 2);
            loadPage(frameList, 3);
            loadPage(frameList, 4);
            printFrameList(frameList);  // Debería imprimir el estado actual de los frames

            // Intentar cargar otra pagina cuando todos los frames estan ocupados
            loadPage(frameList, 5);
            printFrameList(frameList);  // Debería imprimir el estado actual después de la sustitución

            loadPage(frameList, 6);     //Esta es una segunda pagina que se agrega para verse mas detallado del cambio anterior
            printFrameList(frameList);  //Hace otra impreción para mostrar el estado actual después de la sustitución
        


            // Liberar la memoria utilizada por la lista de frames
            Frame *current = frameList->head;
            while (current != NULL) {
                    Frame *temp = current;
                    current = current->next;
                    free(temp);
                    }
            free(frameList);

            return 0;
}
