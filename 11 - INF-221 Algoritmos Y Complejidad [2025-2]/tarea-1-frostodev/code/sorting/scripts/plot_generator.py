import matplotlib.pyplot as plt
import numpy as np
import os
import re
import glob
from collections import defaultdict
from scipy.optimize import curve_fit

def parse_measurement_files():
    """Parse todos los archivos de medición y organiza los datos"""
    measurements_path = "../data/measurements/"
    files = glob.glob(os.path.join(measurements_path, "*_measurement.txt"))
    
    # Estructura: datos[tipo][algoritmo][n] = {'tiempo': [], 'memoria': []}
    datos = defaultdict(lambda: defaultdict(lambda: defaultdict(lambda: {'tiempo': [], 'memoria': [], 'count': 0})))
    
    for file_path in files:
        filename = os.path.basename(file_path)
        # Extraer información del nombre del archivo
        match = re.match(r'(\d+)_([a-zA-Z]+)_([a-zA-Z0-9]+)_([a-z])_measurement\.txt', filename)
        if not match:
            continue
            
        n = int(match.group(1))
        tipo = match.group(2)  # ascendente, descendente, aleatorio
        dominio = match.group(3)
        muestra = match.group(4)
        
        # Leer el archivo
        with open(file_path, 'r') as f:
            for line in f:
                if ':' in line:
                    parts = line.split(':')
                    algoritmo = parts[0].strip()
                    valores = parts[1].split(',')
                    
                    tiempo = float(valores[0].replace('segundos', '').strip())
                    memoria = float(valores[1].replace('KB de memoria', '').strip())
                    
                    # Almacenar datos
                    datos[tipo][algoritmo][n]['tiempo'].append(tiempo)
                    datos[tipo][algoritmo][n]['memoria'].append(memoria)
                    datos[tipo][algoritmo][n]['count'] += 1
    
    return datos

def calcular_promedios(datos):
    """Calcular promedios para cada combinación"""
    promedios = defaultdict(lambda: defaultdict(lambda: {}))
    
    for tipo in datos:
        for algoritmo in datos[tipo]:
            for n in datos[tipo][algoritmo]:
                tiempo_vals = datos[tipo][algoritmo][n]['tiempo']
                memoria_vals = datos[tipo][algoritmo][n]['memoria']
                
                if tiempo_vals and memoria_vals:
                    promedios[tipo][algoritmo][n] = {
                        'tiempo_promedio': np.mean(tiempo_vals),
                        'memoria_promedio': np.mean(memoria_vals),
                        'count': len(tiempo_vals)
                    }
    
    return promedios

def fit_complexity_curve(ns, tiempos, complexity_func):
    """Ajustar una curva de complejidad teórica a los datos"""
    try:
        # Función para ajuste de curva
        def model_func(n, a):
            return a * complexity_func(n)
        
        # Ajustar parámetro de escala
        popt, _ = curve_fit(model_func, ns, tiempos)
        a = popt[0]
        
        # Generar puntos suavizados para la curva teórica
        n_smooth = np.logspace(np.log10(min(ns)), np.log10(max(ns)), 100)
        theoretical_curve = a * complexity_func(n_smooth)
        
        return n_smooth, theoretical_curve, a
        
    except:
        # Fallback: usar normalización simple
        max_time = max(tiempos)
        max_n = max(ns)
        scale_factor = max_time / complexity_func(max_n)
        
        n_smooth = np.logspace(np.log10(min(ns)), np.log10(max(ns)), 100)
        theoretical_curve = scale_factor * complexity_func(n_smooth)
        
        return n_smooth, theoretical_curve, scale_factor

def graficar_tiempo_vs_n(promedios, tipo):
    """Graficar tiempo vs n para un tipo específico"""
    plt.figure(figsize=(12, 8))
    
    algoritmos = list(promedios[tipo].keys())
    colores = plt.cm.Set3(np.linspace(0, 1, len(algoritmos)))
    
    # Definir funciones de complejidad teórica
    complexity_funcs = {
        'insertionsort': lambda n: n**2,
        'mergesort': lambda n: n * np.log2(n + 1e-10),  # +1e-10 para evitar log(0)
        'quicksort': lambda n: n * np.log2(n + 1e-10),
        'stlsort': lambda n: n * np.log2(n + 1e-10),
        'pandasort': lambda n: n * np.sqrt(n)
    }
    
    complexity_labels = {
        'insertionsort': 'O(n²)',
        'mergesort': 'O(n log n)',
        'quicksort': 'O(n log n)',
        'stlsort': 'O(n log n)',
        'pandasort': 'O(n√n)'
    }
    
    # Graficar cada algoritmo
    for i, algoritmo in enumerate(algoritmos):
        ns = sorted(promedios[tipo][algoritmo].keys())
        tiempos = [promedios[tipo][algoritmo][n]['tiempo_promedio'] for n in ns]
        
        plt.plot(ns, tiempos, 'o-', label=algoritmo, color=colores[i], linewidth=2, markersize=8, alpha=0.8)
        
        # Agregar línea punteada de complejidad esperada con ajuste de curva
        if algoritmo in complexity_funcs:
            n_smooth, theoretical_curve, scale_factor = fit_complexity_curve(
                ns, tiempos, complexity_funcs[algoritmo]
            )
            
            plt.plot(n_smooth, theoretical_curve, '--', 
                    color=colores[i], alpha=0.7, linewidth=2,
                    label=f'{algoritmo} ({complexity_labels[algoritmo]})')
    
    plt.xscale('log')
    plt.yscale('log')
    plt.xlabel('Cantidad de elementos (n)')
    plt.ylabel('Tiempo (segundos)')
    plt.title(f'Tiempo vs Cantidad de elementos - Tipo: {tipo}')
    plt.legend()
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.savefig(f'../data/plots/tiempo_vs_n_{tipo}.png', dpi=300, bbox_inches='tight')
    plt.close()

