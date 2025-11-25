#include <iostream>
#include <string>

using namespace std;

class Login {
private:
    struct Usuario {
        string nombre;
        string clave;
        bool activo = false;
    };

    Usuario* tabla;  // Arreglo de usuarios
    int capacidad;   // Tamaño de la tabla
    int tamano;      // Cantidad de elementos almacenados
    float factorCarga;  // Factor de carga máximo

    /*****
    * int funcionHash()
    ******
    * Aplica hashing cerrado
    ******
    * Input:
    * string clave: contraseña asociada a un nombre de usuario
    * .......
    ******
    * Returns:
    * int, retorna un índice válido dentro de la tabla de hash.
    *****/
    int funcionHash(string clave) {
        int hash = 0;
        for (int i = 0; i < clave.length(); i++) {
            hash += clave[i];  // Suma el valor ASCII de cada carácter de la clave
        }
        return hash % capacidad;  // Aplica el módulo para obtener el índice en la tabla
    }

    /*****
    * int siguientePrimo()
    ******
    * toma un numero y busca el siguiente numero primo
    ******
    * Input:
    * int num: numero natural
    * .......
    ******
    * Returns:
    * int, retorna el sguiente numero primo mas cercano
    *****/
    int siguientePrimo(int num) {
        num++;
        while (!esPrimo(num)) {
            num++;
        }
        return num;
    }

    /*****
    * bool esPrimo()
    ******
    * Evalua si el numero dado es primo
    ******
    * Input:
    * int num: numero natural
    * .......
    ******
    * Returns:
    * bool, retorna true si el numero dado es primo
    *****/
    bool esPrimo(int num) {
        if (num <= 1) {
            return false;
        }
        for (int i = 2; i * i <= num; i++) {
            if (num % i == 0) {
                return false;
            }
        }
        return true;
    }

    /*****
    *void redimensionarTabla
    ******
    * La función crea una tabla de tamaño igual al siguiente número primo. La función usa hashing cerrado y va actualizando el tamaño de la tabla según el factor de carga
    ******
    ******
    * no retorna nada
    *****/
    void redimensionarTabla() {
        int nuevoTamano = siguientePrimo(capacidad * 2);
        Usuario* nuevaTabla = new Usuario[nuevoTamano];  // Crea una nueva tabla con el tamaño siguiente primo
        for (int i = 0; i < capacidad; i++) {
            if (tabla[i].activo) {
                int indice = funcionHash(tabla[i].nombre) % nuevoTamano;
                int salto = 1;
                while (nuevaTabla[indice].activo) {
                    indice = (indice + salto) % nuevoTamano;  // Utiliza hashing cerrado con incrementos lineales
                    salto++;
                }
                nuevaTabla[indice] = tabla[i];  // Copia el usuario a la nueva tabla
            }
        }
        delete[] tabla;  // Libera la memoria del arreglo anterior
        tabla = nuevaTabla;  // Actualiza la referencia al nuevo arreglo
        capacidad = nuevoTamano;  // Actualiza el tamaño de la tabla
    }

public:
    Login() {
        capacidad = 32;
        tamano = 0;
        factorCarga = 0.7;
        tabla = new Usuario[capacidad];  // Crea un arreglo de usuarios con el tamaño inicial
    }

    ~Login() {
        delete[] tabla;  // Libera la memoria del arreglo al destruir el objeto
    }

    /*****
    *bool iniciar sesion
    ******
    * dado el nombre de usuario y la contraseña retornar true si el usuario está registrado y la contraseña coincide
    ******
    * Input:
    * string usuario : nombre de usuario
    * string clave: contraseña asociada al nombre de usuario
    ******
    * Returns:
    * bool, retorna true si el usuario existe y la contraseña coincide con este
    *****/
    bool iniciar_sesion(string usuario, string clave) {
        int indice = funcionHash(usuario);
        int intentos = 0;

        while (tabla[indice].activo && intentos < capacidad) {
            if (tabla[indice].nombre == usuario) {
                if (tabla[indice].clave == clave) {
                    cout << "Sesion iniciada con exito" << endl;
                    return true;
                } else {
                    cout << "La clave ingresada no coincide" << endl;
                    return false;
                }
            }
            indice = (indice + 1) % capacidad;  // Utiliza hashing cerrado con incrementos lineales
            intentos++;
        }

        cout << "El usuario no se encuentra registrado" << endl;
        return false;
    }

