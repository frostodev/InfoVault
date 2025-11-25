// std::sort
// Referencia: https://en.cppreference.com/w/cpp/algorithm/sort.html

#include <algorithm>
#include <vector>

std::vector<int> stlSort(std::vector<int>& arr) {
    std::sort(arr.begin(), arr.end());  // std::sort de la STL
    return arr;
}