def graficar_memoria_vs_n(promedios, tipo):
    """Graficar memoria vs n para un tipo específico"""
    plt.figure(figsize=(12, 8))
    
    algoritmos = list(promedios[tipo].keys())
    colores = plt.cm.Set3(np.linspace(0, 1, len(algoritmos)))
    
    # Definir funciones de complejidad de memoria
    memory_complexity_funcs = {
        'insertionsort': lambda n: n,
        'mergesort': lambda n: n,
        'quicksort': lambda n: n,
        'stlsort': lambda n: n,
        'pandasort': lambda n: n
    }
    
    memory_complexity_labels = {
        'insertionsort': 'O(n)',
        'mergesort': 'O(n)',
        'quicksort': 'O(n)',
        'stlsort': 'O(n)',
        'pandasort': 'O(n)'
    }
    
    for i, algoritmo in enumerate(algoritmos):
        ns = sorted(promedios[tipo][algoritmo].keys())
        memorias = [promedios[tipo][algoritmo][n]['memoria_promedio'] for n in ns]
        
        plt.plot(ns, memorias, 'o-', label=algoritmo, color=colores[i], linewidth=2, markersize=8, alpha=0.8)
        
        # Agregar línea punteada de complejidad esperada con ajuste de curva
        if algoritmo in memory_complexity_funcs:
            n_smooth, theoretical_curve, scale_factor = fit_complexity_curve(
                ns, memorias, memory_complexity_funcs[algoritmo]
            )
            
            plt.plot(n_smooth, theoretical_curve, '--', 
                    color=colores[i], alpha=0.7, linewidth=2,
                    label=f'{algoritmo} ({memory_complexity_labels[algoritmo]})')
    
    plt.xscale('log')
    plt.yscale('log')
    plt.xlabel('Cantidad de elementos (n)')
    plt.ylabel('Memoria (KB)')
    plt.title(f'Memoria vs Cantidad de elementos - Tipo: {tipo}')
    plt.legend()
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.savefig(f'../data/plots/memoria_vs_n_{tipo}.png', dpi=300, bbox_inches='tight')
    plt.close()

def graficar_todos_algoritmos_tiempo(promedios, tipo):
    """Graficar todos los algoritmos juntos para tiempo"""
    plt.figure(figsize=(14, 10))
    
    algoritmos = list(promedios[tipo].keys())
    colores = plt.cm.tab10(np.linspace(0, 1, len(algoritmos)))
    
    for i, algoritmo in enumerate(algoritmos):
        ns = sorted(promedios[tipo][algoritmo].keys())
        tiempos = [promedios[tipo][algoritmo][n]['tiempo_promedio'] for n in ns]
        
        plt.plot(ns, tiempos, 'o-', label=algoritmo, color=colores[i], linewidth=3, markersize=10)
    
    plt.xscale('log')
    plt.yscale('log')
    plt.xlabel('Cantidad de elementos (n)', fontsize=12)
    plt.ylabel('Tiempo (segundos)', fontsize=12)
    plt.title(f'Comparación de todos los algoritmos - Tipo: {tipo}', fontsize=14)
    plt.legend(fontsize=11)
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.savefig(f'../data/plots/todos_algoritmos_tiempo_{tipo}.png', dpi=300, bbox_inches='tight')
    plt.close()
    

def main():
    """Función principal"""
    # Crear directorios de salida si no existen
    os.makedirs('../data/plots', exist_ok=True)
    
    # Parsear datos
    print("Parseando archivos de medición...")
    datos = parse_measurement_files()
    promedios = calcular_promedios(datos)
    
    # Tipos de arreglos a procesar
    tipos = ['ascendente', 'descendente', 'aleatorio']
    
    for tipo in tipos:
        if tipo in promedios:
            print(f"Generando gráficos para tipo: {tipo}")
            
            # Gráficos individuales
            graficar_tiempo_vs_n(promedios, tipo)
            graficar_memoria_vs_n(promedios, tipo)
            
            # Gráfico con todos los algoritmos
            graficar_todos_algoritmos_tiempo(promedios, tipo)
    
    print("¡Gráficos generados exitosamente!")

if __name__ == "__main__":
    main()