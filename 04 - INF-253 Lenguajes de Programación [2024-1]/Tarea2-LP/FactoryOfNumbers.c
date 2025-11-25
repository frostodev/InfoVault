#include <stdlib.h>
#include <stdio.h>
#include "string.h"

#include "Io.h"
#include "Manufactura.h"
#include "Robot.h"
#include "Mundo.h"

void construir(int pos, char tipo, int direccion, int operacion) {
    /*
    *   Nombre: construir
    *   Parámetros: int pos, char tipo, int direccion, int operacion
    *   Retorno: void
    *   Resumen: Esta función construye un robot o una manufactura en una posición determinada
    *            y le proporciona los atributos a cada estructura
    */

    // Robot
    if (tipo == 'R') {
        ((casilla *) (mundo[pos])) -> tipo_estructura = 'R';
        ((casilla *) (mundo[pos])) -> estructura = (void*) malloc(sizeof(robot));
        ((casilla *) (mundo[pos])) -> step = step_robot;

        ((robot *) ((casilla *) (mundo[pos])) -> estructura) -> inventario = (int *) malloc(sizeof(int));
        *((robot *) ((casilla *) (mundo[pos])) -> estructura) -> inventario = 0;
        ((robot *) ((casilla *) (mundo[pos])) -> estructura) -> tiene_inventario = 'F';
        ((robot *) ((casilla *) (mundo[pos])) -> estructura) -> direccion = direccion;
    }

    // Manufactura
    if (tipo == 'M') {

        // Definimos los atributos de la estructura
        ((casilla *) (mundo[pos])) -> tipo_estructura = 'M';
        ((casilla *) (mundo[pos])) -> estructura = (void*) malloc(sizeof(manufactura));
        ((casilla *) (mundo[pos])) -> step = step_craft;
        
        ((manufactura *) ((casilla *) (mundo[pos])) -> estructura) -> tamanio = 0;
        ((manufactura *) ((casilla *) (mundo[pos])) -> estructura) -> tamanio_salida = 0;
        
        for (int i = 0; i < 10; i++) {
            ((manufactura *) ((casilla *) (mundo[pos])) -> estructura) -> inventario[i] = malloc(sizeof(int));
            *((manufactura *) ((casilla *) (mundo[pos])) -> estructura) -> inventario[i] = 0;
            ((manufactura *) ((casilla *) (mundo[pos])) -> estructura) -> salida[i] = malloc(sizeof(int));
            *((manufactura *) ((casilla *) (mundo[pos])) -> estructura) -> salida[i] = 0;
        }

        // Suma
        if (operacion == 1) {
            ((manufactura *) ((casilla *) (mundo[pos])) -> estructura) -> operar = &operar_suma;
        }

        // Resta
        if (operacion == 2) {
            ((manufactura *) ((casilla *) (mundo[pos])) -> estructura) -> operar = &operar_resta;
        }

        // Multiplicación
        if (operacion == 3) {
            ((manufactura *) ((casilla *) (mundo[pos])) -> estructura) -> operar = &operar_multiplicacion;
        }

        // División
        if (operacion == 4) {
            ((manufactura *) ((casilla *) (mundo[pos])) -> estructura) -> operar = &operar_division;
        }

    }
}

void destruir(int pos) {
    /*
    *   Nombre: destruir
    *   Parámetros: int pos
    *   Retorno: void
    *   Resumen: Esta función destruye un robot o una manufactura en una posición determinada
    *            y se asegura de liberar la memoria asociada
    */

    // Definimos nuestra estructura
    void* estructura = ((casilla*) (mundo[pos])) -> estructura;

    // Robot
    if (((casilla*) (mundo[pos])) -> tipo_estructura == 'R')
    {
        free(((robot *) ((casilla *) (mundo[pos])) -> estructura) -> inventario);
        free(estructura);

    // Manufactura
    } else if (((casilla*) (mundo[pos])) -> tipo_estructura == 'M'){
        for (int i = 0; i < 10; i++)
        {
            free(((manufactura *) ((casilla *) (mundo[pos])) -> estructura) -> inventario[i]);
            free(((manufactura *) ((casilla *) (mundo[pos])) -> estructura) -> salida[i]);
        }
        free(estructura);
    }

    // Una vez se hayan eliminado los contenidos de la casilla, asignarle tipo Vacío.
    ((casilla*) (mundo[pos])) -> tipo_estructura = 'V';
}

