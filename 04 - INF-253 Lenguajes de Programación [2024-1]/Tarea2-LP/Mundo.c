#include <stdlib.h>
#include <stdio.h>

#include "Mundo.h"
#include "Io.h"
#include "Robot.h"

// Mundo y tamaño como variables globales
void **mundo;
unsigned int tam;

void crear_mundo(int tamanio) {
    /*
    *   Nombre: crear_mundo
    *   Parámetros: int tamanio
    *   Retorno: void
    *   Resumen: Esta función crea el mundo de juego. Para ello asigna memoria a cada casilla
    */

    // Alojar memoria para el mundo (un array de punteros a casillas)
    mundo = (void **) malloc(tamanio * sizeof(void *));

    // Inicializar cada casilla del mundo 
    for (int i = 0; i < tamanio; i++) {
        // Asignar un bloque de memoria para cada casilla
        mundo[i] = (void*) malloc(sizeof(casilla));

        // Asignar los atributos de casilla vacía
        ((casilla *) (mundo[i])) -> tipo_estructura = 'V';
        ((casilla *) (mundo[i])) -> estructura = NULL;
        ((casilla *) (mundo[i])) -> step = NULL;
    }

    // Inicializar las entradas y salidas
    // La primera casilla es siempre entrada
    ((casilla *) (mundo[0])) -> tipo_estructura = 'E';
    ((casilla *) (mundo[0])) -> estructura = (void*) malloc(sizeof(io));

    ((io*) (((casilla *) (mundo[0])) -> estructura)) -> entrada_o_salida = 'E';
    ((io*) (((casilla *) (mundo[0])) -> estructura)) -> objeto = malloc(sizeof(int));

    // La entrada genera una cantidad ilimitada de unos
    *((int*) (((io*) (((casilla *) (mundo[0])) -> estructura)) -> objeto)) = 1;

    // La última casilla es siempre salida, y su posición sería tamanio-1
    ((casilla *) (mundo[tamanio - 1])) -> tipo_estructura = 'S';
    ((casilla *) (mundo[tamanio - 1])) -> estructura = (void*) malloc(sizeof(io));

    ((io*) (((casilla *) (mundo[tamanio - 1])) -> estructura)) -> entrada_o_salida = 'S';
    ((io*) (((casilla *) (mundo[tamanio - 1])) -> estructura)) -> objeto = malloc(sizeof(int));

    // La salida no tiene objeto inicial
    *((int*) (((io*) (((casilla *) (mundo[tamanio - 1])) -> estructura))->objeto)) = 0;

    // Asignar a la variable global de tamaño su valor correspondiente
    tam = tamanio;
}

void mostrar_mundo() {
    /*
    *   Nombre: mostrar_mundo
    *   Parámetros: void
    *   Retorno: void
    *   Resumen: Esta función recorre cada casilla del mundo y muestra su contenido en pantalla
    */

    printf("|");
    for (unsigned int i = 0; i < tam; i++) {

        printf(" ");
        
        // Mostrar el robot
        if (((casilla*) (mundo[i])) -> tipo_estructura == 'R') {
            robot *robot = ((casilla*) (mundo[i])) -> estructura;
            printf("R");
            if (robot -> direccion == 0) {
                printf("<");
            } else if (robot -> direccion == 1) {
                printf(">");
            }

        // Mostrar la manufactura
        } else if (((casilla*) (mundo[i])) -> tipo_estructura == 'M') {

            // Mostrar la operación asociada
            if (((manufactura *) ((casilla*) (mundo[i])) -> estructura) -> operar == operar_suma) {
                printf("M+");
            } else if (((manufactura *) ((casilla*) (mundo[i])) -> estructura) -> operar == operar_resta) {
                printf("M-");
            } else if (((manufactura *) ((casilla*) (mundo[i])) -> estructura) -> operar == operar_multiplicacion) {
                printf("M*");
            } else if (((manufactura *) ((casilla*) (mundo[i])) -> estructura) -> operar == operar_division) {
                printf("M/");
            }

        } else {
            printf("%c", ((casilla*) (mundo[i])) -> tipo_estructura);
        }
        printf(" |");
    }

    printf("\n");
}

