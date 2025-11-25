#ifndef MUNDO_H
#define MUNDO_H

typedef struct casilla
{
    char tipo_estructura;
    void *estructura;
    void (*step)(void *estructura, int x);
} casilla;

extern void **mundo;

void crear_mundo(int tamanio);
void mostrar_mundo();
void borrar_mundo();
void simular(int ticks);

#endif