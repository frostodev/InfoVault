#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

// Constantes de servicios
int const SERV_DESAYUNO = 0;
int const SERV_ALMUERZO = 1;
int const SERV_ONCE = 2;
int const SERV_CENA = 3;

// Struct para saldo del Colaborador
struct SaldoColaborador {
    char rut[11];
    int saldo_desayuno;
    int saldo_almuerzo;
    int saldo_once;
    int saldo_cena;
};

/*****
* bool puedeConsumir
******
* Devuelve true si el colaborador tiene saldo suficiente para consumir; devuelve false en caso contrario.
******
* Input:
* char* rut : RUT del colaborador
* int servicio: Tipo de servicio correspondiente a las constantes definidas anteriormente
* string consumos_dia: Nombre del archivo ASCII donde se almacena el registro de consumos del colaborador
******
* Returns:
* bool, true en caso de tener saldo, false en caso de no tener.
*****/

bool puedeConsumir(char* rut, int servicio, string consumos_dia) {

    // Abrir el archivo de saldos en modo binario
    ifstream archivoSaldos;
    archivoSaldos.open("saldos.bin", ios::binary);

    // Leer cantidad de colaboradores del archivo de saldos
    int n;
    archivoSaldos.read((char*)&n, sizeof(n));

    // Crear n colaboradores a partir del struct definido anteriormente
    SaldoColaborador colaborador[n];
    archivoSaldos.read((char*) &colaborador, sizeof(colaborador));
    archivoSaldos.close();

    int valorSaldoActual;
    string nombreServicio;
    bool continuarCiclo = true;

    for (int i = 0; i <= n - 1; i++) {

        // Si el RUT del colaborador es el actual
        if (strcmp(rut, colaborador[i].rut) == 0) {

            // Cambiar el valor del saldo al correspondiente al servicio
            // Además, incluir el nombre del servicio para su uso en el archivo ASCII
            if (servicio == 0) {
                valorSaldoActual = colaborador[i].saldo_desayuno;
                nombreServicio = "DESAYUNO";
                continuarCiclo = false;

            } else if (servicio == 1) {
                valorSaldoActual = colaborador[i].saldo_almuerzo;
                nombreServicio = "ALMUERZO";
                continuarCiclo = false;

            } else if (servicio == 2) {
                valorSaldoActual = colaborador[i].saldo_once;
                nombreServicio = "ONCE";
                continuarCiclo = false;

            } else {
                valorSaldoActual = colaborador[i].saldo_cena;
                nombreServicio = "CENA";
                continuarCiclo = false;

            }
        } else {
            // En caso de que no sea, o si ya se encontró el RUT, se asigna saldo 0
            if (continuarCiclo == true) {
                valorSaldoActual = 0;
            }
        }
    }

    // Determinar si el colaborador tiene saldo disponible para el servicio
    bool saldoDisponible;

    if (valorSaldoActual >= 1) {
        // En el caso de que tenga saldo, cambiar el return como verdadero
        saldoDisponible = true;

        // Escribir el archivo ASCII de consumos con el formato correspondiente
        ofstream archivoConsumos;
        archivoConsumos.open("consumos.txt", ios::app);
        archivoConsumos << rut << " " << nombreServicio << "\n";
        archivoConsumos.close();

    } else {
        // En caso de que no tenga, devolver false y no escribir ningun archivo
        saldoDisponible = false;
    }

    return saldoDisponible;

}

int main(){

    // Ejemplo con RUT y servicio arbitrario
    if (puedeConsumir((char*)&"21234020-9", SERV_DESAYUNO, "consumos.txt") == true) {
        cout << "El colaborador tiene saldo disponible." << endl;
    } else {
        cout << "El colaborador no tiene saldo disponible. :(" << endl;
    }

    return 0;
}