void borrar_mundo() {
    /*
    *   Nombre: borrar_mundo
    *   Parámetros: void
    *   Retorno: void
    *   Resumen: Esta función borra los contenidos de cada casilla y luego borra el mundo entero
    */

    for (unsigned int i = 0; i < tam; i++) {

        // Liberar el inventario del robot
        if (((casilla*) (mundo[i])) -> tipo_estructura == 'R') {
            if (((robot*) ((casilla*) (mundo[i])) -> estructura) -> inventario != NULL){
                free(((robot*) ((casilla*)(mundo[i])) -> estructura) -> inventario);
            }
        }

        // Liberar la manufactura
        else if (((casilla*) (mundo[i])) -> tipo_estructura == 'M') {
            for (int j = 0; j < 10; j++) {

                // Liberar el inventario
                if (((manufactura*) ((casilla*) (mundo[i])) -> estructura) -> inventario[j] != NULL) {
                    free(((manufactura*) ((casilla*) (mundo[i])) -> estructura) -> inventario[j]);
                    ((manufactura*) ((casilla*) (mundo[i])) -> estructura) -> inventario[j] = NULL;
                }

                // Liberar la salida
                if (((manufactura*) ((casilla*) (mundo[i])) -> estructura) -> salida[j] != NULL) {
                    free(((manufactura*) ((casilla*) (mundo[i])) -> estructura) -> salida[j]);
                    ((manufactura*) ((casilla*) (mundo[i])) -> estructura) -> salida[j] = NULL;
                }
            }
        }

        // Limpiar la entrada
        else if (((casilla*) (mundo[i])) -> tipo_estructura == 'E') {
            if (((io*) ((casilla*) (mundo[i])) -> estructura) -> objeto != NULL) {
                free(((io*) ((casilla*) (mundo[i])) -> estructura) -> objeto);
            }
        }

        // Limpiar la salida
        else if (((casilla*) (mundo[i])) -> tipo_estructura == 'S') {
            if (((io*) ((casilla*) (mundo[i])) -> estructura) -> objeto != NULL) {
                free(((io*) ((casilla*)(mundo[i])) -> estructura) -> objeto);
            }
        }

        // Limpiar denuevo una casilla destruida causa un error en Valgrind, asi que nos aseguramos que no pase
        if (((casilla*)(mundo[i])) -> tipo_estructura != 'V') {
            free(((casilla*)(mundo[i])) -> estructura);
        }

        free(mundo[i]);
    }

    // Liberar memoria asignada al array de punteros a casillas
    free(mundo);
    mundo = NULL;
}

void simular(int ticks) {
    /*
    *   Nombre: simular
    *   Parámetros: int ticks
    *   Retorno: void
    *   Resumen: Esta funcion intenta simular el estado siguiente del mundo.
    *            Para ello, se usa una cantidad de 'ticks' que indican cuantos 'ciclos' se deben simular.
    */
    
    for (int i = 0; i < ticks; i++)
    {
        printf("Tick a simular: %d\n", i+1);
        for (unsigned int j = 1; j < tam - 2; j++) {

            // Verifica si el objeto en la primera casilla es nulo, si es así, lo inicializa con un valor de 1
            if (((io *) ((casilla *) (mundo[0])) -> estructura) -> objeto == NULL) {
                ((io *) ((casilla *) (mundo[0])) -> estructura) -> objeto = malloc(sizeof(int));
                *((io *) ((casilla *) (mundo[0])) -> estructura) -> objeto= 1;
            }

            // Si la casilla actual tiene una función 'step' definida, la ejecuta pasando la estructura de la casilla
            // y el número de tick como parámetros
            if (((casilla *) (mundo[i])) -> step != NULL) {
                ((casilla *) (mundo[i])) -> step(((casilla *) (mundo[i])) -> estructura, i);
            }
            
            if (*((io *) ((casilla *) (mundo[tam - 1])) -> estructura) -> objeto != 0) {
                printf("Resultado: %d", *((io*) ((casilla*) (mundo[tam - 1])) -> estructura) -> objeto);

                // Liberar el objeto anterior
                free(((io *) ((casilla *) (mundo[tam - 1])) -> estructura) -> objeto);

                ((io *) ((casilla *) (mundo[tam - 1])) -> estructura) -> objeto = malloc(sizeof(int));
                *((io *) ((casilla *) (mundo[tam - 1])) -> estructura) -> objeto = 0;
            }
        }
        printf("Tick %d Simulado.\n", i+1);
    }
    printf("Simulacion completada.");
}