void informacion(int pos, int tam) {
    /*
    *   Nombre: informacion
    *   Parámetros: int pos, int tam
    *   Retorno: void
    *   Resumen: Esta función muestra información de cada casilla y sus atributos
    */

    printf("\n-------------------- \n");

    // Casos base para la entrada
    if (pos == 0) {
        printf("Tipo de estructura: I/O\n");
        printf("Tipo exacto: entrada (input)\n");
    }
    
    // Caso base para la salida
    if (pos == tam - 1) {
        printf("Tipo de estructura: I/O\n");
        printf("Tipo exacto: salida (output)\n");
    }

    // Robot
    if (((casilla*) (mundo[pos])) -> tipo_estructura == 'R') {
        printf("Tipo de estructura: Robot\n");

        // Inventario
        if (((robot*) (mundo[pos])) -> tiene_inventario == 'T') {
            int inv = *((robot*) (mundo[pos])) -> inventario;
            printf("Inventario: %d\n", inv);
        }

        if (((robot*) (mundo[pos])) -> tiene_inventario == 'F') {
            printf("El inventario esta vacio.\n");
        }
    }
    
    if (((casilla*) (mundo[pos])) -> tipo_estructura == 'M')
    {
        printf("Tipo de estructura: Manufactura\n");

        if (((manufactura*) ((casilla*) (mundo[pos])) -> estructura) -> operar == operar_suma) {
            printf("Operacion especifica: +\n");
        }

        if (((manufactura*) ((casilla*) (mundo[pos])) -> estructura) -> operar == operar_resta) {
            printf("Operacion especifica: -\n");
        }

        if (((manufactura*) ((casilla*) (mundo[pos])) -> estructura) -> operar == operar_multiplicacion) {
            printf("Operacion especifica: *\n");
        }

        if (((manufactura*) ((casilla*) (mundo[pos])) -> estructura) -> operar == operar_division) {
            printf("Operacion especifica: /\n");
        }

        if (((manufactura*) ((casilla*) (mundo[pos])) -> estructura) -> tamanio == 0) {
            printf("Inventario: \n");

        } else {
            printf("Inventario: ");
            for (int i = 0; i < ((manufactura*) ((casilla*) (mundo[pos])) -> estructura) -> tamanio; i++)
            {
                printf("%d\n", *((manufactura*) ((casilla*) (mundo[pos])) -> estructura) -> inventario[i]);
            }
            printf("\n");
        }
        if (((manufactura*) ((casilla*) (mundo[pos])) -> estructura) -> tamanio_salida == 0) {
            printf("Salida: \n");
        } else {
            printf("Salida: ");
            for (int i = 0; i < ((manufactura*) ((casilla*) (mundo[pos])) -> estructura) -> tamanio_salida; i++)
            {
                printf("%d", *((manufactura*) ((casilla*) (mundo[pos])) -> estructura) -> salida[i]);
            }
            printf("\n");
        }
    }

    // Para las casillas vacías
    if (((casilla*) (mundo[pos])) -> tipo_estructura == 'V')
    {
        printf("CASILLA VACIA\n");
    }

    printf("-------------------- \n");
}

void salir() {
    /*
    *   Nombre: salir
    *   Parámetros: void
    *   Retorno: void
    *   Resumen: Esta función se asegura de borrar el mundo antes de salir
    */

    printf("Saliendo...\n");
    borrar_mundo();
    exit(0);
}

