# Grupo 02 - Tarea 3
## Estudiantes:
**Paralelo 200**
+ Alondra Acosta `Rol: -`
+ Sergio Cárcamo `Rol: -`

### Carpeta Tarea3
Esta carpeta contiene todos los archivos asociados a la Tarea 3. Se divide en dos subcarpetas, una para el `codigo` y otra para el `informe`.

#### codigo 
Esta carpeta contiene todos los archivos asociados a la Tarea 3 para poder ser ejecutada. En particular, los archivos que contienen los respectivos códigos para realizar la tarea solicitada son: 
- `LAB3_Acosta_Carcamo.cpp`. (Versión en el lenguaje de C++)
- `LAB3_Acosta_Carcamo.java`. (Versión en el lenguaje de Java)

Y el archivo que contiene el script para generar los gráficos implementados en el informe es:
- `script_plots.py`

En particular, la carpeta `codigo` también posee una subcarpeta `data`, donde se encuentran las siguientes carpetas:
- ***inputs***: carpeta donde se encuentran todos los grafos a procesar.
- ***measurements***: carpeta donde se encuentran los datos para graficar.
- ***outputs***: carpeta donde se encuentran los archivos salidaFork/Thread.txt solicitados.
- ***plots***: carpeta donde se encuentran los gráficos generados.

#### informe
En esta carpeta se encuentra el informe realizado en su formato `PDF` y `LaTeX`.


### Condiciones de ejecución
Tal como se menciona en el informe. desarrollo total de la tarea fue realizado bajo las siguientes condiciones:

#### Computador Alondra - programa de hebras 
- Procesador: 13th Gen Intel(R) Core(TM) i5-1335U, 1300 MHz (1.30 GHz) base, 10 procesadores
principales, 12 procesadores lógicos.
- Memoria RAM: 16,0 GB
- Almacenamiento: SSD NVMe, marca SAMSUNG MZVL41T0HBLB-00B, con una capacidad total de
954 GB.
- Sistema operativo: Ubuntu 22.04.5 LTS
- Compilador: g++ 11.4.0
- Arquitectura: x64 bits

#### Computador Sergio - programa de subprocesos
- Procesador: 13th Gen Intel(R) Core(TM) i5-1335U, 1300 MHz (1.30 GHz) base, 10 procesadores
principales, 12 procesadores lógicos.
- Memoria RAM: 16,0 GB
- Almacenamiento: SSD NVMe, marca Micron MTFDKBA512QGN-1BN1AABGA, con una capacidad total de
512 GB.
- Sistema operativo: Ubuntu 24.04.3 LTS (WSL2) bajo Microsoft Windows 11 Enterprise IoT LTSC
- Compilador: g++ 11.4.0
- Arquitectura: x64 bits

### Ejecutar programa
Para ejecutar los programas que contiene los códigos de la tarea, se debe navegar hasta la carpeta `Tarea3` y, dentro de ella, ejecutar en la terminal alguno de los siguientes comandos del `Makefile`.

#### 1. Compilar los programas de Java y C++ para ejecutarlos por separado:
 **`make`**: Al ingresar en la terminal este comando, se compilaran los programas mencionados y se generarán sus respectivos ejecutables. Para ejecutar el programa de C++, ingresar a la terminal `./lab3`, para ejecutar el programa de java, ingresar a la terminal `java LAB3_Acosta_Carcamo`.  

#### 2. Compilar y ejecutar todos los programas de la tarea
**`make stats`**: Este comando compila y ejecuta los programas del repositorio (Java, C++ y Python), haciendo un trabajo adicional de generar dos archivos, uno por cada programa, donde se escribe, entre otras cosas, el uso de CPU y uso de RAM, usado en el análisis del informe.

Para limpiar los archivos generados (ejecutables y archivos de salida), debe ejecutar `make clean`.

### Algoritmo
- **(C++)** Se aplicó el algoritmo sugerido de Bellman-Ford, el cual sirve para encontrar las distancias más cortas desde un vértice origen a todos los demás en un grafo ponderado, incluso si hay pesos negativos (siempre que no existan ciclos negativos alcanzables). En nuestra implementación, el proceso padre divide la lista total de aristas del grafo y le entrega a cada proceso hijo un subconjunto de esas aristas. Cada hijo solo es responsable de analizar su propio pedazo.
- **(Java)** Se utilizó el mismo algoritmo de Bellman-Ford para mantener una comparación justa. La paralelización se implementó asignando un subconjunto de aristas a cada Thread (hilo) "Worker". Dado que los hilos comparten memoria, se utilizó un mecanismo de sincronización con barreras `(CyclicBarrier)`. En cada iteración, los hilos leen el arreglo de distancias compartido, pero guardan sus "propuestas" de actualización en una lista local. El hilo main espera a que todos los hilos lleguen a la barrera y solo entonces aplica los cambios en el arreglo compartido. Esto garantiza que no ocurran condiciones de carrera.

Finalmente, los programas generan los mismos tipos de archivos de salida, que se pueden encontrar dentro de las carpetas data/measurements y data/outputs.
- **`salidaFork/Thread.txt`**: Se encuentran dentro de la carpeta outputs y son los archivos que contienen el camino mas corto encontrado por el algoritmo de bellman-ford.
- **`datos_cpp/java.csv`**: Son archivos auxiliares que contienen los datos usados para graficar información que se analiza en el informe y se guardan dentro de la carpeta measurements.
- **`stats_cpp/java.txt`**: Son archivos auxiliares que se generan al ejecutar `make stats`, que contienen la información de uso de CPU y RAM, se guardan dentro de la carpeta measurements.


### Supuestos y aclaraciones
- **(C++)** Se compiló sin optimizaciones (-O0) debido a la posibilidad de deadlock, y para garantizar consistencia en la comparación con Java.
- **(C++)** la cantidad de procesos hijos es igual a la cantidad de procesadores (núcleos lógicos de CPU) disponibles en la máquina donde se ejecuta.
- **(Java)** Al igual que en C++, la cantidad de hebras es igual a la cantidad de procesadores (nucleos lógicos de CPU) disponibles.
- Se asume que los archivos de entrada siempre estarán dentro de la carpeta `inputs` y tendrán un formato como `grafo_numero.txt` (ej. grafo_1.txt).
