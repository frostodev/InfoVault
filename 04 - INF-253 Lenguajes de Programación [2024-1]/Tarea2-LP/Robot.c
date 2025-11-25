#include <stdlib.h>
#include <stdio.h>

#include "Robot.h"
#include "Mundo.h"
#include "Io.h"
#include "Manufactura.h"

void step_robot(void *estructura, int x) {
    /*
    *   Nombre: step_robot
    *   Parámetros: void *estructura, int x
    *   Retorno: void
    *   Resumen: Esta función simula el movimiento y las interacciones de un robot. El robot se mueve una casilla a la vez
    *            y realiza diferentes acciones según el tipo de casilla adyacente y su dirección.
    */

    // Asigna la estructura proporcionada a la casilla correspondiente en el mundo
    ((casilla* ) (mundo[x])) -> estructura = estructura;

    // Obtiene el tipo de estructura de las casillas adyacentes
    char anterior = ((casilla *) (mundo[x - 1])) -> tipo_estructura;
    char siguiente = ((casilla *) (mundo[x + 1])) -> tipo_estructura;

    // Obtiene la dirección en la que se mueve el robot
    int dcc = ((robot*) (mundo[x])) -> direccion;

    // Si el robot se mueve hacia la izquierda y la casilla adyacente es vacía
    if (dcc == 0 && anterior == 'V') {
        // Mueve el robot a la izquierda y ajusta las estructuras correspondientes
        // Obtener el robot en la posición actual
        robot *actual = ((casilla*) (mundo[x])) -> estructura;

        // Cambiar el tipo de estructura de la casilla adyacente a 'R' (robot)
        ((casilla *) (mundo[x - 1])) -> tipo_estructura = 'R';

        // Asignar la estructura del robot a la casilla adyacente a la izquierda
        ((casilla *) (mundo[x - 1])) -> estructura = (void*) malloc(sizeof(robot));

        // Obtener el robot en la casilla adyacente a la derecha
        robot *izquierda = ((casilla *)(mundo[x + 1]))->estructura;

        // Copiar los atributos del robot actual al robot en la casilla adyacente a la izquierda
        izquierda -> direccion = actual -> direccion;
        izquierda -> tiene_inventario = actual -> tiene_inventario;

        // Asignar memoria y copiar el inventario del robot actual al robot en la casilla adyacente a la izquierda
        izquierda -> inventario = (int *) malloc(sizeof(int));
        *(izquierda -> inventario) = *(actual -> inventario);
        
        // Cambiar el tipo de estructura de la casilla actual a vacía ('V')
        ((casilla *) (mundo[x])) -> tipo_estructura = 'V';

        // Liberar la memoria del inventario y la estructura
        free(actual -> inventario);
        free(((casilla *) (mundo[x])) -> estructura);

        // Desactivar el puntero a función step en la casilla actual
        ((casilla *) (mundo[x])) -> step = NULL;
    }

    // Si el robot se mueve hacia la derecha y la casilla adyacente es vacía
    if (dcc == 1 && siguiente == 'V') {
        // Mueve el robot a la derecha y ajusta las estructuras correspondientes

        // Obtener el robot en la posición actual
        robot *actual = ((casilla*) (mundo[x])) -> estructura;

        // Cambiar el tipo de estructura de la casilla adyacente a 'R' (robot)
        ((casilla *) (mundo[x + 1])) -> tipo_estructura = 'R';

        // Asignar la estructura del robot a la casilla adyacente a la derecha
        ((casilla *) (mundo[x + 1])) -> estructura = (void*) malloc(sizeof(robot));

        // Obtener el robot en la casilla adyacente a la derecha
        robot *derecha = ((casilla *) (mundo[x + 1])) -> estructura;

        // Copiar los atributos del robot actual al robot en la casilla adyacente a la derecha
        derecha -> direccion = actual -> direccion;
        derecha -> tiene_inventario = actual -> tiene_inventario;

        // Asignar memoria y copiar el inventario del robot actual al robot en la casilla adyacente a la derecha
        derecha -> inventario = (int *) malloc(sizeof(int));
        *(derecha -> inventario) = *(actual -> inventario);
        
        // Cambiar el tipo de estructura de la casilla actual a vacía ('V')
        ((casilla *) (mundo[x])) -> tipo_estructura = 'V';

        // Liberar la memoria del inventario y de la estructura
        free(((robot*) ((casilla*) (mundo[x])) -> estructura) -> inventario);
        free(((casilla *) (mundo[x])) -> estructura);

        // Desactivar el puntero a función step en la casilla actual
        ((casilla *) (mundo[x])) -> step = NULL;
    }

    // Si el robot se mueve hacia la derecha y la casilla adyacente es otro robot
    if (dcc == 1 && siguiente == 'R') {
        // Cambia la dirección del robot hacia la izquierda
        ((robot*) (mundo[x])) -> direccion = 0;
    }
    
    // Si el robot se mueve hacia la izquierda y la casilla adyacente es otro robot
    if (dcc == 0 && anterior == 'R') {
        // Cambia la dirección del robot hacia la derecha
        ((robot*) (mundo[x])) -> direccion = 1;
    }

    // Si el robot se mueve hacia la izquierda y la casilla adyacente es una manufactura
    if (dcc == 0 && anterior == 'M') {
        // Realiza interacciones con la manufactura si el robot tiene un objeto en su inventario
        robot *rob = ((casilla *) (mundo[x])) -> estructura;
        if(rob -> tiene_inventario == 'F') {
            manufactura *manf = ((casilla *) (mundo[x - 1])) -> estructura;
            interactuar_manufactura(manf, rob);
        }
        ((robot *) (mundo[x])) -> direccion = 1;
    }

    // Si el robot se mueve hacia la derecha y la casilla adyacente es una manufactura
    if (dcc == 1 && siguiente == 'M') {
        // Realiza interacciones con la manufactura si el robot no tiene un objeto en su inventario
        robot *rob = ((casilla *) (mundo[x])) -> estructura;
        if(rob -> tiene_inventario == 'T'){
            manufactura *manf = ((casilla *) (mundo[x + 1])) -> estructura;
            interactuar_manufactura(manf, rob);
        }
        ((robot *) (mundo[x])) -> direccion = 0;
    }
    
    // Si el robot se mueve hacia la izquierda y la casilla adyacente es una entrada
    if (dcc == 0 && anterior == 'E') {
        // Realiza interacciones con la entrada si el robot no tiene un objeto en su inventario
        robot *rob = ((casilla *) (mundo[x])) -> estructura;
        if (rob -> tiene_inventario == 'F')
        {
            io *io = ((casilla *) (mundo[x - 1])) -> estructura;
            interactuar_io(io, rob);
        }
        rob -> direccion = 2;
    }

    // Si el robot se mueve hacia la derecha y la casilla adyacente es una salida
    if (dcc == 2 && siguiente == 'S') {
        // Realiza interacciones con la salida si el robot no tiene un objeto en su inventario
        robot *rob = ((casilla *) (mundo[x])) -> estructura;
        if (rob -> tiene_inventario == 'F')
        {
            io *io = ((casilla *) (mundo[x + 1])) -> estructura;
            interactuar_io(io, rob);
        }
        ((robot *) (mundo[x])) -> direccion = 0;
    }
}


