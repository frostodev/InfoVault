import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import os
import re 

DATOS_DIR = 'data/measurements' 

CPP_FILE = os.path.join(DATOS_DIR, 'datos_cpp.csv')
JAVA_FILE = os.path.join(DATOS_DIR, 'datos_java.csv')

CPP_STATS_FILE = os.path.join(DATOS_DIR, 'stats_cpp.txt')
JAVA_STATS_FILE = os.path.join(DATOS_DIR, 'stats_java.txt')

GRAFICOS_DIR = 'data/plots'

os.makedirs(GRAFICOS_DIR, exist_ok=True)

try:
    df_cpp = pd.read_csv(CPP_FILE)
    df_java = pd.read_csv(JAVA_FILE)
except FileNotFoundError as e:
    print(f"Error FATAL: No se pudo encontrar el archivo: {e.filename}")
    print(f"Asegúrate de que tus archivos .csv estén en '{DATOS_DIR}'")
    exit(1)

print("Archivos CSV de tiempos cargados exitosamente.")

df_cpp['programa'] = 'C++ (fork)'
df_java['programa'] = 'Java (threads)'

df_combined = pd.concat([df_cpp, df_java])

df_success = df_combined[df_combined['exito'] == 'exito'].copy()

df_success['tiempoMS'] = pd.to_numeric(df_success['tiempoMS'])
df_success['numero_nodos'] = pd.to_numeric(df_success['numero_nodos'])
df_success['numero_aristas'] = pd.to_numeric(df_success['numero_aristas'])

def parse_stats_file(filepath, program_name):
    """
    Lee un archivo .txt de stats (salida de /usr/bin/time -v) 
    y extrae los valores de CPU y RAM.
    """
    cpu, ram = None, None
    
    # Expresiones regulares para la salida de /usr/bin/time -v
    cpu_re = re.compile(r'Percent of CPU this job got: ([\d\.]+)%')
    ram_re = re.compile(r'Maximum resident set size \(kbytes\): ([\d\.]+)')
    
    try:
        with open(filepath, 'r') as f:
            content = f.read() 
            
            cpu_match = cpu_re.search(content)
            ram_match = ram_re.search(content)
            
            if cpu_match:
                cpu = float(cpu_match.group(1))
            
            if ram_match:
                ram_kbytes = float(ram_match.group(1))
                ram = ram_kbytes / 1024.0
                
    except FileNotFoundError:
        print(f"Advertencia: No se encontró el archivo de stats: {filepath}")
        return None
    except Exception as e:
        print(f"Advertencia: Error al parsear {filepath}: {e}")
        return None

    if cpu is None or ram is None:
        print(f"Advertencia: No se pudieron encontrar los datos de CPU o RAM en {filepath}")
        print("  -> Verifique el contenido del archivo y las expresiones regulares.")
        return None
        
    return {'programa': program_name, 'cpu_percent': cpu, 'ram_mb': ram}

stats_data = []
stats_data.append(parse_stats_file(CPP_STATS_FILE, 'C++ (fork)'))
stats_data.append(parse_stats_file(JAVA_STATS_FILE, 'Java (threads)'))
stats_data = [d for d in stats_data if d is not None]
df_stats = pd.DataFrame(stats_data)

if not df_stats.empty:
    print("Archivos TXT de estadísticas cargados y parseados exitosamente.")
    print("Datos de CPU/RAM extraídos (RAM convertida a MiB):")
    print(df_stats.to_string(index=False))
else:
    print("Advertencia: No se pudieron cargar datos de estadísticas (CPU/RAM).")

# grafico 1 -- Tiempo vs Cantidad de Nodos
print("\nGenerando Gráfico 1: Nodos vs. Tiempo...")
plt.figure(figsize=(10, 6))
sns.lineplot( 
    data=df_success,
    x='tiempoMS',       
    y='numero_nodos',   
    hue='programa',     
    palette={'C++ (fork)': 'blue', 'Java (threads)': 'orange'},
    marker='o',         
    linewidth=2.5       
)

plt.title('Cantidad de Nodos vs. Tiempo de Ejecución (Tendencia)', fontsize=16)
plt.xlabel('Tiempo de Ejecución (ms)', fontsize=12)
plt.ylabel('Cantidad de Nodos', fontsize=12)
plt.legend(title='Programa')
plt.grid(True, linestyle='--', alpha=0.6)
plt.tight_layout()
plot1_path = os.path.join(GRAFICOS_DIR, '1_nodos_vs_tiempo.png')
plt.savefig(plot1_path)
plt.close()

# Grafico 2 -- Tiempo vs Cantidad de Aristas
print("Generando Gráfico 2: Aristas vs. Tiempo...")
plt.figure(figsize=(10, 6))
sns.lineplot( 
    data=df_success,
    x='tiempoMS',           
    y='numero_aristas',     
    hue='programa',
    palette={'C++ (fork)': 'blue', 'Java (threads)': 'orange'},
    marker='o',         
    linewidth=2.5       
)

plt.title('Cantidad de Aristas vs. Tiempo de Ejecución (Tendencia)', fontsize=16)
plt.xlabel('Tiempo de Ejecución (ms)', fontsize=12)
plt.ylabel('Cantidad de Aristas', fontsize=12)
plt.legend(title='Programa')
plt.grid(True, linestyle='--', alpha=0.6)
plt.tight_layout()
plot2_path = os.path.join(GRAFICOS_DIR, '2_aristas_vs_tiempo.png')
plt.savefig(plot2_path)
plt.close()

# graficos de Barras (CPU y RAM)
if not df_stats.empty:
    
    # grafico 3: Uso de CPU 
    print("Generando Gráfico 3: Uso de CPU...")
    plt.figure(figsize=(8, 6))
    sns.barplot(
        data=df_stats,
        x='programa',
        y='cpu_percent',
        hue = 'programa',
        palette={'C++ (fork)': 'blue', 'Java (threads)': 'orange'},
        legend=False
    )
    plt.title('Uso Promedio de CPU', fontsize=16)
    plt.xlabel('Programa', fontsize=12)
    plt.ylabel('Uso de CPU (%)', fontsize=12)
    plt.tight_layout()
    plot3_path = os.path.join(GRAFICOS_DIR, '3_uso_cpu.png')
    plt.savefig(plot3_path)
    plt.close()

    # grafico 4: Uso de RAM
    print("Generando Gráfico 4: Uso de RAM...")
    plt.figure(figsize=(8, 6))
    sns.barplot(
        data=df_stats,
        x='programa',
        y='ram_mb',
        hue='programa',
        palette={'C++ (fork)': 'blue', 'Java (threads)': 'orange'},
        legend=False
    )
    plt.title('Uso Máximo de RAM', fontsize=16)
    plt.xlabel('Programa', fontsize=12)
    plt.ylabel('Uso de RAM (MiB)', fontsize=12) 
    plt.tight_layout()
    plot4_path = os.path.join(GRAFICOS_DIR, '4_uso_ram.png')
    plt.savefig(plot4_path)
    plt.close()

else:
    print("Saltando gráficos 3 y 4 (CPU/RAM) porque no se pudieron cargar los datos de stats.")


print(f"\nFinalizado. Gráficos guardados en la carpeta: {GRAFICOS_DIR}")