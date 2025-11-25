# Documentación

Este directorio contiene el código fuente y los scripts para la Tarea 2 de Algoritmos y Complejidad (CppCorp).

La información a detalle sobre implementación y supuestos se encuentra en el informe.

## Implementación

El núcleo de la solución se encuentra dividido en los siguientes módulos de C++:

* **`brute-force.cpp`**: Implementación de fuerza bruta (Backtracking) para encontrar la solución óptima exhaustiva.
* **`greedy1.cpp`**: Primera heurística greedy, enfocada en maximizar la productividad absoluta de cada segmento localmente.
* **`greedy2.cpp`**: Segunda heurística greedy, enfocada en maximizar la densidad (promedio) de productividad por empleado.
* **`dynamic-programming.cpp`**: Solución óptima eficiente implementada mediante Programación Dinámica (*bottom-up*).

### Programa principal

* **`general.cpp`**: Es el punto de entrada de la aplicación. Se encarga de leer los archivos de entrada, orquestar la ejecución de los algoritmos seleccionados y registrar las métricas de tiempo y memoria resultantes.

Los casos de prueba deben generarse **antes** de la compilación, usando el script `/scripts/testcases_generator.py`.

Para compilar, basta con ejecutar `make` y luego `make run` desde **/code/implementation**, el cual compilará, ejecutará el programa principal y posteriormente generará los gráficos.

### Scripts

Herramientas de soporte escritas en Python:

* **`testcases_generator.py`**: Genera sets de casos de prueba aleatorios variando el tamaño $n$ y los perfiles de productividad.
* **`plot_generator.py`**: Procesa los archivos de salida y mediciones para generar los gráficos comparativos (tiempo, memoria y calidad) utilizados en el reporte.