#include <stdlib.h>

#include "Manufactura.h"
#include "Mundo.h"

void step_craft(void *estructura, int x) {
    /*
    *   Nombre: step_craft
    *   Parámetros: void *estructura, int x
    *   Retorno: void
    *   Resumen: Esta función realiza una operación de manufactura en una casilla específica del mundo de juego.
    *            Se encarga de operar sobre los inventarios de la casilla y almacenar el resultado en la salida.
    */

    int coord = x;
    
    // Asignar la estructura proporcionada a la casilla correspondiente en el mundo
    ((casilla* ) (mundo[coord])) -> estructura = estructura;

    // Verificar si el tamaño del inventario es suficiente para realizar una operación de manufactura y si hay espacio en la salida
    if (((manufactura*) (mundo[coord])) ->tamanio >= 2 && ((manufactura*) (mundo[coord])) -> tamanio_salida < 10) {

        // Obtiener los elementos del inventario
        int* primero = ((manufactura*) (mundo[coord])) -> inventario[0];
        int* segundo = ((manufactura*) (mundo[coord])) -> inventario[1];
        
        // Realizar la operación de manufactura y obtener el resultado
        int* resultado = ((manufactura*) (mundo[coord])) -> operar(primero, segundo);

        // Almacenar el resultado en el output y actualizar el tamaño de la salida
        int output = ((manufactura*) (mundo[coord])) -> tamanio_salida - 1;
        ((manufactura*) (mundo[coord])) ->tamanio_salida = ((manufactura*) (mundo[coord])) -> tamanio_salida + 1;
        *((manufactura*) (mundo[coord])) ->salida[output] = *resultado;
        
        // Actualizar el tamaño del inventario y reiniciar los elementos utilizados en la operación
        ((manufactura*) (mundo[coord])) -> tamanio = ((manufactura*) (mundo[coord])) -> tamanio - 2;
        ((manufactura*) (mundo[coord])) -> inventario[0] = 0;
        ((manufactura*) (mundo[coord])) -> inventario[1] = 0;

        // Desplazar los elementos restantes en el inventario
        for (int i = 1; i < 10; i++) {
            *((manufactura*) (mundo[coord])) -> inventario[i-1] = *((manufactura*) (mundo[coord])) -> inventario[i];
        }
    }
}

int *operar_suma(int *a, int *b) {
    /*
    *   Nombre: operar_suma
    *   Parámetros: int *a, int *b
    *   Retorno: int res
    *   Resumen: Esta función realiza la operación de suma entre dos valores de un puntero
    */
    int* res = (int*) malloc(sizeof(int));
    *res = *a + *b;
    return res;
}

int *operar_resta(int *a, int *b) {
    /*
    *   Nombre: operar_resta
    *   Parámetros: int *a, int *b
    *   Retorno: int res
    *   Resumen: Esta función realiza la operación de resta entre dos valores de un puntero
    */
    int* res = (int*) malloc(sizeof(int));
    *res = *a - *b;
    return res;
}

int *operar_multiplicacion(int *a, int *b) {
    /*
    *   Nombre: operar_multiplicacion
    *   Parámetros: int *a, int *b
    *   Retorno: int res
    *   Resumen: Esta función realiza la operación de multiplicación entre dos valores de un puntero
    */
    int* res = (int*) malloc(sizeof(int));
    *res = *a * *b;
    return res;
}

int *operar_division(int *a, int *b) {
    /*
    *   Nombre: operar_division
    *   Parámetros: int *a, int *b
    *   Retorno: int res
    *   Resumen: Esta función realiza la operación de división entre dos valores de un puntero
    */
    int* res = (int*) malloc(sizeof(int));
    *res = *a / *b;
    return res;
}

