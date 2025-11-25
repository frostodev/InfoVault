/**
 * Progra dinámica
 * Recurrencia: DP[i] = max(DP[j] + productividad(j, i-1)) para 0 <= j < i
 * Complejidad Temporal: O(n^3)
 * Complejidad Espacial: O(n) (array lineal 'dp')
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

int calcular_productividad_total_dp(int n, vector<string> lineas) {
    vector<Empleado> empleados;
    empleados.reserve(n);

	// leer entrada
    for (auto& linea : lineas) {
        stringstream ss(linea);
        long long A, B;
        string C;
        ss >> A >> B >> C;
        empleados.push_back({A, B, C});
    }

    // Definición de estado
    // dp[i] almacena la productividad máxima posible considerando SOLO los primeros i empleados
    // El objetivo final es encontrar dp[n]
    vector<long long> dp(n + 1);
    
    // El caso base sería que 0 empleados generan 0 productivdad
    dp[0] = 0;

    // Llenado de la tabla Bottom-Up
    for (int i = 1; i <= n; i++) {
        dp[i] = LLONG_MIN; // Inicializar con un valor enano
        
        // Transición:
        // Para calcular dp[i], probamos todos los posibles "últimos equipos" formados por el segmento [j, i-1]
        // dp[i] será el maximo de (la solución óptima hasta j + lo que aporta el nuevo equipo [j, i-1])
        for (int j = 0; j < i; j++) {
            long long prod_ultimo_segmento = productividad_equipo(empleados, j, i - 1);
            
            if (dp[j] != LLONG_MIN) { // check de seguridad (aunque dp[0] en teoría siempre es válido)
                dp[i] = max(dp[i], dp[j] + prod_ultimo_segmento);
            }
        }
    }

    return static_cast<int>(dp[n]);
}