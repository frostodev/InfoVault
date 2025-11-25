#ifndef MANUFACTURA_H
#define MANUFACTURA_H

typedef struct manufactura
{
    int *inventario[10];
    int tamanio;
    int *salida[10];
    int tamanio_salida;
    int *(*operar)(int *a, int *b);

} manufactura;

void step_craft(void *estructura, int x);
int *operar_suma(int *a, int *b);
int *operar_resta(int *a, int *b);
int *operar_division(int *a, int *b);
int *operar_multiplicacion(int *a, int *b);

#endif