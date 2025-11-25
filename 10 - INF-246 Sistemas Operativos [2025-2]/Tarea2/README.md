# Grupo 02 - Tarea 2
## Estudiantes:
**Paralelo 200**
+ Alondra Acosta `Rol: -`
+ Sergio Cárcamo `Rol: -`

### Carpeta Tarea2
Esta carpeta contiene todos los archivos asociados a la Tarea 2 para poder ser ejecutada. En particular, el archivo que contiene el código para realizar la tarea solicitada es: `LAB2_Acosta_Carcamo.cpp`.

### Condiciones de ejecución
El desarrollo total de la tarea fue realizado bajo las siguientes condiciones:
- Procesador: 13th Gen Intel(R) Core(TM) i5-1335U, 1300 MHz (1.30 GHz) base, 10 procesadores
principales, 12 procesadores lógicos.
- Memoria RAM: 16,0 GB
- Almacenamiento: SSD NVMe, marca SAMSUNG MZVL41T0HBLB-00B, con una capacidad total de
954 GB.
- Sistema operativo: Ubuntu 22.04.5 LTS
- Compilador: g++ 11.4.0

### Ejecutar programa
Para ejecutar el programa que contiene el código de la tarea, se debe navegar hasta la carpeta `Tarea2` y dentro de ella, ejecutar en la terminal el comando `make`, lo que generará un ejecutable `./lab2` que debe ingresarse en la terminal.

Para limpiar los archivos generados, debe ejecutar `make clean`.

### Algoritmo
El programa simula un escenario de supervivencia gestionado por un proceso coordinador (padre) y cuatro equipos de recolección (hijos), cuya comunicación se realiza mediante memoria compartida. El flujo del algoritmo se divide en tres etapas principales:

#### Etapa 1: Configuración inicial
+ Se crea la memoria compartida (`shared memory`), que servirá como el canal de comunicación entre el coordinador y los equipos.
+ Se inicializan las variables globales de la simulación: la moral del grupo en 100, los recursos en 0 y el estado de juego como activo.
+ El programa solicita al usuario que ingrese el número de días que durará la simulación, validando que el valor esté entre 10 y 30 días.

#### Etapa 2: Ciclo de simulación diaria
En esta etapa, el programa entra en un bucle principal que se ejecuta una vez por cada "día" de supervivencia simulado.

+ **Creación de equipos (procesos hijos)**: Cada día, el proceso coordinador (padre) crea 4 procesos hijos utilizando `fork()`, siendo uno para cada equipo de recolección (Agua, Alimentos, Construcción y Señales).
+ **Lógica de los equipos (hijos)**: Cada proceso hijo ejecuta su lógica de forma independiente y paralela entre si.
  - Simula sus tareas de recolección.
  - Calcula la cantidad de recursos obtenidos según un sistema de probabilidades.
  - Escribe su reporte.
+ **Coordinación y evaluación (proceso padre)**: El proceso padre espera a que los 4 hijos terminen su ejecución mediante un `wait()`. Una vez que todos los equipos han reportado:
  - Lee los resultados de la memoria compartida y muestra un resumen del día.
  - Evalúa si se cumplieron los requisitos mínimos de recursos diarios (agua, alimentos, etc...).
  - Ajusta la moral del grupo según los descuentos por recursos insuficientes.
  - Verifica las condiciones de victoria o derrota, para evaluar si se continua o se detiene la simulación.
 
#### Etapa 3: Fin de la simulación
Una vez que el bucle termina por alguna de las condiciones, el programa libera correctamente la memoria compartida para evitar fugas de recursos en el sistema operativo, luego, se muestra un mensaje indicando la razón por la que terminó la simulación (rescate, fracaso o límite de tiempo).


### Supuestos y aclaraciones
+ Se asume que el usuario es la persona que ejecuta el código, no el programador, por eso está la opción de escoger cuantos días a simular.
+ Se asume que la entrada de los días es un valor entero entre 10 y 30.
+ La penalización de moral, se implementó como una pérdida de 5 puntos por cada categoria de recurso que no alcanzó el mínimo diario.
+ Los rangos de recursos obtenidos en los resultados, se definieron de la siguiente manera:
  - Éxito total: entre 100% y 120%. Esto debido a que podía excederse un poco más del mínimo según lo observado en la interfaz presentada en el enunciado.
  - Éxito parcial: Entre 50% y 80% del objetivo.
  - Fracaso: Menos del 30% del objetivo.
+ Se asume que el código será compilado y ejecutado en un sistema operativo tipo UNIX (como Linux), ya que se utiliza librerías específicas de POSIX para la gestión de procesos.
