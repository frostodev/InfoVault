import os
import re
import matplotlib.pyplot as plt
import numpy as np
from pathlib import Path

def parse_measurement_file(file_path):
    """
    Parsea un archivo de mediciones y extrae los datos de tiempo y memoria
    """
    data = {'naive': {}, 'strassen': {}}
    
    try:
        with open(file_path, 'r') as f:
            content = f.read()
            
        # Buscar datos para naive
        naive_match = re.search(r'naive:\s+([\d.]+)\s+segundos,\s+(\d+)\s+KB', content)
        if naive_match:
            data['naive']['time'] = float(naive_match.group(1))
            data['naive']['memory'] = int(naive_match.group(2))
            
        # Buscar datos para strassen
        strassen_match = re.search(r'strassen:\s+([\d.]+)\s+segundos,\s+(\d+)\s+KB', content)
        if strassen_match:
            data['strassen']['time'] = float(strassen_match.group(1))
            data['strassen']['memory'] = int(strassen_match.group(2))
            
    except Exception as e:
        print(f"Error al procesar {file_path}: {e}")
        
    return data

def extract_file_info(filename):
    """
    Extrae información del nombre del archivo según el formato: {n}_{t}_{d}_{m}_measurement.txt
    """
    pattern = r'(\d+)_([a-zA-Z]+)_([a-zA-Z0-9]+)_([a-c])_measurement\.txt'
    match = re.match(pattern, filename)
    
    if match:
        n = int(match.group(1))  # tamaño de la matriz (n x n)
        matrix_type = match.group(2)  # tipo de matriz
        domain = match.group(3)  # dominio
        sample = match.group(4)  # muestra
        
        return n, matrix_type, domain, sample
    return None, None, None, None

def collect_all_data(measurements_dir):
    """
    Recopila todos los datos de mediciones
    """
    data = {
        'dispersa': {'naive': {'n': [], 'time': [], 'memory': []}, 
                    'strassen': {'n': [], 'time': [], 'memory': []}},
        'diagonal': {'naive': {'n': [], 'time': [], 'memory': []}, 
                    'strassen': {'n': [], 'time': [], 'memory': []}},
        'densa': {'naive': {'n': [], 'time': [], 'memory': []}, 
                 'strassen': {'n': [], 'time': [], 'memory': []}}
    }
    
    measurements_path = Path(measurements_dir)
    
    if not measurements_path.exists():
        print(f"Directorio {measurements_dir} no encontrado")
        return data
    
    for file_path in measurements_path.glob('*_measurement.txt'):
        file_info = extract_file_info(file_path.name)
        if file_info[0] is None:
            continue
            
        n, matrix_type, domain, sample = file_info
        
        measurement_data = parse_measurement_file(file_path)
        
        # Agregar datos según el tipo de matriz
        if matrix_type in data:
            for algo in ['naive', 'strassen']:
                if algo in measurement_data and 'time' in measurement_data[algo]:
                    data[matrix_type][algo]['n'].append(n)
                    data[matrix_type][algo]['time'].append(measurement_data[algo]['time'])
                    data[matrix_type][algo]['memory'].append(measurement_data[algo]['memory'])
    
    return data

def plot_complexity_curves(ax, n_values, algo_type):
    """
    Dibuja las curvas de complejidad teórica
    """
    n_array = np.array(n_values)
    
    if algo_type == 'naive':
        # O(n^3)
        theoretical = (n_array ** 3) * 1e-9  # Escalado para mejor visualización
        ax.plot(n_values, theoretical, '--', label='O(n³) teórico', alpha=0.7)
    else:
        # O(n^log2(7)) ≈ O(n^2.807)
        theoretical = (n_array ** 2.807) * 1e-9  # Escalado para mejor visualización
        ax.plot(n_values, theoretical, '--', label='O(n²·⁸⁰⁷) teórico', alpha=0.7)

