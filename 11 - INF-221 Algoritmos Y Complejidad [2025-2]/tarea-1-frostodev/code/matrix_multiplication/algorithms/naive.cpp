// Naive
// Referencia: https://en.wikipedia.org/wiki/Matrix_multiplication_algorithm

#include <vector>

std::vector<std::vector<int>> naive(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B) {

    int n = A.size();
    std::vector<std::vector<int>> C(n, std::vector<int>(n, 0));

    for (int i = 0; i < n; ++i) {         // fila de A
        for (int j = 0; j < n; ++j) {     // columna de B
            for (int k = 0; k < n; ++k) { // elementos comunes
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return C;
}
