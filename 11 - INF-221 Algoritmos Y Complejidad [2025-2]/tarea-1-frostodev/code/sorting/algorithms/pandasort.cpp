// PandaSort
// 
// Este algoritmo divide el arreglo en bloques de tamaño √n, donde n es el tamaño del arreglo.
// 
// PASOS DEL ALGORITMO:
// 1. DIVISIÓN: Divide el arreglo en bloques de tamaño aproximadamente √n
// 2. ORDENAMIENTO INTERNO: Ordena cada bloque individualmente usando Selection Sort
// 3. ALMACENAMIENTO: Guarda cada bloque ordenado en una cola separada
// 4. FUSIÓN: Combina todos los bloques ordenados buscando el elemento más pequeño
//    al frente de cada cola en cada iteración
//
// CARACTERÍSTICAS:
// - Usa un enfoque "dividir y conquistar" con bloques
// - Selection Sort para bloques pequeños (eficiente para tamaños reducidos)
// - Merge de bloques mediante comparación de elementos frontales
// - Complejidad: O(n√n) en el peor caso
//
// VENTAJAS:
// - Simple de implementar
// - No requiere memoria adicional significativa
// - Eficiente para arreglos de tamaño moderado

#include <algorithm>
#include <vector>
#include <queue>
#include <cmath>
#include <climits>

std::vector<int> pandaSort(std::vector<int> arr) {
    int n = arr.size();
    if (n <= 1) return arr;
    
    int block_size = std::sqrt(n);
    int block_count = (n + block_size - 1) / block_size;
    std::vector<std::queue<int>> block_queue(block_count);
    
    // Ordenar cada bloque usando selection sort
    for (int i = 0; i < block_count; i++) {
        int l = i * block_size;
        int r = std::min((i + 1) * block_size - 1, n - 1);
        
        // Selection sort en el bloque [l, r]
        for (int j = l; j <= r; j++) {
            int min_idx = j;
            for (int k = j + 1; k <= r; k++) {
                if (arr[k] < arr[min_idx]) {
                    min_idx = k;
                }
            }
            if (min_idx != j) {
                std::swap(arr[j], arr[min_idx]);
            }
        }
        
        // Llenar la cola con elementos ordenados del bloque
        for (int j = l; j <= r; j++) {
            block_queue[i].push(arr[j]);
        }
    }
    
    // Merge de todos los bloques
    for (int i = 0; i < n; i++) {
        int mn = INT_MAX, mni = -1;
        for (int j = 0; j < block_count; j++) {
            if (block_queue[j].empty()) continue;
            int cur = block_queue[j].front();
            if (cur < mn) {
                mni = j;
                mn = cur;
            }
        }
        arr[i] = mn;
        block_queue[mni].pop();
    }
    
    return arr;
}
