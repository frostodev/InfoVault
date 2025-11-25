import os
import re
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Rutas
SCRIPT_DIR = os.path.dirname(__file__)
BASE_DIR = os.path.abspath(os.path.join(SCRIPT_DIR, ".."))
MEASUREMENT_DIR = os.path.join(BASE_DIR, "data", "measurements")
OUTPUT_DIR = os.path.join(BASE_DIR, "data", "outputs")
PLOT_DIR = os.path.join(BASE_DIR, "data", "plots")

# Regex para parsear el nombre del archivo: testcases_{n}_{i}.txt
FILENAME_REGEX = re.compile(r"^testcases_(\d+)_(\d+)\.txt$")

# Regex para mediciones: "Algoritmo: X segundos, Y KB de memoria"
MEASUREMENT_LINE_REGEX = re.compile(r"^(.*?): (.*?) segundos, (.*?) KB de memoria$")

# Regex para salidas: "Algoritmo: 12345"
OUTPUT_LINE_REGEX = re.compile(r"^(.*?): (\-?\d+)$")


# Parsea el directorio data/measurements/
def parse_measurements():
    data = []
    
    if not os.path.exists(MEASUREMENT_DIR):
        print(f"Error: El directorio {MEASUREMENT_DIR} no existe.")
        return pd.DataFrame()

    print(f"Leyendo mediciones de {MEASUREMENT_DIR}...")
    for filename in os.listdir(MEASUREMENT_DIR):
        file_match = FILENAME_REGEX.match(filename)
        if not file_match: continue
            
        n = int(file_match.group(1))
        i = int(file_match.group(2))
        
        filepath = os.path.join(MEASUREMENT_DIR, filename)
        with open(filepath, 'r') as f:
            for line in f:
                line_match = MEASUREMENT_LINE_REGEX.match(line.strip())
                if not line_match: continue
                
                algoritmo = line_match.group(1).strip()
                tiempo = line_match.group(2).strip()
                memoria = line_match.group(3).strip()
                
                if "(no ejecutado!)" in algoritmo: continue
                
                data.append({
                    'n': n, 'i': i, 'algoritmo': algoritmo,
                    'tiempo': float(tiempo), 'memoria': int(memoria)
                })
    return pd.DataFrame(data)

# Parsea el directorio data/outputs/
def parse_outputs():
    data = []
    
    if not os.path.exists(OUTPUT_DIR):
        print(f"Error: El directorio {OUTPUT_DIR} no existe.")
        return pd.DataFrame()

    print(f"Leyendo salidas de {OUTPUT_DIR}...")
    for filename in os.listdir(OUTPUT_DIR):
        file_match = FILENAME_REGEX.match(filename)
        if not file_match: continue
            
        n = int(file_match.group(1))
        i = int(file_match.group(2))
        
        filepath = os.path.join(OUTPUT_DIR, filename)
        with open(filepath, 'r') as f:
            for line in f:
                line_match = OUTPUT_LINE_REGEX.match(line.strip())
                if not line_match: continue
                
                algoritmo = line_match.group(1).strip()
                productividad = int(line_match.group(2))
                
                if "(no ejecutado!)" in algoritmo: continue
                
                data.append({
                    'n': n, 'i': i, 'algoritmo': algoritmo,
                    'productividad': productividad
                })
    return pd.DataFrame(data)

