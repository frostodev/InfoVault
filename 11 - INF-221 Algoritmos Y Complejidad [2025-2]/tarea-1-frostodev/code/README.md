# Documentación

**Nota: debido a limitaciones de ancho de banda, los outputs de los algoritmos de multiplicación de matrices no se incluyen.**

## Multiplicación de matrices

### Programa principal
- Función: Comparar ambos algoritmos de multipliación de Matrices (Naive y Strassen)
- Uso:
	- make
	- ./matrix_multiplication

### Scripts
- matrix_generator.py: Genera las matrices de prueba en matrix_multiplication/data/matrix_input
- plot_generator.py: Genera gráficos de comparación de tiempo/espacio

Requiere instalación de las siguiente librerías usando pip:
- pandas
- matplotlib
- scipy

## Ordenamiento de arreglo unidimensional

### Programa principal
- Función: Comparar los algoritmos de ordenamiento de arreglos (InsertionSort, QuickSort (Middle element), MergeSort, PandaSort, std::sort)
- Uso: 
	- make
	- ./sorting

### Scripts
- array_generator.py: Genera las matrices de prueba en sorting/data/matrix_input
- plot_generator.py: Genera gráficos de comparación de tiempo/espacio

Requiere instalación de las siguiente librerías usando pip:
- pandas
- matplotlib
- scipy

