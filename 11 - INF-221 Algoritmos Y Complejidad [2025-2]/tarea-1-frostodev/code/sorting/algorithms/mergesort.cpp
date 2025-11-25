// Mergesort
// Referencia: https://es.wikipedia.org/wiki/Ordenamiento_por_mezcla

#include <vector>

// Función auxiliar para fusionar dos subarrays ordenados
void merge(std::vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;  // Tamaño del subarray izquierdo
    int n2 = right - mid;     // Tamaño del subarray derecho

    std::vector<int> L(n1), R(n2);

    // Copiar datos a subarrays temporales L[] y R[]
    for (int i = 0; i < n1; ++i)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; ++j)
        R[j] = arr[mid + 1 + j];

    // Fusionar los subarrays temporales en arr[left..right]
    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }

    // Copiar los elementos restantes de L[], si hay
    while (i < n1) {
        arr[k++] = L[i++];
    }

    // Copiar los elementos restantes de R[], si hay
    while (j < n2) {
        arr[k++] = R[j++];
    }
}

// Función recursiva de MergeSort
void mergeSortHelper(std::vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSortHelper(arr, left, mid);
        mergeSortHelper(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// Interfaz principal
std::vector<int> mergeSort(std::vector<int>& arr) {
    mergeSortHelper(arr, 0, arr.size() - 1);
    return arr;
}
