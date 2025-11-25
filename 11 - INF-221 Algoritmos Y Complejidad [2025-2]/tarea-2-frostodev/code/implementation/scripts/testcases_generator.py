import os
import random

def generar_valores(perfil_rango):    
    # Selección de rango de productividad
    if perfil_rango == 'largo_mixto':
        # Perfil 1: Rango original y completo
        rand_A = random.randint(-10**9, 10**9)
        rand_B = random.randint(-10**9, 10**9)
    
    elif perfil_rango == 'corto_mixto':
        # Perfil 2: Rango corto, centrado en cero
        rand_A = random.randint(-100, 100)
        rand_B = random.randint(-100, 100)
    
    elif perfil_rango == 'corto_positivo':
        # Perfil 3: Rango corto, solo positivo
        rand_A = random.randint(1, 100)
        rand_B = random.randint(1, 100)
    
    elif perfil_rango == 'A_garantizado_mejor':
        # Perfil 4: A (favorito) es siempre bueno, B (otro) es siempre malo
        rand_A = random.randint(1, 500)
        rand_B = random.randint(-500, 0)
    
    else:
        # Fallback por si las moscas
        rand_A = random.randint(-100, 100)
        rand_B = random.randint(-100, 100)

    string = f"{rand_A} {rand_B} "
    
    # Selección de lenguaje
    rand_lang = random.randint(1, 5)
    if rand_lang == 1:
        lang = "c"
    elif rand_lang == 2:
        lang = "cpp"
    elif rand_lang == 3:
        lang = "java"
    elif rand_lang == 4:
        lang = "scheme"
    else:
        lang = "prolog"
    
    string += lang
    return string

def generar_caso(n, p, perfil_rango):    
    # Ruta al directorio 'data/inputs/'
    script_dir = os.path.dirname(__file__)
    ruta_directorio = os.path.abspath(os.path.join(script_dir, "..", "data", "inputs"))
    os.makedirs(ruta_directorio, exist_ok=True)
    
    # Nombre del archivo
    nombre_archivo = f"testcases_{n}_{p}.txt"
    ruta_archivo = os.path.join(ruta_directorio, nombre_archivo)

    print(f"Generando caso {nombre_archivo} (n={n}, id={p}, perfil={perfil_rango})...")

    try:
        with open(ruta_archivo, "w") as archivo:
            # n
            archivo.write(f"{n}\n")

            # A B C
            for _ in range(n):
                linea = generar_valores(perfil_rango)
                archivo.write(linea + "\n")

    except IOError as e:
        print(f"No se pudo escribir el archivo en {ruta_archivo}. {e}")
    
    return

##################################################

# Valores de N a generar
valores = [
    5,       # Ejemplo 1
    6,       # Ejemplo 2
    15,      # Límite bajo de Backtracking
    18,      # Mediano para Backtracking
    20,      # Mediano para Backtracking
    25,      # Casi límite alto de Backtracking
    28,      # Límite alto de Backtracking
    30,      # Límite máximo de Backtracking
    50,      # Mediano-bajo
    100,     # Mediano
    500,     # Mediano-alto
    1000,    # Grande
    5000,    # Muy grande
    9999     # Máximo (límite N < 10^4)
]

# Perfiles de rango de productividad
perfiles = [
    'largo_mixto', 
    'corto_mixto', 
    'corto_positivo', 
    'A_garantizado_mejor'
]

total_casos = 0
for n in valores:
    # Para cada n, generamos un caso por cada perfil
    # El 'id' (p) será el índice del perfil (0, 1, 2, 3)
    for i, perfil in enumerate(perfiles):
        generar_caso(n, i, perfil)
        total_casos += 1

print(f"\nGeneración completada")