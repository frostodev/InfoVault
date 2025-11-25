
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.StringTokenizer;
import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class LAB3_Acosta_Carcamo {
    
    // Constante para infinito
    final long INF = Long.MAX_VALUE;
    // estructura de arista
    record Arista(int origen, int destino, int peso) {}
    // estructura actualizacion
    record Actualizacion(int nodoActualizado, long nuevaDistancia, int predecesor) {}

    // se obtiene la cantidad de procesadores lógicos del notebook (12)
    final int NUM_THREADS = Runtime.getRuntime().availableProcessors();

    // datos del grafo
    int numNodos;
    int nodoInicio;
    int nodoFinal;
    List<Arista> todasLasAristas = new ArrayList<>();
    
    // Arrays de Bellman-Ford
    long[] distancias;
    int[] predecesores;

    // Barreras para coordinar las fases de "trabajo" y "consolidación"
    CyclicBarrier startBarrier;
    CyclicBarrier endBarrier;

    // Lista de los workers y los threads que los ejecutan
    Worker[] workers;
    Thread[] threads;

    double duracionMs = 0.0;
    boolean cicloNegativoDetectado = false;
    /*
    Clase interna que representa el "proceso hijo" (Worker Thread)
    Implementa Runnable, que es lo que un Thread sabe ejecutar
    */
    class Worker implements Runnable {
        private final List<Arista> misAristas;
        
        // Cada worker tiene su propia lista de actualizaciones propuestas
        // El Thread "main" la leerá en la fase de consolidación
        public final List<Actualizacion> misActualizaciones = new ArrayList<>();

        public Worker(List<Arista> misAristas) {
            this.misAristas = misAristas;
        }

        @Override
        public void run() {
            try {
                // V iteraciones de Bellman-Ford
                for (int iter = 0; iter < numNodos; iter++) {
                    
                    // Esperar en la barrera de INICIO
                    // Nadie se mueve hasta que "main" y todos los workers estén aquí
                    startBarrier.await();

                    // Si el thread "main" interrumpió a algun thread (early exit), se sale
                    if (Thread.currentThread().isInterrupted()) {
                        break;
                    }

                    // fase de trabajo paralela
                    misActualizaciones.clear();
                    
                    // Lee "distancias"
                    for (Arista a : misAristas) {
                        if (distancias[a.origen] != INF && 
                            (distancias[a.origen] + a.peso < distancias[a.destino])) {
                            
                            // Propone una actualización
                            misActualizaciones.add(new Actualizacion(
                                a.destino, 
                                distancias[a.origen] + a.peso, 
                                a.origen
                            ));
                        }
                    }

                    // Esperar en la barrera de FIN.
                    // Se le avisa al "main" que se terminó la revision
                    endBarrier.await();
                }
            } catch (InterruptedException | BrokenBarrierException e) {
                // El thread "main" rompió la barrera (early exit) o interrumpió a un thread
                // Simplemente termina
            }
        }
    } // Fin de la clase Worker

    
    // Método principal que orquesta la ejecución de Bellman-Ford
    public String runBellmanFord() {
        // Inicialización
        distancias = new long[numNodos];
        predecesores = new int[numNodos];
        Arrays.fill(distancias, INF);
        Arrays.fill(predecesores, -1);
        distancias[nodoInicio] = 0;

        // Configuración de Barreras y Workers
        // +1 para incluir al hilo '"main" en la sincronización
        startBarrier = new CyclicBarrier(NUM_THREADS + 1);
        endBarrier = new CyclicBarrier(NUM_THREADS + 1);

        workers = new Worker[NUM_THREADS];
        threads = new Thread[NUM_THREADS];

        // Repartir las aristas 
        List<List<Arista>> aristasPorThread = new ArrayList<>(NUM_THREADS);
        for (int i = 0; i < NUM_THREADS; i++) {
            aristasPorThread.add(new ArrayList<>());
        }
        for (int i = 0; i < todasLasAristas.size(); i++) {
            aristasPorThread.get(i % NUM_THREADS).add(todasLasAristas.get(i));
        }

        // Crear e iniciar los hilos
        for (int i = 0; i < NUM_THREADS; i++) {
            workers[i] = new Worker(aristasPorThread.get(i));
            threads[i] = new Thread(workers[i]);
            threads[i].start();
        }

        // Loop Principal de Bellman-Ford (thread "main")   
        long tiempoInicio = System.nanoTime();
        cicloNegativoDetectado = false;

        for (int iter = 0; iter < numNodos; iter++) {
            try {
                // FASE DE TRABAJO
                // "main" llega a la barrera de inicio. Esto libera a los workers
                startBarrier.await();
                
                // "main" llega a la barrera de fin. Espera a que todos los workers terminen
                endBarrier.await();

            } catch (InterruptedException | BrokenBarrierException e) {
                System.err.println("Error en la barrera del hilo main: " + e.getMessage());
                break;
            }

            // FASE DE CONSOLIDACIÓN
            // En este punto todos los workers están pausados (en startBarrier.await())
            // y por lo tanto, es seguro para "main" leer sus listas y escribir en los arrays compartidos
            
            boolean huboCambioGlobal = false;
            
            for (int i = 0; i < NUM_THREADS; i++) {
                for (Actualizacion upd : workers[i].misActualizaciones) {
                    if (upd.nuevaDistancia < distancias[upd.nodoActualizado]) {
                        huboCambioGlobal = true;
                        
                        // Chequeo de ciclo negativo (última iteración)
                        if (iter == numNodos - 1) {
                            this.cicloNegativoDetectado = true;
                            predecesores[upd.nodoActualizado] = -2; // Marcar como ciclo
                        } else {
                            // Aplicar actualización 
                            distancias[upd.nodoActualizado] = upd.nuevaDistancia;
                            predecesores[upd.nodoActualizado] = upd.predecesor;
                        }
                    }
                }
            }

            // Chequeo de Salida Temprana (Early Exit)
            if (!huboCambioGlobal && iter < numNodos - 1) {
                // No hubo cambios, Bellman-Ford terminó.
                // Interrumpimos a todos los workers para que salgan de su loop
                for (Thread t : threads) {
                    t.interrupt();
                }
                break;
            }
        }

        // Join. Esperar a que todos los threads terminen su ejecución
        for (Thread t : threads) {
            try {
                t.join();
            } catch (InterruptedException e) {
                System.err.println("Error esperando (join) al thread: " + e.getMessage());
            }
        }
        
        long tiempoFin = System.nanoTime();
        duracionMs = (tiempoFin - tiempoInicio) / 1_000_000.0;

        // Formatear Resultados
        return formatResult();
    }


    // Carga los datos de un archivo de entrada (los grafos).
    public boolean cargar_grafo(String inputFilePath) {
        try (BufferedReader br = new BufferedReader(new FileReader(inputFilePath))) {
            numNodos = Integer.parseInt(br.readLine());
            nodoInicio = Integer.parseInt(br.readLine());
            nodoFinal = Integer.parseInt(br.readLine());

            todasLasAristas.clear(); // Limpiar aristas de grafos anteriores
            String line;
            while ((line = br.readLine()) != null) {
                StringTokenizer st = new StringTokenizer(line);
                int origen = Integer.parseInt(st.nextToken());
                int destino = Integer.parseInt(st.nextToken());
                int peso = Integer.parseInt(st.nextToken());
                todasLasAristas.add(new Arista(origen, destino, peso));
            }
            return true;
        } catch (IOException | NumberFormatException e) {
            System.err.println("Error leyendo el archivo " + inputFilePath + ": " + e.getMessage());
            return false;
        }
    }

    
    // Reconstruye el string del camino
    private String reconstruirCamino() {
        List<Integer> camino = new ArrayList<>();
        int actual = nodoFinal;

        while (actual != -1) {
            camino.add(actual);
            if (actual == nodoInicio) break;
            actual = predecesores[actual];
        }

        if (camino.isEmpty() || camino.get(camino.size() - 1) != nodoInicio) {
            return "(Camino no encontrado o no reconstruido)";
        }

        Collections.reverse(camino);
        
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < camino.size(); i++) {
            sb.append(camino.get(i));
            if (i < camino.size() - 1) {
                sb.append(" -> ");
            }
        }
        return sb.toString();
    }


    // Formatea el string de salida 
    private String formatResult() {
        StringBuilder sb = new StringBuilder();
        sb.append(String.format("Grafo de %d nodos. Ruta de %d a %d.\n", numNodos, nodoInicio, nodoFinal));

        if (this.cicloNegativoDetectado) {
            sb.append("Resultado: Se detectó un ciclo de peso negativo\n");
        } else if (distancias[nodoFinal] == INF) {
            sb.append("Resultado: No se encontró un camino\n");
        } else {
            String caminoStr = reconstruirCamino();
            sb.append("Resultado: Éxito\n");
            sb.append("Distancia más corta: ").append(distancias[nodoFinal]).append("\n");
            sb.append("Camino: ").append(caminoStr).append("\n");
        }
        
        sb.append(String.format("Tiempo de ejecución (%d threads): %.4f ms.\n", NUM_THREADS, this.duracionMs));
        return sb.toString();
    }

    // programa main
