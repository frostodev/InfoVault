#include <iostream>
#include <chrono>
#include <sys/resource.h>
#include <unistd.h>
#include <vector>
#include <fstream>
#include <filesystem>
#include <unordered_map>
#include <regex>

namespace fs = std::filesystem;

std::vector<std::vector<int>> naive(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B);
std::vector<std::vector<int>> strassen(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B);

long obtener_uso_memoria() {
    std::ifstream status_file("/proc/self/status");
    if (!status_file.is_open()) return -1;

    std::string line;
    while (std::getline(status_file, line)) {
        if (line.rfind("VmRSS:", 0) == 0) {
            std::istringstream iss(line);
            std::string key, unit;
            long value_kb;
            iss >> key >> value_kb >> unit;
            return value_kb;
        }
    }

    return -1;
}

void guardar_resultados(const std::string& nombre_archivo, const std::string& algoritmo, double duracion, long memoria_kb) {
    std::string ruta = "data/measurements/" + nombre_archivo;
    std::ofstream out(ruta, std::ios::app);
    out << algoritmo << ": " << duracion << " segundos, " << memoria_kb << " KB de memoria\n";
}

// Leer matriz desde archivo
std::vector<std::vector<int>> leer_matriz(const std::string& ruta) {
    std::ifstream archivo(ruta);
    std::vector<std::vector<int>> matriz;
    std::string linea;

    while (std::getline(archivo, linea)) {
        std::istringstream iss(linea);
        std::vector<int> fila;
        int num;
        while (iss >> num) {
            fila.push_back(num);
        }
        matriz.push_back(fila);
    }

    return matriz;
}

// Guardar matriz en archivo
void guardar_matriz(const std::string& ruta, const std::vector<std::vector<int>>& matriz) {
    std::ofstream archivo(ruta);
    for (const auto& fila : matriz) {
        for (size_t j = 0; j < fila.size(); ++j) {
            archivo << fila[j];
            if (j < fila.size() - 1)
                archivo << " ";
        }
        archivo << "\n";
    }
}

void multiplicar_matrices(char algoritmo, auto& A, auto& B, const std::string& base_filename, const std::string& muestra) {
    
    std::vector<std::vector<int>> matriz_resultante;
    std::string nombre_algoritmo;
    std::string ruta_salida;
    
    // comenzar a medir memoria y tiempo
    long memoria_inicial = obtener_uso_memoria();
    auto start = std::chrono::high_resolution_clock::now();
    
    switch (algoritmo) {
        case 'N':
            std::cout << "Multiplicando usando algoritmo Naive...." << "\n"; 
            nombre_algoritmo = "naive";
            matriz_resultante = naive(A, B);
            break;
            
        case 'S':
            std::cout << "Multiplicando usando algoritmo Strassen...." << "\n"; 
            nombre_algoritmo = "strassen";
            matriz_resultante = strassen(A, B);
            break;
        
        default:
            return;
    }

    // dejar de medir el tiempo
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracion = end - start;
    
    // medir memoria después de ejecutar y calcular diferencia
    long memoria_final = obtener_uso_memoria();
    long memoria_utilizada = memoria_final - memoria_inicial;

    // Construir nombre de archivo de salida según formato: {n}_{t}_{d}_{m}_out.txt
    std::string nombre_salida = base_filename + "_out.txt";
    ruta_salida = "data/matrix_output/" + nombre_salida;
    guardar_matriz(ruta_salida, matriz_resultante);
    
    // Guardar mediciones en archivo separado para cada muestra
    std::string nombre_measurement = base_filename + "_measurement.txt";
    guardar_resultados(nombre_measurement, nombre_algoritmo, duracion.count(), memoria_utilizada);

    std::cout << "  Tiempo: " << duracion.count() << "s, Memoria: " << memoria_utilizada << " KB" << "\n";
    std::cout << "  Resultado guardado en: " << ruta_salida << "\n";
}

int main() {
    std::unordered_map<std::string, std::vector<std::string>> pares_por_base;

    // Emparejar archivos _1.txt y _2.txt
    for (const auto& archivo : fs::recursive_directory_iterator("data/matrix_input")) {
        std::string nombre = archivo.path().filename().string();
        
        // Buscar archivos que terminen con _1.txt o _2.txt
        if (nombre.find("_1.txt") != std::string::npos || nombre.find("_2.txt") != std::string::npos) {
            // Extraer la base del nombre (sin _1.txt o _2.txt)
            size_t pos = nombre.find_last_of('_');
            std::string base_filename = nombre.substr(0, pos);
            
            // Verificar que el formato sea correcto: {n}_{t}_{d}_{m}
            std::regex pattern(R"((\d+)_([a-zA-Z]+)_([a-zA-Z0-9]+)_([a-c]))");
            std::smatch matches;
            
            if (std::regex_match(base_filename, matches, pattern)) {
                pares_por_base[base_filename].push_back(archivo.path().string());
            }
        }
    }

    // Operar cada par de matrices
    int i = 0;
    for (const auto& [base_filename, rutas] : pares_por_base) {
        if (rutas.size() < 2) continue; // Necesitamos al menos dos archivos (matriz1 y matriz2)
        
        std::string ruta1, ruta2;
        
        // Encontrar qué ruta es _1 y qué ruta es _2
        for (const auto& ruta : rutas) {
            if (ruta.find("_1.txt") != std::string::npos) {
                ruta1 = ruta;
            } else if (ruta.find("_2.txt") != std::string::npos) {
                ruta2 = ruta;
            }
        }
        
        if (ruta1.empty() || ruta2.empty()) continue;

        // Extraer la muestra del nombre base
        size_t last_underscore = base_filename.find_last_of('_');
        std::string muestra = base_filename.substr(last_underscore + 1);

        std::cout << "\nProcesando: " << base_filename << " (par de matrices N° " << i + 1 << ")" << "\n";
        
        // Leer y multiplicar las matrices
        auto A = leer_matriz(ruta1);
        auto B = leer_matriz(ruta2);

        multiplicar_matrices('N', A, B, base_filename, muestra);
        multiplicar_matrices('S', A, B, base_filename, muestra);
        
        ++i;
    }

    return 0;
}