def create_plots(data, output_dir):
    """
    Crea los gráficos para cada tipo de matriz
    """
    # Crear directorio de salida si no existe
    os.makedirs(output_dir, exist_ok=True)
    
    matrix_types = ['dispersa', 'diagonal', 'densa']
    colors = {'naive': 'blue', 'strassen': 'red'}
    
    # Gráficos de tiempo
    fig_time, axes_time = plt.subplots(1, 3, figsize=(18, 6))
    fig_time.suptitle('Tiempo de ejecución vs Tamaño de matriz', fontsize=16, fontweight='bold')
    
    # Gráficos de memoria
    fig_memory, axes_memory = plt.subplots(1, 3, figsize=(18, 6))
    fig_memory.suptitle('Uso de memoria vs Tamaño de matriz', fontsize=16, fontweight='bold')
    
    for i, matrix_type in enumerate(matrix_types):
        # Gráfico de tiempo
        ax_time = axes_time[i]
        ax_memory = axes_memory[i]
        
        for algo in ['naive', 'strassen']:
            if data[matrix_type][algo]['n']:
                # Ordenar por tamaño n
                sorted_data = sorted(zip(data[matrix_type][algo]['n'], 
                                       data[matrix_type][algo]['time'],
                                       data[matrix_type][algo]['memory']))
                n_sorted, time_sorted, memory_sorted = zip(*sorted_data)
                
                # Tiempo
                ax_time.plot(n_sorted, time_sorted, 'o-', color=colors[algo], 
                           label=f'{algo.capitalize()}', linewidth=2, markersize=6)
                
                # Memoria
                ax_memory.plot(n_sorted, memory_sorted, 'o-', color=colors[algo], 
                             label=f'{algo.capitalize()}', linewidth=2, markersize=6)
                
                # Dibujar curvas de complejidad teórica
                plot_complexity_curves(ax_time, n_sorted, algo)
        
        ax_time.set_xlabel('Tamaño de matriz (n)')
        ax_time.set_ylabel('Tiempo (segundos)')
        ax_time.set_title(f'Matriz {matrix_type.capitalize()}')
        ax_time.grid(True, alpha=0.3)
        ax_time.legend()
        ax_time.set_xscale('log')
        ax_time.set_yscale('log')
        
        ax_memory.set_xlabel('Tamaño de matriz (n)')
        ax_memory.set_ylabel('Memoria (KB)')
        ax_memory.set_title(f'Matriz {matrix_type.capitalize()}')
        ax_memory.grid(True, alpha=0.3)
        ax_memory.legend()
        ax_memory.set_xscale('log')
        ax_memory.set_yscale('log')
    
    # Ajustar layout y guardar
    fig_time.tight_layout()
    fig_memory.tight_layout()
    
    fig_time.savefig(os.path.join(output_dir, 'tiempo_ejecucion.png'), dpi=300, bbox_inches='tight')
    fig_memory.savefig(os.path.join(output_dir, 'uso_memoria.png'), dpi=300, bbox_inches='tight')
    
    plt.close(fig_time)
    plt.close(fig_memory)

def main():
    # Directorios
    measurements_dir = "../data/measurements"
    plots_dir = "../data/plots"
    
    print("Recopilando datos de mediciones...")
    data = collect_all_data(measurements_dir)
    
    print("Creando gráficos...")
    create_plots(data, plots_dir)
    
    print("Gráficos guardados en:", plots_dir)
    
    # Mostrar resumen de datos recopilados
    print("\nResumen de datos recopilados:")
    for matrix_type in ['dispersa', 'diagonal', 'densa']:
        print(f"\n{matrix_type.upper()}:")
        for algo in ['naive', 'strassen']:
            n_count = len(data[matrix_type][algo]['n'])
            if n_count > 0:
                print(f"  {algo}: {n_count} mediciones")
                print(f"    Tamaños: {sorted(data[matrix_type][algo]['n'])}")

if __name__ == "__main__":
    main()