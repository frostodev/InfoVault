// Strassen "puro"
// Referencia: https://en.cppreference.com/w/cpp/algorithm/sort.html

#include <iostream>
#include <sstream>
#include <vector>
#include <string>

// Suma de matrices
std::vector<std::vector<int>> sumar(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B) {
    int n = A.size();
    std::vector<std::vector<int>> C(n, std::vector<int>(n));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            C[i][j] = A[i][j] + B[i][j];
    return C;
}

// Resta de matrices
std::vector<std::vector<int>> restar(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B) {
    int n = A.size();
    std::vector<std::vector<int>> C(n, std::vector<int>(n));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            C[i][j] = A[i][j] - B[i][j];
    return C;
}

// Algoritmo de Strassen
std::vector<std::vector<int>> strassen(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B) {
    int n = A.size();
    if (n == 1) {
        return {{A[0][0] * B[0][0]}};
    }

    int k = n / 2;
    std::vector<std::vector<int>> A11(k, std::vector<int>(k)), A12(k, std::vector<int>(k)),
                                   A21(k, std::vector<int>(k)), A22(k, std::vector<int>(k)),
                                   B11(k, std::vector<int>(k)), B12(k, std::vector<int>(k)),
                                   B21(k, std::vector<int>(k)), B22(k, std::vector<int>(k));

    for (int i = 0; i < k; ++i)
        for (int j = 0; j < k; ++j) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + k];
            A21[i][j] = A[i + k][j];
            A22[i][j] = A[i + k][j + k];

            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j + k];
            B21[i][j] = B[i + k][j];
            B22[i][j] = B[i + k][j + k];
        }

    auto M1 = strassen(sumar(A11, A22), sumar(B11, B22));
    auto M2 = strassen(sumar(A21, A22), B11);
    auto M3 = strassen(A11, restar(B12, B22));
    auto M4 = strassen(A22, restar(B21, B11));
    auto M5 = strassen(sumar(A11, A12), B22);
    auto M6 = strassen(restar(A21, A11), sumar(B11, B12));
    auto M7 = strassen(restar(A12, A22), sumar(B21, B22));

    auto C11 = sumar(restar(sumar(M1, M4), M5), M7);
    auto C12 = sumar(M3, M5);
    auto C21 = sumar(M2, M4);
    auto C22 = sumar(restar(sumar(M1, M3), M2), M6);

    std::vector<std::vector<int>> C(n, std::vector<int>(n));
    for (int i = 0; i < k; ++i)
        for (int j = 0; j < k; ++j) {
            C[i][j] = C11[i][j];
            C[i][j + k] = C12[i][j];
            C[i + k][j] = C21[i][j];
            C[i + k][j + k] = C22[i][j];
        }

    return C;
}
