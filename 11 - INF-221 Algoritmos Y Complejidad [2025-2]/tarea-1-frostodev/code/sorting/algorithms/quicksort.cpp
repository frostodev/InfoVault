// Quicksort (Middle element).
// Referencia: https://es.wikipedia.org/wiki/Quicksort

#include <vector>
#include <iostream>

void quickSortHelper(std::vector<int>& arr, int low, int high) {
    if (low >= high) return;

    int mid = low + (high - low) / 2;
    int pivot = arr[mid];

    int i = low;
    int j = high;

    while (i <= j) {
        while (arr[i] < pivot) i++;
        while (arr[j] > pivot) j--;

        if (i <= j) {
            std::swap(arr[i], arr[j]);
            i++;
            j--;
        }
    }

    // Recursivamente ordenar las dos mitades
    if (low < j) quickSortHelper(arr, low, j);
    if (i < high) quickSortHelper(arr, i, high);
}

std::vector<int> quickSort(std::vector<int>& arr) {
    quickSortHelper(arr, 0, arr.size() - 1);
    return arr;
}
