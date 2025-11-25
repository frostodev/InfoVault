/**
 * Greedy 2
 * Heurística: Elige el equipo que maximice la productividad promedio por empleado.
 * Complejidad Temporal: O(n^3)
 * Complejidad Espacial: O(n)
 */

#include <bits/stdc++.h>
using namespace std;

struct Empleado {
    long long A, B;
    string C;
};

// Función auxiliar O(n) para calcular productividad de un rango (misma que la de backtracking)
static long long productividad_equipo(const vector<Empleado>& empleados, int l, int r) {
    if (l > r) return 0;
    unordered_map<string, int> freq;
    long long suma_B_total = 0;
    unordered_map<string, long long> suma_A_menos_B;
    int max_freq = 0;
	
    for (int i = l; i <= r; i++) {
        freq[empleados[i].C]++;
        max_freq = max(max_freq, freq[empleados[i].C]);
        suma_B_total += empleados[i].B;
        suma_A_menos_B[empleados[i].C] += (empleados[i].A - empleados[i].B);
    }
	
    vector<string> lenguajes_mas_frecuentes;
    for (auto& [lang, f] : freq) {
        if (f == max_freq) lenguajes_mas_frecuentes.push_back(lang);
    }
	
    long long mejor_prod = LLONG_MAX;
    for (auto& lang : lenguajes_mas_frecuentes) {
        long long total = suma_B_total + suma_A_menos_B[lang];
        mejor_prod = min(mejor_prod, total);
    }
    return mejor_prod;
}

int calcular_productividad_total_greedy2(int n, vector<string> lineas) {
    vector<Empleado> empleados;
    empleados.reserve(n);

	// leer la entrada
    for (auto& linea : lineas) {
        stringstream ss(linea);
        long long A, B;
        string C;
        ss >> A >> B >> C;
        empleados.push_back({A, B, C});
    }

    long long productividad_total = 0;
    int start = 0;

    while (start < n) {
        double mejor_densidad = -1e18; // Inicializar con un valor muy bajo
        int mejor_end = start;

        // busqueda local
        for (int end = start; end < n; end++) {
            long long prod_actual = productividad_equipo(empleados, start, end);
            int num_empleados = end - start + 1;
            
            // Criterio greedy: Productividad / Cantidad de empleados
            double densidad_actual = static_cast<double>(prod_actual) / num_empleados;
            
            if (densidad_actual > mejor_densidad) {
                mejor_densidad = densidad_actual;
                mejor_end = end;
            }
        }
        
        // Recalculamos el valor real (entero) del mejor segmento encontrado para sumarlo al total
        productividad_total += productividad_equipo(empleados, start, mejor_end);
        start = mejor_end + 1; 
    }

    return static_cast<int>(productividad_total);
}