public static void main(String[] args) {
        // archivos de entrada (los grafos)
        String inputDir = "data/inputs/";
        
        // archivo de salida "salidaThread.txt" donde se encuentran los caminos mas cortos del bellman-ford
        String outputFilePath = "data/outputs/salidaThread.txt"; 
        
        // ruta del archivo de salida "datos_java.csv" con los datos necesarios para graficar en el informe
        String csvOutputFilePath = "data/measurements/datos_java.csv";

        // Limpiar archivo de salida ("salidaThread.txt") anterior
        try {
            Files.deleteIfExists(Paths.get(outputFilePath));
        } catch (IOException e) {
            System.err.println("No se pudo limpiar el archivo de salida anterior: " + e.getMessage());
        }

        System.out.println("Iniciando procesamiento con Java Threads...");
        System.out.println("Usando " + Runtime.getRuntime().availableProcessors() + " threads.");

        // abrir los archivos para escribir y gestionar los archivos de entrada
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(outputFilePath, true));
             BufferedWriter csvWriter = new BufferedWriter(new FileWriter(csvOutputFilePath)); 
             Stream<Path> paths = Files.walk(Paths.get(inputDir))) {

            // Header del csv
            csvWriter.write("numero_grafo,exito,numero_nodos,numero_aristas,tiempoMS");
            csvWriter.newLine();

            List<Path> files = paths
                .filter(Files::isRegularFile)
                .filter(path -> path.toString().endsWith(".txt"))
                .sorted()
                .collect(Collectors.toList());

            if (files.isEmpty()) {
                 System.err.println("No se encontraron archivos .txt en " + inputDir);
                 return;
            }

            // Bucle principal que procesa cada archivo
            for (Path entry : files) {
                String inputFilePath = entry.toString();
                System.out.println("Procesando: " + inputFilePath);
                
                // Crear una nueva instancia de la clase principal para este grafo
                LAB3_Acosta_Carcamo lab = new LAB3_Acosta_Carcamo();
                
                if (!lab.cargar_grafo(inputFilePath)) {
                    continue; // Saltar al siguiente archivo si hay error
                }

                // Ejecutar Bellman-Ford para este grafo
                String resultado = lab.runBellmanFord();

                // Escribir los resultados de este grafo al archivo de salidaThread
                writer.write("\n★Resultados para " + inputFilePath + "★");
                writer.newLine();
                writer.write(resultado);
                writer.newLine();

                // escritura del csv
                try {
                    String nombreArchivo = entry.getFileName().toString();
                    csvWriter.write(nombreArchivo + ",");

                    // Determinar el estado
                    String estado;
                    if (lab.cicloNegativoDetectado) {
                        estado = "ciclo_negativo";
                    } else if (lab.distancias[lab.nodoFinal] == lab.INF) {
                        estado = "sin_camino";
                    } else {
                        estado = "exito";
                    }
                    csvWriter.write(estado + ",");
                    
                    // Escribir el resto de los datos
                    csvWriter.write(lab.numNodos + ",");
                    csvWriter.write(lab.todasLasAristas.size() + ",");
                    csvWriter.write(String.format(java.util.Locale.US, "%.4f", lab.duracionMs));
                    csvWriter.newLine();

                } catch (IOException e_csv) {
                    System.err.println("Error escribiendo datos CSV para " + inputFilePath + ": " + e_csv.getMessage());
                }
            }

        } catch (IOException e) { 
            System.err.println("Error durante el procesamiento de archivos: " + e.getMessage());
        }
        
        System.out.println("Procesamiento completado. Resultados en: " + outputFilePath);
        System.out.println("Datos de medición en: " + csvOutputFilePath);
    }
}