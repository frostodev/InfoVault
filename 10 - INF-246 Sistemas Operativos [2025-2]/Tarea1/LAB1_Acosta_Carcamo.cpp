#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>
#include <map>

using namespace std;
namespace fs = filesystem;

/*
    Esta funcion recibe una ruta de archivo y posteriormente revisa linea por linea el archivo hasta encontrar
    las pistas que lo categorizan dentro de 5 tipos. 
*/
string categorizar_archivo(const fs::path& path) {
    ifstream file(path);
    string line;

    // revisar el archivo linea por linea y ver a que tipo corresponde
    while (getline(file, line)) {
        if (line.find("extension: .sys") != string::npos) {
            return "Drivers";
        } 
        else if (line.find("extension: .exe") != string::npos) {
            return "Programs";
        }
        else if (line.find("extension: .dll") != string::npos) {
            return "Dynamic Link-Library";
        }
        else if (line.find("tipo: log") != string::npos || line.find("distancia:") != string::npos || line.find("coordenadas:") != string::npos)  {
            return "Pistas";
        }
    }
    return "Basura";
}

/*
    funcion que, mediante el nombre de una categoria y un contador asociado a la misma, se encarga de crear
    un nombre para asignar a un archivo. ejemplo: pista1.txt
*/
string crear_nombre(const string& categoria, int contador) {
    if (categoria == "Pistas") {
        return "pista" + to_string(contador) + ".txt";
    } else if (categoria == "Drivers") {
        return "driver" + to_string(contador) + ".txt";
    } else if (categoria == "Programs") {
        return "programa" + to_string(contador) + ".txt";
    } else if (categoria == "Dynamic Link-Library") {
        return "dll" + to_string(contador) + ".txt";
    } else {
        return "basura" + to_string(contador) + ".txt";
    }
}

int main() {

    // crear los directorios de salida
    fs::create_directory("Output");
    fs::create_directories("Output/Pistas");
    fs::create_directories("Output/Basura");
    fs::create_directories("Output/System");
    fs::create_directories("Output/System/Drivers");
    fs::create_directories("Output/System/Programs");
    fs::create_directories("Output/System/Dynamic Link-Library");

    // inicializacion de los contadores para los nombres de los archivos.
    map<string, int> contador;
    contador["Pistas"] = 1;
    contador["Drivers"] = 1;
    contador["Programs"] = 1;
    contador["Dynamic Link-Library"] = 1;
    contador["Basura"] = 1;

    // revisar cada archivo en la entrada
    for (auto& archivo : fs::recursive_directory_iterator("filesystem_root")) { 
        if (fs::is_regular_file(archivo.path())) {

            // categorizar el archivo según su tipo
            string categoria = categorizar_archivo(archivo.path());
            fs::path destino;

            // indicar el destino del archivo
            if (categoria == "Pistas") {
                destino = "Output/Pistas/";

            } else if (categoria == "Drivers") {
                destino = "Output/System/Drivers/";

            } else if (categoria == "Programs") {
                destino = "Output/System/Programs/";
                
            } else if (categoria == "Dynamic Link-Library") {
                destino = "Output/System/Dynamic Link-Library/";

            } else {
                destino = "Output/Basura/";
            }

            // asignar nuevo nombre a los archivos, segun su tipo/categoria.
            string nuevoNombre = crear_nombre(categoria, contador[categoria]);

            // mover el archivo a la nueva direccion/carpeta correspondiente
            fs::path destino_final = destino / nuevoNombre;
            fs::copy_file(archivo.path(), destino_final, fs::copy_options::overwrite_existing);
            contador[categoria]++;
        }
    }

    return 0;
}