void interactuar_manufactura(manufactura *manf, robot *rob) {
    /*
    *   Nombre: interactuar_manufactura
    *   Parámetros: manufactura *manf, robot *rob
    *   Retorno: void
    *   Resumen: Esta función simula la interacción entre un robot y una línea de manufactura.
    */

    // Si hay productos en salida y el robot está en la posición inicial
    if (manf -> tamanio_salida != 0 && rob -> direccion == 0)
    {
        // El robot recoge el primer producto disponible
        rob -> tiene_inventario = 'T';                  // Indica que el robot tiene un objeto en su inventario
        *rob -> inventario = *manf -> salida[0];        // Transfiere el producto de la salida de manufactura al inventario del robot
        rob -> direccion = 1;                           // Cambia la dirección del robot para que se mueva hacia la línea de manufactura

        // Desplaza los productos restantes en la salida de la línea de manufactura hacia adelante
        for (int i = 1; i < 10; i++)
        {
            *manf -> salida[i-1] = *manf -> salida[i];
        }
    }

    // Si el robot está en dirección de regreso y hay espacio en la línea de manufactura
    if (rob -> direccion == 1 && manf -> tamanio < 10)
    {
        // El robot coloca su producto en la línea de manufactura
        rob -> tiene_inventario = 'F';                        // Indica que el robot no tiene objeto en su inventario
        int cllenas = manf -> tamanio;
        *manf -> inventario[cllenas] = *rob -> inventario;    // Transfiere el producto del inventario del robot a la línea de manufactura
        free(rob -> inventario);                              // Libera la memoria del inventario anterior del robot
        rob -> inventario = (int *) malloc(sizeof(int));      // Asigna memoria para un nuevo inventario al robot
        rob -> direccion = 0;                                 // Cambia la dirección del robot para que se mueva hacia el punto de inicio
    }
    
    // Si no hay productos en salida y el robot está en la posición inicial
    if (manf -> tamanio_salida == 0 && rob -> direccion == 0)
    {
        // El robot simplemente cambia de dirección
        rob -> direccion = 1;
    }
}


void interactuar_io(io *io, robot *rob) {
    /*
    *   Nombre: interactuar_io
    *   Parámetros: io *io, robot *rob
    *   Retorno: void
    *   Resumen: Esta función maneja la interacción entre un robot y un dispositivo de entrada/salida (io).
    *            Si el dispositivo es de entrada, el robot recoge el objeto del dispositivo y lo guarda en su inventario.
    *            Si el dispositivo es de salida, el robot coloca el objeto de su inventario en el dispositivo.
    */

    // Si el dispositivo es de entrada
    if (io -> entrada_o_salida == 'E')
    {
        // El robot adquiere el objeto del dispositivo
        *rob -> inventario = *io -> objeto;     // Transfiere el objeto del dispositivo al inventario del robot
        rob -> tiene_inventario = 'T';          // (True) Indica que el robot tiene un objeto en su inventario
    }
    
    // Si el dispositivo es de salida
    if (io -> entrada_o_salida == 'S')
    {
        // El robot coloca su objeto en el dispositivo
        *io -> objeto = *rob -> inventario;     // Transfiere el objeto del inventario del robot al dispositivo
        *rob -> inventario = 0;                 // Borra el objeto del inventario del robot
        rob -> tiene_inventario = 'F';          // (False) Indica que el robot no tiene objeto en su inventario    
    }
}