/**
 * Backtracking
 * Complejidad Temporal: O(n * 2^n) - Intratable para n > 30
 * Complejidad Espacial: O(n) debido a la pila de recursión
 */

#include <bits/stdc++.h>
using namespace std;

struct Empleado {
    long long A, B;
    string C;
};

// Función auxiliar para calcular la productividad de un segmento [l, r].
// Complejidad: O(r - l) -> Peor caso O(n)
static long long productividad_equipo(const vector<Empleado>& empleados, int l, int r) {
    if (l > r) return 0;

    unordered_map<string, int> freq;
    long long suma_B_total = 0;
    // Optimización, guardamos (A - B) para sumar solo la diferencia nomás después
    unordered_map<string, long long> suma_A_menos_B;

    int max_freq = 0;

    // Pasada única para calcular frecuencias y sumas base
    for (int i = l; i <= r; i++) {
        freq[empleados[i].C]++;
        max_freq = max(max_freq, freq[empleados[i].C]);
        
        suma_B_total += empleados[i].B; // Asumimos base B para todos
        suma_A_menos_B[empleados[i].C] += (empleados[i].A - empleados[i].B); // Delta si es favorito
    }

    // Identificar candidatos a lenguaje del equipo (los mas frecuentes)
    vector<string> lenguajes_mas_frecuentes;
    for (auto& [lang, f] : freq) {
        if (f == max_freq) {
            lenguajes_mas_frecuentes.push_back(lang);
        }
    }

    long long mejor_prod = LLONG_MAX;

    // Para el desempate, elegir el lenguaje que genere la menor productividad posible
    for (auto& lang : lenguajes_mas_frecuentes) {
        long long total = suma_B_total + suma_A_menos_B[lang];
        mejor_prod = min(mejor_prod, total);
    }

    return mejor_prod;
}

// Función recursiva de Backtracking
// Explora todos los posibles cortes para el equipo actual
long long resolver_backtracking(const vector<Empleado>& empleados, int start, int n) {
    // Caso Base: si ya cubrimos a todos los empleados (start == n), la productividad extra es 0
    if (start == n) return 0; 

    long long mejor = LLONG_MIN;

    // Probar todos los posibles puntos de corte end desde start hasta el final
    // Esto genera el árbol de recursión de orden O(2^n)
    for (int end = start; end < n; end++) {
        long long prod_segmento = productividad_equipo(empleados, start, end);
        
        // recursión: prod del equipo actual + mejor prod del resto de la fila
        long long total = prod_segmento + resolver_backtracking(empleados, end + 1, n);
        
        // Maximizar la productividad total
        mejor = max(mejor, total);
    }

    return mejor;
}

int calcular_productividad_total_backtracking(int n, vector<string> lineas) {
    vector<Empleado> empleados;
    empleados.reserve(n);

    // Leer la entrada
    for (auto& linea : lineas) {
        stringstream ss(linea);
        long long A, B;
        string C;
        ss >> A >> B >> C;
        empleados.push_back({A, B, C});
    }

    // Iniciar la recursión desde el empleado 0
    long long resultado = resolver_backtracking(empleados, 0, n);
    return static_cast<int>(resultado);
}