# Toma el dataframe completo y genera los gráficos
def generate_plots(df_full):
    print("Generando gráficos...")
    
    # Agrupar por 'algoritmo' y 'n', y calcular la media
    df_avg = df_full.groupby(['algoritmo', 'n']).mean(numeric_only=True).reset_index()
    
    # Separar Backtracking para los gráficos de poly
    df_poly = df_avg[df_avg['algoritmo'] != 'Backtracking']

    os.makedirs(PLOT_DIR, exist_ok=True)
    sns.set_theme(style="whitegrid")

    # Gráfico 1: Tiempo vs. n (Todos, Log/Log)
    plt.figure(figsize=(10, 6))
    sns.lineplot(data=df_avg, x='n', y='tiempo', hue='algoritmo', marker='o')
    plt.xscale('log'); plt.yscale('log')
    plt.title('Tiempo de Ejecución vs. n (Log-Log)')
    plt.xlabel('Cantidad de empleados (n) - Escala Log')
    plt.ylabel('Tiempo (segundos) - Escala Log')
    plt.legend(title='Algoritmo')
    plt.savefig(os.path.join(PLOT_DIR, "tiempo_vs_n_todos_log.png"))
    plt.clf(); print(f"-> Gráfico 'tiempo_vs_n_todos_log.png' guardado.")

    # Gráfico 2: Tiempo vs. n (DP y Greedy, Log/Log)
    plt.figure(figsize=(10, 6))
    sns.lineplot(data=df_poly, x='n', y='tiempo', hue='algoritmo', marker='o')
    plt.xscale('log'); plt.yscale('log')
    plt.title('Tiempo de Ejecución vs. n - (DP y Greedy, Log-Log)')
    plt.xlabel('Cantidad de empleados (n) - Escala Log')
    plt.ylabel('Tiempo (segundos) - Escala Log')
    plt.legend(title='Algoritmo')
    plt.savefig(os.path.join(PLOT_DIR, "tiempo_vs_n_poly_log.png"))
    plt.clf(); print(f"-> Gráfico 'tiempo_vs_n_poly_log.png' guardado.")

    # Gráfico 3: Memoria vs. N (Todos, Log/Log)
    plt.figure(figsize=(10, 6))
    sns.lineplot(data=df_avg, x='n', y='memoria', hue='algoritmo', marker='o')
    plt.xscale('log'); plt.yscale('log')
    plt.title('Uso de Memoria vs. n (Log-Log)')
    plt.xlabel('Cantidad de empleados (n) - Escala Log')
    plt.ylabel('Memoria (KB) - Escala Log')
    plt.legend(title='Algoritmo')
    plt.savefig(os.path.join(PLOT_DIR, "memoria_vs_n_todos_log.png"))
    plt.clf(); print(f"-> Gráfico 'memoria_vs_n_todos_log.png' guardado.")

    # Gráfico 4: Memoria vs. N (DP y Greedy, Log/Log)
    plt.figure(figsize=(10, 6))
    sns.lineplot(data=df_poly, x='n', y='memoria', hue='algoritmo', marker='o')
    plt.xscale('log'); plt.yscale('log')
    plt.title('Uso de Memoria vs. n - (DP y Greedy, Log-Log)')
    plt.xlabel('Cantidad de empleados (n) - Escala Log')
    plt.ylabel('Memoria (KB) - Escala Log')
    plt.legend(title='Algoritmo')
    plt.savefig(os.path.join(PLOT_DIR, "memoria_vs_n_poly_log.png"))
    plt.clf(); print(f"-> Gráfico 'memoria_vs_n_poly_log.png' guardado.")

    # Gráfico 5: Productividad Promedio vs. n
    plt.figure(figsize=(10, 6))
    sns.lineplot(data=df_avg, x='n', y='productividad', hue='algoritmo', marker='o', errorbar=None)
    plt.xscale('log')
    plt.title('Productividad Promedio vs. n')
    plt.xlabel('Cantidad de empleados (n) - Escala Log')
    plt.ylabel('Productividad Total Promedio')
    plt.legend(title='Algoritmo')
    plt.savefig(os.path.join(PLOT_DIR, "productividad_vs_n_lineplot.png"))
    plt.clf(); print(f"-> Gráfico 'productividad_vs_n_lineplot.png' guardado.")

    # Gráfico 6: Scatter Plot (Greedy vs. Óptimo)
    print("Preparando datos para scatter plot de calidad...")
    # Obtener soluciones óptimas (DP o Backtracking)
    df_optimal = df_full[
        (df_full['algoritmo'] == 'Programación dinámica') | 
        (df_full['algoritmo'] == 'Backtracking')
    ].copy()
    # De-duplicar, preferir DP si ambos existen (para n pequeños)
    df_optimal = df_optimal.sort_values('algoritmo').drop_duplicates(subset=['n', 'i'], keep='last')
    df_optimal = df_optimal[['n', 'i', 'productividad']].rename(columns={'productividad': 'prod_optima'})

    # Obtener soluciones Greedy
    df_greedy = df_full[
        (df_full['algoritmo'] == 'Greedy (primera heurística)') | 
        (df_full['algoritmo'] == 'Greedy (segunda heurística)')
    ].copy()

    # Unir por (n, i) para tener (Greedy_Prod, Optimal_Prod) en la misma fila
    df_quality = pd.merge(df_greedy, df_optimal, on=['n', 'i'])
    
    if not df_quality.empty:
        plt.figure(figsize=(10, 7))
        sns.scatterplot(data=df_quality, x='prod_optima', y='productividad', hue='algoritmo', alpha=0.7)
        
        # Dibujar la línea x=y (la "solución perfecta")
        # Obtener los límites para que la línea cubra todo el gráfico
        lims = [
            min(df_quality['prod_optima'].min(), df_quality['productividad'].min()),
            max(df_quality['prod_optima'].max(), df_quality['productividad'].max()),
        ]
        if pd.isna(lims[0]): lims[0] = 0
        if pd.isna(lims[1]): lims[1] = 100
        
        plt.plot(lims, lims, 'k--', alpha=0.75, zorder=0, label='Línea Óptima (x=y)')
        
        plt.title('Calidad de Solución (Greedy vs. Óptima)')
        plt.xlabel('Productividad Óptima (Obtenida por DP/Backtracking)')
        plt.ylabel('Productividad Obtenida (Greedy)')
        plt.legend(title="Algoritmo", loc="best")
        plt.savefig(os.path.join(PLOT_DIR, "calidad_scatter.png"))
        plt.clf()
        print(f"-> Gráfico 'calidad_scatter.png' guardado.")
    else:
        print("No se pudo generar 'calidad_scatter.png' (datos insuficientes).")

    print("\nGráficos generados con éxito")


def main():
    print("Iniciando generador de gráficos")
    # Parsear ambos directorios
    df_measure = parse_measurements()
    df_outputs = parse_outputs()

    # Unir (merge) los DataFrames
    print("Uniendo datasets...")
    df_full = pd.merge(df_measure, df_outputs, on=['n', 'i', 'algoritmo'], how="inner")
    
    if df_full.empty:
        print("\nError: Los datos no se pudieron unir.")
        print("Asegúrate que los algoritmos coincidan en los archivos de salida y medición.")
        return

    # Generar gráficos
    generate_plots(df_full)

if __name__ == "__main__":
    main()