int main() {
    /*
    *   Nombre: main
    *   Parámetros: void
    *   Retorno: void
    *   Resumen: Función principal
    */

    printf("Factory of Numbers\n");

    int largoFabrica = 0;

    // Asegurarnos de que la fabrica tenga largo mayor a 5
    while (1) {
        printf("Ingrese un largo\n");
        scanf("%d", &largoFabrica);

        if (largoFabrica >= 5) {
            break;
        } else {
            printf("Largo invalido. Debe ser mayor o igual a 5. \n");
        }
    }

    // Crear el mundo
    crear_mundo(largoFabrica);

    // Ciclo principal
    while (1) {

        printf("\n");
        mostrar_mundo();
        printf("\n");
        printf("MENU PRINCIPAL\n");
        printf("1.- Construir 2.- Destruir\n");
        printf("3.- Simular 4.- Informacion extra\n");
        printf("5.- Salir \n");

        printf("Ingrese una accion: \n");

        int seleccion;
        scanf("%d", &seleccion);
        switch (seleccion) {

        case 1: {

            int subseleccion;
            printf("Elija una opción para construir: \n");
            printf("1.- Robot \n");
            printf("2.- Manufactura \n");
            printf(">> ");
            scanf("%d", &subseleccion);

            // Robot
            if (subseleccion == 1) {
                int seleccionDireccion;
                printf("Direccion inicial: \n");
                printf("1.- Izquierda \n");
                printf("2.- Derecha \n");
                printf(">> ");
                scanf("%d", &seleccionDireccion);

                // Izquierda
                if (seleccionDireccion == 1) {

                    int pos;
                    printf("Posicion: \n");
                    printf("De 1 a %d", largoFabrica);
                    printf(">> ");                   
                    scanf("%d", &pos);

                    if (pos == 1 || pos == largoFabrica) {
                        printf("POSICION INVALIDA. \n");                                
                    } else {
                        construir(pos - 1, 'R', 0, 0);
                        break;
                    }

                // Derecha
                } else if (seleccionDireccion == 2) {

                    int pos;
                    printf("Posicion: \n");
                    printf("De 1 a %d", largoFabrica);
                    printf(">> ");                   
                    scanf("%d", &pos);

                    if (pos == 1 || pos == largoFabrica) {
                        printf("POSICION INVALIDA. \n");                        
                    } else {
                        construir(pos - 1, 'R', 1, 0);
                        break;
                    }

                // Inválido
                } else {
                    break;
                }

            // Manufactura
            } else if (subseleccion == 2) {

                int posMan;
                printf("Posicion: \n");
                printf("De 1 a %d", largoFabrica);
                printf(">> ");                   
                scanf("%d", &posMan);

                if (posMan == 1 || posMan == largoFabrica) {
                    printf("POSICION INVALIDA. \n");                                
                }

                int seleccionOperacion;
                printf("Elija una operación: \n");
                printf("1.- Sumar \n");
                printf("2.- Restar \n");
                printf("3.- Multiplicar \n");
                printf("4.- Dividir \n");
                printf(">> ");
                scanf("%d", &seleccionOperacion);

                // Suma
                if (seleccionOperacion == 1) {
                    construir(posMan - 1, 'M', 0, 1);
                // Resta
                } else if (seleccionOperacion == 2) {
                    construir(posMan - 1, 'M', 0, 2);

                // Multiplicar
                } else if (seleccionOperacion == 3) {
                    construir(posMan - 1, 'M', 0, 3);

                // Dividir
                } else if (seleccionOperacion == 4) {
                    construir(posMan - 1, 'M', 0, 4);

                // Inválido
                } else {
                    break;
                }


            // Inválido
            } else {
                break;
            }

            break;
        }

        // Eliminar
        case 2: {

            int posdel;
            printf("Posicion: \n");
            printf("De 1 a %d", largoFabrica);
            printf(">> ");                   
            scanf("%d", &posdel);

            if (posdel == 1 || posdel == largoFabrica) {
                printf("No se pueden eliminar las entradas/salidas!\n");
                break;
            }

            destruir(posdel - 1);

            break;
        }

        // Simular
        case 3: {

            int ticks;
            printf("Cantidad de ticks a simular?: \n");
            printf(">> ");                   
            scanf("%d", &ticks);

            simular(ticks);
            break;
        }

        // Información extra
        case 4: {
            int posExtra;

            printf("Posicion: \n");
            printf("De 1 a %d", largoFabrica);
            printf(">> ");                  
            scanf("%d", &posExtra);

            informacion(posExtra - 1, largoFabrica);
            break;
        }

        // Salir
        case 5: {
            salir();
            break;
        }

        // Caso default
        default: {
            break;
        }
        }
    }

    return 0;
}
