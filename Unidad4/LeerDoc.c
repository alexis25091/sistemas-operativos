/* Equipo: Sin nombre    Fecha: 15-11-24   */
/* El codigo abre un archivo .docx, extrae y muestra en la consola el texto contenido en el archivo. */

/* INDICACIONES PARA EJECUTAR EL CODIGO (COMPILAR EN LINUX)

En linux instalar necesitamos instalar las bibliotecas que te permitiran trabajar con archivos .docx:
-	libzip para descomprimir el archivo .docx.
-	libxml2 para procesar el XML dentro del archivo.
Ejecutar el comando: 
sudo apt-get install libzip-dev libxml2-dev

Para compilar usa el comando (en nuestro caso el archivo se llama LeerDoc.c): 
cc -o LeerDoc LeerDoc.c -I/usr/include/libxml2 -lzip -lxml2
Y despues: ./LeerDoc
Se ejecutara el archivo e ingresa la ruta de tu archivo .docx, 
ejemplo " /home/jazmin/Escritorio/SistemasOperativos/Hardware/PorHardware.docx "

Si necesitas la ruta puedes buscarla con el comando, por ejemplo: 
find /home/nombre_pc/Escritorio -name "Nombre_Archivo.docx" */


/* Apartado de bibliotecas */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zip.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#define DOCX_XML_PATH "word/document.xml"

/* Funcion para leer el contenido del archivo XML dentro del .docx */
void leer_texto_docx(const char *ruta_archivo) {
    int err = 0;
    zip_t *zip = zip_open(ruta_archivo, 0, &err);
    if (!zip) {
        printf("Error al abrir el archivo .docx (posiblemente no existe o no se puede leer).\n");
        return;
    }

    printf("Archivo .docx abierto correctamente.\n");

    /* Abrir el archivo document.xml dentro del .docx */
    zip_file_t *xml_file = zip_fopen(zip, DOCX_XML_PATH, 0);
    if (!xml_file) {
        printf("No se pudo encontrar el archivo document.xml dentro del .docx\n");
        zip_close(zip);
        return;
    }

    /* Obtener el tamanio del archivo XML */
    struct zip_stat st;
    zip_stat_init(&st);
    zip_stat(zip, DOCX_XML_PATH, 0, &st);
    char *buffer = malloc(st.size + 1);

    /* Leer el contenido del archivo XML */
    zip_fread(xml_file, buffer, st.size);
    buffer[st.size] = '\0'; /* Asegurar que la cadena está terminada en NULL */

    /* Procesar el XML usando libxml2 */
    xmlDocPtr doc = xmlReadMemory(buffer, st.size, DOCX_XML_PATH, NULL, XML_PARSE_NOERROR | XML_PARSE_NOWARNING);
    if (doc == NULL) {
        printf("Error al analizar el archivo XML\n");
        free(buffer);
        zip_fclose(xml_file);
        zip_close(zip);
        return;
    }

    xmlNodePtr root_element = xmlDocGetRootElement(doc);

    /* Mostrar el nombre del nodo raiz para verificar que el XML se ha cargado correctamente */
    printf("Nodo raíz del XML: %s\n", root_element->name);

    /* Recorrer el XML para inspeccionar todos los nodos */
    printf("Inspeccionando nodos del XML...\n");
    for (xmlNodePtr node = root_element; node; node = node->next) {
        /* Imprimir el nombre de cada nodo y su tipo */
        printf("Nodo: %s, Tipo: %d\n", node->name, node->type);
        
        /* Imprimir el contenido si el nodo tiene contenido */
        if (node->type == XML_ELEMENT_NODE) {
            xmlChar *content = xmlNodeGetContent(node);
            if (content) {
                printf("Contenido del nodo: %s\n", content);
                xmlFree(content);
            }
        }
    }

    /* Limpiar y cerrar archivos */
    xmlFreeDoc(doc);
    free(buffer);
    zip_fclose(xml_file);
    zip_close(zip);
}

int main() {
    char ruta[512];

    /* Solicitar al usuario la ruta del archivo .docx */
    printf("Introduce la ruta del archivo .docx: ");
    fgets(ruta, sizeof(ruta), stdin);
    ruta[strcspn(ruta, "\n")] = '\0';  /* Eliminar el salto de línea al final */

    /* Llamar a la función para leer el texto */
    leer_texto_docx(ruta);

    return 0;
}
