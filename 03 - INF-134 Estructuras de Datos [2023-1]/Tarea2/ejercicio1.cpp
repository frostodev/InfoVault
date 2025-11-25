#include <iostream>
#include <fstream>
using namespace std;

// Struct de Nodo
struct tNodo {
    string operacion;
    int posicion;
    char nucleotido;
    tNodo* sig;
};

// TDA Lista
class tLista {
private:
    tNodo* head;
    tNodo* tail;
    tNodo* curr;
    unsigned int listSize;
    unsigned int pos;

public:
    // Inicializar lista vacía
    tLista() {
        head = NULL;
        tail = NULL;
    }

    /*****
    * void insert
    ******
    * Inserta un nuevo elemento a la lista
    ******
    * Input:
    * string operacion : un string que corresponde a la operacion a realizar
    * int posicion : la posición actual en la lista
    * char nucleotido: el nucleotido a insertar
    *****/

    void insert(string operacion, int posicion, char nucleotido) {

        // Crea un nodo con la información del nuevo elemento
        tNodo* nodoNuevo = new tNodo;
        nodoNuevo -> operacion = operacion;
        nodoNuevo -> posicion = posicion;
        nodoNuevo -> nucleotido = nucleotido;
        nodoNuevo -> sig = NULL;

        // Si la lista está vacía
        if (head == NULL) {
            // El nuevo nodo será el primer y último elemento
            head = nodoNuevo;
            tail = nodoNuevo;
        }
        else {
            // Si no está vacía, agregamos el nuevo nodo y actualizamos el último elemento
            tail -> sig = nodoNuevo;
            tail = nodoNuevo;
        }
    }

    /*****
    * tNodo* getHead
    ******
    * Método que devuelve la primera posición de la lista
    ******
    * Returns:
    * tNodo, devuelve un puntero al nodo en la primera posición de la lista
    *****/
    tNodo* getHead() {
        return head;
    }

    /*****
    * void clear
    ******
    * Método que borra los contenidos de la lista y borra la memoria
    *****/
    void clear() {
        tNodo* nodoActual = head;
        while (nodoActual != NULL) {
            // Crear un nodo temporal
            tNodo* temp = nodoActual;

            // Mover el nodo actual a la siguiente posición
            nodoActual = nodoActual -> sig;

            // Borrar la memoria dinámica del nodo temporal
            delete temp;
        }
        // Borrar puntero del primer valor
        head = NULL;

        // Borrar puntero del último valor
        tail = NULL;
    }
};

/*****
* string aplicarModificaciones
******
* Reconstruye la secuencia con las modificaciones en la lista
******
* Input:
* int l : longitud de la secuencia base
* const string secuenciaBase : la secuencia base a modificar
* tLista listaModificaciones: la lista enlazada que contiene las modificaciones a realizar en formato nodo
******
* Returns:
* string, retorna la secuencia reconstruida con las modificaciones indicadas.
*****/
string aplicarModificaciones(int l, const string& secuenciaBase, tLista& listaModificaciones) {
    string secuencia = secuenciaBase;

    // Iterar sobre la lista hasta el final
    for (tNodo* nodoActual = listaModificaciones.getHead(); nodoActual != NULL; nodoActual = nodoActual -> sig) {
        const string& operacion = nodoActual -> operacion;
        int posicion = nodoActual -> posicion;
        char nucleotido = nodoActual -> nucleotido;

        // Aplicar operaciones
        if (operacion == "INSERTAR") {
            secuencia.insert(posicion, 1, nucleotido);
        }
        else if (operacion == "BORRAR") {
            secuencia.erase(posicion, 1);
        }
        else if (operacion == "INTERCAMBIAR") {
            secuencia[posicion] = nucleotido;
        }
    }

    return secuencia;
}

int main() {
    // Abrir los archivos
    ifstream archivoEntrada;
    ofstream archivoSalida;

    archivoEntrada.open("secuencias-nucleotidos.txt", ios::in);
    archivoSalida.open("secuencias-reconstruidas.txt", ios::out);

    int l;
    int k;

    // Leer la longitud l desde el archivo
    archivoEntrada >> l;
    string secuenciaBase;

    // Leer la secuencia base desde el archivo
    archivoEntrada >> secuenciaBase;
    // Leer la cantidad k de modificaciones a realizar en la secuencia
    archivoEntrada >> k;

    for (int i = 0; i < k; i++) {
        int m;
        // Leer la cantidad m de modificaciones
        archivoEntrada >> m;

        // Crear la lista enlazada de modificaciones
        tLista listaModificaciones;

        for (int j = 0; j < m; j++) {
            string operacionActual;
            char nucleotidoActual = ' ';
            int posicionActual;

            // Leer la posición actual y la operación a realizar desde el archivo
            archivoEntrada >> operacionActual >> posicionActual;
            if (operacionActual != "BORRAR") {
                // En caso de que la operación sea de borrado, cambiamos el valor del nucleótido actual por el del archivo
                archivoEntrada >> nucleotidoActual;
            }

            // Insertar modificación a la lista enlazada
            listaModificaciones.insert(operacionActual, posicionActual, nucleotidoActual);
        }

        // Reconstruir la secuencia con las modificaciones
        string secuenciaNueva = aplicarModificaciones(l, secuenciaBase, listaModificaciones);
        archivoSalida << secuenciaNueva << endl;

        // Limpiar la lista
        listaModificaciones.clear();
    }

    // Cerrar los archivos
    archivoEntrada.close();
    archivoSalida.close();

    return 0;
}