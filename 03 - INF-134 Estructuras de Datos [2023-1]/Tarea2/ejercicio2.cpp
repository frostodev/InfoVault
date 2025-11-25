#include <iostream>
#include <string>

using namespace std;

// Definir el tipo de datos
typedef int tipoElem;

// Struct de nodo del árbol de búsqueda binaria
struct tNodoArbolBin {
    tipoElem info;
    tNodoArbolBin* izq;
    tNodoArbolBin* der;
};

/*****
* tNodoArbolBin crearNodo
******
* Crea un nuevo nodo con el elemento indicado
******
* Input:
* tipoElem valor: el elemento a insertar
******
* Returns:
* tNodoArbolBin*: el puntero al nuevo nodo
*****/

tNodoArbolBin* crearNodo(tipoElem valor) {
    tNodoArbolBin* nuevoNodo = new tNodoArbolBin;
    nuevoNodo -> info = valor;
    nuevoNodo -> izq = NULL;
    nuevoNodo -> der = NULL;
    return nuevoNodo;
}

// Clase para el árbol de búsqueda binaria
class tABB {

    private:
        tNodoArbolBin* raiz;
        int nElems;
        /*****
        *tNodoArbolBin insertHelp
        ******
        *Función auxiliar para la inserción de un elemento en el árbol
        ******
        * Input:
        * tipoElem dato: El valor del nodo
        * tNodoArbolbin nodo: el nodo
        ******
        * Return:
        * nodo: el nodo con su nuevo valor
        *****/
        tNodoArbolBin* insertHelp(tNodoArbolBin* nodo, tipoElem dato) {
            if (nodo == NULL) {
                return crearNodo(dato);
            }

            if (dato < nodo -> info) {
                nodo -> izq = insertHelp(nodo -> izq, dato);
            } else if (dato > nodo->info) {
                nodo -> der = insertHelp(nodo -> der, dato);
            }

            return nodo;
        }
        /*****
        * bool findHelp:
        ******
        * Busca un valor en un ABB T, llamando a la función findHelp y retorna 0 en caso de no encontrarlo
        ******
        * Input:
        * tNodoArbolbin nodo: el nodo
        * tipoElem dato: el valor del nodo
        ******
        * Returns:
        * bool: 0 en caso de no encontrar nada, de lo contrario, el valor del nodo
        *****/
        bool findHelp(tNodoArbolBin* nodo, tipoElem dato) {
            if (nodo == NULL) {
                return false;
            }

            if (dato == nodo -> info) {
                return true;
            } else if (dato < nodo -> info) {
                return findHelp(nodo -> izq, dato);
            } else {
                return findHelp(nodo -> der, dato);
            }
        }
        /*****
        * int lower_boundhelp:
        ******
        * Función auxiliar para encontrar el límite inferior de un valor en el árbol
        ******
        * Input:
        * tipoElem dato: valor del nodo
        * tNodoArbolbin nodo: el nodo
        ******
        * Returns:
        * int resultado: el valor del límite inferior del árbol
        *****/
        int lower_boundHelp(tNodoArbolBin* nodo, tipoElem dato) {
            int resultado = -1;
            while (nodo != NULL) {
                if (nodo -> info <= dato) {
                    resultado = nodo -> info;
                    nodo = nodo -> der;
                } else {
                    nodo = nodo -> izq;
                }
            }
            return resultado;
        }

    public:
        //Creación del árbol binario
        tABB() {
            raiz = NULL;
            nElems = 0;
        }
        // Inserta un elemento
        void insert(tipoElem dato) {
            raiz = insertHelp(raiz, dato);
        }

        // Método para buscar un elemento en el árbol
        bool find(tipoElem dato) {
            return findHelp(raiz, dato);
        }

        // Método para encontrar el límite inferior de un valor en el árbol
        int lower_bound(tipoElem dato) {
            return lower_boundHelp(raiz, dato);
        }

    };

int main() {

    // Primero se crea un árbol de búsqueda binaria
    tABB abb;
    int totalEncendidos = 0;

    string entrada;

    // Se lee la entrada hasta que se ingrese la función PARAR_PROGRAMA
    while (getline(cin, entrada)) {
        unsigned int posicion = 0;
        string comando;
        int valor = 0;

        // Lee el comando desde la entrada
        while (posicion < entrada.length() && entrada[posicion] != ' ') {
            comando += entrada[posicion];
            posicion++; 
        }
        // Avanza a la siguiente posición después del espacio en blanco
        posicion++;

        // Extrae el valor de la línea de entrada, si hay alguno
        if (posicion < entrada.length()) {
            while (posicion < entrada.length() && (entrada[posicion] >= '0' && entrada[posicion] <= '9')) {
                valor = valor * 10 + (entrada[posicion] - '0');
                posicion++;
            }
        }

        // Verificar el comando ingresado y realizar la acción correspondiente
        if (comando == "ENCENDER") {
            // Si el valor no está presente en el árbol, se inserta y se incrementa el contador de luces encendidas
            if (!abb.find(valor)) {
                abb.insert(valor);
                totalEncendidos++;
            }
        } else if (comando == "CUANTOS_ENCENDER") {
            // Encontrar el límite inferior del valor en el árbol y calcular la cantidad de luces apagadas
            int limiteInferior = abb.lower_bound(valor);
            int apagados = (limiteInferior == -1) ? valor + 1 : valor - limiteInferior;
            cout << apagados << endl;
        } else if (comando == "PARAR_PROGRAMA") {
            // Imprimir el número total de elementos encendidos y salir del bucle while
            cout << totalEncendidos << endl;
            break;
        }
    }

    return 0;
}