
#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <filesystem>
#include <sys/resource.h>
#include <unistd.h>
#include <sstream>

namespace fs = std::filesystem;

// declaraciones directas de las funciones (ya que no nos dejan usar .h)
std::vector<int> insertionSort(std::vector<int>& arr);
std::vector<int> mergeSort(std::vector<int>& arr);
std::vector<int> pandaSort(std::vector<int> arr);
std::vector<int> quickSort(std::vector<int>& arr);
std::vector<int> stlSort(std::vector<int>& arr);

long obtener_uso_memoria() {
    std::ifstream statm("/proc/self/statm");
    long size, resident, share, text, lib, data, dt;
    statm >> size >> resident >> share >> text >> lib >> data >> dt;
    statm.close();
    
    long page_size = sysconf(_SC_PAGESIZE); // tamaño de página en bytes
    return (resident * page_size); // convertir a KB
}

std::vector<int> leer_arreglo(const std::string& ruta) {
    std::ifstream archivo(ruta);
    std::vector<int> arr;
    int num;
    while (archivo >> num) {
        arr.push_back(num);
    }
    return arr;
}

void guardar_arreglo_ordenado(const std::vector<int>& arr, const std::string& nombre_archivo_salida) {
    std::ofstream out("data/array_output/" + nombre_archivo_salida);
    for (int num : arr) {
        out << num << " ";
    }
}

void guardar_resultados(const std::string& nombre_archivo, const std::string& algoritmo, double duracion, long memoria_kb) {
    std::ofstream out("data/measurements/" + nombre_archivo, std::ios::app);
    out << algoritmo << ": " << duracion << " segundos, " << memoria_kb << " KB de memoria\n";
}

void ordenar_medir_arreglo(char algoritmo, const std::vector<int>& arr_original, const std::string& nombre_base) {
    std::vector<int> arr_copia = arr_original;
    std::vector<int> arr_ordenado;
    std::string nombre_algoritmo;
	bool truncado = false;
	
	// en el caso de Insertion Sort, truncar la cantidad de elementos (o si no tomará semanas en completar)
    if (algoritmo == 'I' && arr_original.size() > 500000) {
        arr_copia = std::vector<int>(arr_original.begin(), arr_original.begin() + 500000);
        truncado = true;
    } else {
        arr_copia = arr_original;
    }

	// comenzar a medir memoria y tiempo
	long memoria_inicial = obtener_uso_memoria();
    auto start = std::chrono::high_resolution_clock::now();

    switch (algoritmo) {
        case 'I':
            std::cout << "Algoritmo actual: Insertion Sort";
            if (truncado) std::cout << " (truncado de " << arr_original.size() << "elementos a 500 000 elementos)";
            std::cout << "\n";
            nombre_algoritmo = "insertionsort";
            arr_ordenado = insertionSort(arr_copia);
            break;

        case 'M':
            std::cout << "Algoritmo actual: Merge Sort" << "\n";
            nombre_algoritmo = "mergesort";
            arr_ordenado = mergeSort(arr_copia);
            break;

        case 'P':
            std::cout << "Algoritmo actual: Panda Sort" << "\n";
            nombre_algoritmo = "pandasort";
            arr_ordenado = pandaSort(arr_copia);
            break;

        case 'Q':
            std::cout << "Algoritmo actual: Quick Sort" << "\n";
            nombre_algoritmo = "quicksort";
            arr_ordenado = quickSort(arr_copia);
            break;

        case 'S':
            std::cout << "Algoritmo actual: Sort estándar (STL)" << "\n";
            nombre_algoritmo = "stlsort";
            arr_ordenado = stlSort(arr_copia);
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

    // generar nombres de salida
    std::string nombre_archivo_salida = nombre_base + "_" + nombre_algoritmo + ".txt";
    std::string nombre_measurement = nombre_base + "_measurement.txt";

	// guardar los arreglos ordenados y los resultados
    guardar_arreglo_ordenado(arr_ordenado, nombre_archivo_salida);
    guardar_resultados(nombre_measurement, nombre_algoritmo, duracion.count(), memoria_utilizada);
	
	std::cout << "  Tiempo: " << duracion.count() << "s, Memoria: " << memoria_utilizada << " KB";
	if (truncado) std::cout << " (cantidad de elementos truncada)";
	std::cout << "\n";
}

int main() {
	
	// leer cada arreglo de cada archivo
    for (const auto& archivo : fs::recursive_directory_iterator("data/array_input")) {
        std::string ruta = archivo.path().string();
        std::string nombre_archivo = archivo.path().filename().string();
        std::string nombre_base = nombre_archivo.substr(0, nombre_archivo.find(".txt"));

        std::cout << "\nProcesando archivo: " << nombre_archivo << "\n";
        std::vector<int> arr = leer_arreglo(ruta);

        // aplicar todos los algoritmos
        ordenar_medir_arreglo('Q', arr, nombre_base);
        ordenar_medir_arreglo('M', arr, nombre_base);
        ordenar_medir_arreglo('P', arr, nombre_base);
        ordenar_medir_arreglo('I', arr, nombre_base);
        ordenar_medir_arreglo('S', arr, nombre_base);
    }

    return 0;
}
