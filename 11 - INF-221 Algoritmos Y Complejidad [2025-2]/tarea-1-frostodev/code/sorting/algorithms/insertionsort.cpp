// InsertionSort
// Referencia: https://es.wikipedia.org/wiki/Ordenamiento_por_inserci%C3%B3n

#include <vector>

std::vector<int> insertionSort(std::vector<int>& arr) {
    int n = arr.size();

    for (int i = 1; i < n; ++i) {
        int key = arr[i];
        int j = i - 1;

        // Mueve los elementos que son mayores que key una posición adelante
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }

        arr[j + 1] = key;
    }

    return arr;
}