    /*****
    *bool crear_nuevo_usuario
    ******
    * si el usuario no existe y la contraseña es valida, registra un nuevo usario y retorna true
    ******
    * Input:
    * string usuario : nombre de usuario
    * string clave: contraseña asociada al nombre de usuario
    ******
    * Returns:
    * bool, retorna true si el nombre no existe todavia y la contraseña cumple con los requisitos
    *****/
    bool crear_nuevo_usuario(string usuario, string clave) {
        if (tamano >= capacidad * factorCarga) {
            redimensionarTabla();  // Redimensiona la tabla si se supera el factor de carga máximo
        }

        int indice = funcionHash(usuario);
        int salto = 1;
        int intentos = 0;

        while (tabla[indice].activo && intentos < capacidad) {
            if (tabla[indice].nombre == usuario) {
                cout << "El usuario ya se encuentra registrado" << endl;
                return false;
            }
            indice = (indice + salto) % capacidad;  // Utiliza hashing cerrado con incrementos lineales
            intentos++;
        }

        if (clave.length() < 8) {
            cout << "La clave debe tener al menos 8 caracteres" << endl;
            return false;
        }

        tabla[indice].nombre = usuario;
        tabla[indice].clave = clave;
        tabla[indice].activo = true;
        tamano++;
        cout << "Usuario registrado con exito" << endl;
        return true;
    }

    /*****
    *bool cambiar_clave
    ******
    * cambia la clave asociada al nombre de usuario dado, solo si la nueva contraseña cumple con los requisitos
    ******
    * Input:
    * string usuario : nombre del usuario
    * string nueva_clave: clave por la cual se quiere cambiar la clave actualmente asiciada al usuario
    ******
    * Returns:
    * bool, retorna true si el usuario existe y si la nueva contraseña es valida
    *****/
    bool cambiar_clave(string usuario, string nueva_clave) {
        int indice = funcionHash(usuario);
        int salto = 1;
        int intentos = 0;

        while (tabla[indice].activo && intentos < capacidad) {
            if (tabla[indice].nombre == usuario) {
                if (nueva_clave.length() < 8) {
                    cout << "La clave debe tener al menos 8 caracteres" << endl;
                    return false;
                }
                tabla[indice].clave = nueva_clave;
                cout << "Clave actualizada con exito" << endl;
                return true;
            }
            indice = (indice + salto) % capacidad;  // Utiliza hashing cerrado con incrementos lineales
            intentos++;
        }

        cout << "El usuario no se encuentra registrado" << endl;
        return false;
    }
};

int main() {
    Login login;
    string operacion;

    // Leer la operación hasta que sea "FINALIZAR"
    while (operacion != "FINALIZAR") {
        cin >> operacion;

        // Operación Registrar
        if (operacion == "REGISTRAR") {
            string usuario, clave;

            // Leer el usuario y la clave
            cin >> usuario >> clave;

            // Llamar al método para crear un nuevo usuario
            login.crear_nuevo_usuario(usuario, clave);
          
        // Operación Iniciar Sesión
        } else if (operacion == "INICIAR_SESION") {
            string usuario, clave;

            // Leer el usuario y la clave
            cin >> usuario >> clave;

            // Llamar al método para iniciar sesión
            login.iniciar_sesion(usuario, clave);

        // Operación Actualizar
        } else if (operacion == "ACTUALIZAR") {
            string usuario, clave;

            // Leer el usuario y la clave
            cin >> usuario >> clave;

            // Llamar al método para cambiar la clave del usuario
            login.cambiar_clave(usuario, clave);
        }
    }

    return 0;
}