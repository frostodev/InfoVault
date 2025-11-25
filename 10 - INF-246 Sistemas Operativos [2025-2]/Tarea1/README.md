# Grupo 02 - Tarea 1
## Estudiantes:
**Paralelo 200**
+ Alondra Acosta `Rol: -`
+ Sergio Cárcamo `Rol: -`

### Carpeta Tarea1
Esta carpeta contiene todos los archivos asociados a la Tarea 1 para poder ser ejecutada. En particular, el archivo que contiene el código para realizar la tarea solicitada es: `LAB1_Acosta_Carcamo.cpp`.

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
Para ejecutar el programa que contiene el código de la tarea, se debe navegar hasta la carpeta `Tarea1` y dentro de ella, ejecutar en la terminal el comando `make`, lo que generará un ejecutable `./lab1` que debe ingresarse en la terminal.

Para limpiar los archivos generados, debe ejecutar `make clean`.

### Algoritmo
Nuestra tarea implementa un algoritmo que consiste en tres etapas que clasifican y organizan automáticamente archivos, basado en sus contenidos.

#### Etapa 1: Análisis
Recorre recursivamente todos los archivos en el directorio `filesystem_root`, leyendo linea por linea y buscando patrones para clasificarlos.

#### Etapa 2: Organización
+ Se crea la estructura de directorios de salida(Output/Pistas/...)
+ Determina el directorio de destino del archivo según la categoría identificada.
+ Mantiene los contadores asociados a cada categoría.

#### Etapa 3: Renombre y guardado
+ Usando los contadores asociados a las categorías, se generan nombres descriptivos como: pista1.txt, driver2.txt, etc.
+ Se copia cada archivo a su directorio correspondiente con el nuevo nombre y se incrementa el contador de la categoría asociada.

### Supuestos
+ Se asume que el directorio `filesystem_root` está en la misma ruta que el ejecutable compilado.
+ Se asume que el código será compilado y ejecutado en alguna distribución de Linux.