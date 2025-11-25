# Grupo 02 - Tarea 4
## Estudiantes:
**Paralelo 200**
+ Alondra Acosta `Rol: -`
+ Sergio Cárcamo `Rol: -`

### Carpeta Tarea4
Esta carpeta contiene todos los archivos asociados a la Tarea 4 para poder ser ejecutada. En particular, el archivo que contiene el código para realizar la tarea solicitada es: `LAB4_Acosta_Carcamo.cpp`.

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
Para ejecutar el programa que contiene el código de la tarea, se debe navegar hasta la carpeta `Tarea4` y dentro de ella, ejecutar en la terminal el comando `python3 LAB4_Acosta_Carcamo.py`.

### Algoritmo
El sistema simula un entorno de extracción concurrente de recursos utilizando la librería threading de Python. El algoritmo principal orquesta la interacción entre 64 hebras (Trabajadores) y 16 recursos compartidos (Zonas) a través de 8 rondas de ejecución sincronizada.

#### Arquitectura del programa

El programa se estructura en tres clases principales:
- **Trabajador (Hebra)**: Encapsula el estado individual (resistencia, mochila, historial). Posee un Lock propio para asegurar la exclusión mutua al modificar sus atributos vitales.

- **Zona (Recurso Compartido)**: Representa el área de extracción. Gestiona la concurrencia mediante un Semaphore (capacidad máxima) y un Lock para proteger el inventario de cristales y la lista de trabajadores presentes.

- **SistemaExtraccion**: Inicializa el entorno, gestiona el ciclo de vida de las hebras, la sincronización global por rondas y la generación de logs.

#### Estrategias de sincronización

Para garantizar la consistencia de datos y cumplir los requisitos de coordinación, se implementaron los siguientes mecanismos:

- **Sincronización de rondas (threading.Barrier)**: Se utiliza una barrera al inicio de la función trabajador_trabajar(). Esto asegura que ninguna hebra comience una nueva ronda hasta que todas las hebras activas y el hilo principal estén listos, garantizando que las rondas de 15 segundos (simuladas) sean uniformes.

- **Control de acceso a zonas (threading.Semaphore)**: Cada objeto Zona cuenta con un semáforo inicializado en 8. Esto limita estrictamente la cantidad de trabajadores simultáneos en una zona, bloqueando el acceso si la capacidad está completa.

- **Protección de recursos críticos (threading.Lock)**: 
	- **Nivel trabajador**: Protege la modificación de la resistencia y la mochila para evitar condiciones de carrera durante actualizaciones de estado o intercambios.
 
	- **Nivel zona**: Protege la variable cristales para evitar que dos trabajadores extraigan el mismo recurso simultáneamente.
 
	- **Logs**: Un lock global serializa la escritura en los diccionarios de logs para evitar salidas corruptas o entremezcladas.
 
#### Lógica de toma de decisiones (Algoritmo del trabajador)

En cada ronda, las hebras ejecutan el siguiente flujo lógico:

- **Verificación de estado**: Si el trabajador está "Agotado", entra en lógica de descanso (omite la ronda para recuperar resistencia). Si está "Intoxicado", termina su ejecución inmediatamente.

- **Sistema de prioridad (Selección de zona)**: Se implementó un algoritmo de prioridad dinámica. Si el trabajador tiene menos de 5 cristales en su mochila, ordena las zonas por cantidad de cristales (descendente) para intentar acceder a las más ricas. De lo contrario, elige una zona aleatoria.

- **Extracción y competencia**:
	- Al entrar a la zona, se calcula una probabilidad éxito del 70%.
	
	- **Éxito**: Extrae entre 1 y 3 cristales (limitado por espacio en mochila y disponibilidad de zona) y reduce su resistencia según la cantidad extraída (5, 12 o 20 puntos).
	
	- **Conflicto**: Si cae en el 30% de probabilidad de fallo, no extrae nada y pierde 10 puntos de resistencia adicional.
	
- **Cálculo de toxicidad**: Antes de salir, se reduce la resistencia basada en la fórmula: `toxicidad_zona / trabajadores_presentes`, simulando la distribución del daño ambiental

#### Lógica Post-ronda (Hebra principal)

Al finalizar la ejecución de todas las hebras en una ronda (sincronizado vía join):

- **Gestión de estados**: Se actualizan los contadores de descanso para los trabajadores agotados.

- **Intercambio cooperativo**: Se ejecuta un algoritmo greedy donde los trabajadores activos con excedente de recursos (>3 cristales) transfieren carga a trabajadores agotados aleatorios para ayudarles a recuperar resistencia (+15 puntos), fomentando la supervivencia del equipo.

### Supuestos y aclaraciones
+ Se asume que la 'duración de 15 segundos'  es una descripción narrativa y no una directiva de implementación, por lo que la simulación de la ronda se ejecuta computacionalmente tan rápido como es posible.