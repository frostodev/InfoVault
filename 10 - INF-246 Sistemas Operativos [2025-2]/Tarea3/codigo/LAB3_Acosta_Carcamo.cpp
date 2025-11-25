#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <climits>    // Solo para LLONG_MAX
#include <unistd.h>
#include <sys/wait.h>
#include <algorithm>  // Solo para reverse
#include <iomanip>
#include <chrono>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

// Cantidad de procesos hijo
//#define NUM_HIJOS 10
#define NUM_HIJOS sysconf(_SC_NPROCESSORS_ONLN) // En nuestro caso, son 12 núcleos lógicos, entonces 12 procesos

const long long INF = LLONG_MAX;

// Struct para representar una arista del grafo
struct Arista {
    int origen;
    int destino;
    int peso;
};

// Struct para la comunicación
// Un proceso hijo envía esto al padre cuando encuentra una mejora
struct Actualizacion {
    int nodo_actualizado;      // El nodo que encontramos cómo acortar
    long long nueva_distancia; // La nueva distancia más corta
    int predecesor;            // El nodo desde el que llegamos
};

// Función que ejecuta cada proceso hijo. Es un loop persistente que espera trabajo del padre
void funcion_hijo(const vector<Arista>& mis_aristas, int num_nodos, int pipe_padre_a_hijo[2], int pipe_hijo_a_padre[2]) {
    // Limpieza de Pipes
    // El hijo cierra los extremos del pipe que NO usa. 
	// Para P->H, el hijo lee (cierra escritura)
    close(pipe_padre_a_hijo[1]);
    // Para H->P, el hijo escribe (cierra lectura)
    close(pipe_hijo_a_padre[0]);

    // Crear los buffers
    vector<long long> distancias_locales(num_nodos);
    vector<Actualizacion> mis_actualizaciones;

    // Loop principal del hijo:
    // El hijo itera V veces, esperando V "paquetes" de trabajo del padre
    for (int i = 0; i < num_nodos; ++i) {
        // Primero, esperar trabajo (Leer de P->H)
        // El read() es bloqueante. El hijo se duerme aquí hasta que el padre le mande el array de distancias
        ssize_t bytes_leidos = read(pipe_padre_a_hijo[0], distancias_locales.data(), sizeof(long long) * num_nodos);

        // Si read() devuelve 0, significa que el padre cerró el pipe (osea, hay que terminar)
        if (bytes_leidos <= 0) {
            break;
        }

		// Limpiar aristas pasadas
        mis_actualizaciones.clear();

        // El hijo revisa solo su subconjunto de aristas
        for (const auto &arista : mis_aristas) {
            // Chequeo estándar de Bellman-Ford
            if (distancias_locales[arista.origen] != INF &&
                (distancias_locales[arista.origen] + arista.peso < distancias_locales[arista.destino])) {
                // Ojo acá, el hijo NO actualiza su array local. Solo "propone" la actualización al padre
				// El padre es el único que consolida los resultados
                mis_actualizaciones.push_back({
                    arista.destino,
                    distancias_locales[arista.origen] + arista.peso,
                    arista.origen});
            }
        }

        // Ahora hay que reportar (Escribir en H->P)
        // Le decimos al padre cuantas actualizaciones encontramos
        int num_actualizaciones = mis_actualizaciones.size();
        if (write(pipe_hijo_a_padre[1], &num_actualizaciones, sizeof(int)) == -1) {
            // Si padre murió o cerró el pipe, entonces nada que hacer y salimos nomás
            break;
        }

        // Si tenemos actualizaciones, las mandamos
        if (num_actualizaciones > 0)
        {
            if (write(pipe_hijo_a_padre[1], mis_actualizaciones.data(), sizeof(Actualizacion) * num_actualizaciones) == -1) {
                // Misma idea, padre murió, entonces se sale
                break;
            }
        }
        // El hijo vuelve al inicio del loop a esperar más trabajo
    }

    // Cuando se termina, cerramos los pipes restantes y terminamos
    close(pipe_padre_a_hijo[0]);
    close(pipe_hijo_a_padre[1]);

    // exit(0);
}

// Función que reconstruye el string del camino usando el array de predecesores
string reconstruir_camino(int nodo_inicio, int nodo_final, const vector<int>& predecesores) {
    vector<int> camino;
    int actual = nodo_final;

    // Vamos hacia atrás desde el final hasta el inicio
    while (actual != -1) {
        camino.push_back(actual);
        if (actual == nodo_inicio) {
		    break; // Llegamos
		}
        actual = predecesores[actual];
    }

    // Si el loop terminó y no encontramos el inicio, algo raro pasó (o quizás el camino era solo el nodo de inicio)
    if (camino.empty() || camino.back() != nodo_inicio) {
        return "(Camino no reconstruido)";
    }

    // El camino está al revés (ej: 9 <- 2 <- 5), así que se invierte
    reverse(camino.begin(), camino.end());

    // Finalmente crear el string
    stringstream ss;
    for (size_t i = 0; i < camino.size(); ++i) {
        ss << camino[i];
        if (i < camino.size() - 1) {
            ss << " -> ";
        }
    }
    return ss.str();
}

// Función principal (proceso padre)
int main() {
	// Abrir el archivo de salida
    ofstream archivo_salida("data/outputs/salidaFork.txt");
    if (!archivo_salida.is_open()) {
        cerr << "Error al abrir el archivo de salida" << endl;
        return 1;
    }

    ofstream datos_csv("data/measurements/datos_cpp.csv");
    if(!datos_csv.is_open()) {
        cerr << "Error al abrir el archivo de datos de C++" << endl;
        return 1;
    }
    // Header del archivo CSV
    datos_csv << "numero_grafo,exito,numero_nodos,numero_aristas,tiempoMS" << endl;

    // Definir la ruta del directorio de entrada
    const string path_directorio_str = "data/inputs/";
    const fs::path path_directorio(path_directorio_str);

    // Iterar sobre cada archivo dentro del directorio
    for (const auto& entry : fs::directory_iterator(path_directorio)) {
        if (!entry.is_regular_file()) {
            continue;
        }

        // Obtener el nombre del archivo
        string nombre_archivo_entrada = entry.path().string();
        ifstream archivo_entrada(nombre_archivo_entrada);

        if (!archivo_entrada.is_open()) {
            cerr << "Error al abrir el archivo: " << nombre_archivo_entrada << endl;
            continue;
        }

        cout << "Procesando: " << nombre_archivo_entrada << endl;
        auto tiempo_inicio_grafo = chrono::high_resolution_clock::now();

        int num_nodos, nodo_inicio, nodo_final;
        vector<Arista> todas_las_aristas;

        // Leer las especificaciones del grafo
        archivo_entrada >> num_nodos;
        archivo_entrada >> nodo_inicio;
        archivo_entrada >> nodo_final;

        int origen, destino, peso;
        while (archivo_entrada >> origen >> destino >> peso) {
            todas_las_aristas.push_back({origen, destino, peso});
        }
        archivo_entrada.close();

        // Inicialización de Bellman-Ford
        vector<long long> distancias(num_nodos, INF);
        vector<int> predecesores(num_nodos, -1);
        distancias[nodo_inicio] = 0;

        // Repartir las aristas entre los hijos
        vector<vector<Arista>> aristas_por_hijo(NUM_HIJOS);
        for (size_t i = 0; i < todas_las_aristas.size(); ++i) {
            aristas_por_hijo[i % NUM_HIJOS].push_back(todas_las_aristas[i]);
        }

        // Crear pipes
        // Necesitamos N*2 pipes: N para P->H y N para H->P
        int pipes_padre_a_hijo[NUM_HIJOS][2];
        int pipes_hijo_a_padre[NUM_HIJOS][2];
        vector<pid_t> pids_hijos;

        for (int i = 0; i < NUM_HIJOS; ++i) {
            // Crear los dos pipes para este hijo
            if (pipe(pipes_padre_a_hijo[i]) == -1 || pipe(pipes_hijo_a_padre[i]) == -1) {
                perror("pipe");
                exit(EXIT_FAILURE);
            }

            // Crear el proceso hijo
            pid_t pid = fork();
            if (pid == -1) {
                perror("fork");
                exit(EXIT_FAILURE);
            }

            if (pid == 0) {
                // El hijo no necesita la lista de PIDs, recibe sus aristas y se pone a trabajar
                funcion_hijo(aristas_por_hijo[i], num_nodos, pipes_padre_a_hijo[i], pipes_hijo_a_padre[i]);
                // Ojo que la función hijo no debería retornar, si eso pasa salimos nomás
                exit(0);
            }
            else {
                // Codigo del padre post-fork
                pids_hijos.push_back(pid);

                // Cerrar los extremos del pipe que el padre NO usa
                // Para P->H, el padre escribe (cierra lectura)
                close(pipes_padre_a_hijo[i][0]);
                // Para H->P, el padre lee (cierra escritura)
                close(pipes_hijo_a_padre[i][1]);
            }
        }

        string resultado_final = "";
        bool ciclo_negativo_detectado = false;

        // Bellman-Ford itera V veces (V-1 para distancias, 1 para chequear ciclos)
        for (int iter = 0; iter < num_nodos; ++iter) {
            // El padre transmite el estado actual de las distancias a todos los hijos
            for (int i = 0; i < NUM_HIJOS; ++i) {
                // Mandar el array de distancias
                // El write() es atómico y bloqueará si el pipe está lleno, pero el hijo estará esperando para leer, así que no debería haber deadlock (ojalá)
                if (write(pipes_padre_a_hijo[i][1], distancias.data(), sizeof(long long) * num_nodos) == -1){
                    perror("Padre: error escribiendo en pipe P->H");
                }
            }

            // Ahora, el padre espera los resultados (actualizaciones) de TODOS los hijos
            bool hubo_cambio_global = false;
            vector<Actualizacion> todas_las_actualizaciones;

            for (int i = 0; i < NUM_HIJOS; ++i) {
                // Leer cuántas actualizaciones nos manda este hijo
                int num_actualizaciones_hijo = 0;
                if (read(pipes_hijo_a_padre[i][0], &num_actualizaciones_hijo, sizeof(int)) == -1) {
                    perror("Padre: error leyendo de pipe H->P (cantidad)");
                }

                if (num_actualizaciones_hijo > 0) {
                    hubo_cambio_global = true;
                    // Leer ese número de actualizaciones
                    vector<Actualizacion> actualizaciones_hijo(num_actualizaciones_hijo);
                    if (read(pipes_hijo_a_padre[i][0], actualizaciones_hijo.data(), sizeof(Actualizacion) * num_actualizaciones_hijo) == -1) {
                        perror("Padre: error leyendo de pipe H->P (datos)");
                    }
                    // Y agregarlas al lote global
                    todas_las_actualizaciones.insert(todas_las_actualizaciones.end(), actualizaciones_hijo.begin(), actualizaciones_hijo.end());
                }
            }

            // El padre ahora revisa todas las actualizaciones propuestas y aplica la mejor
            for (const auto& actualizacion : todas_las_actualizaciones) {
                if (actualizacion.nueva_distancia < distancias[actualizacion.nodo_actualizado]) {
                    // Si estamos en la última iteración (V), y todavía encontramos mejoras, es porque hay un ciclo negativo (no debería ocurrir con el dataset de Aula, pero igual)
                    if (iter == num_nodos - 1) {
                        ciclo_negativo_detectado = true;
                        // Marcar este nodo como parte de un ciclo
                        predecesores[actualizacion.nodo_actualizado] = -2;
                    }
                    else {
                        distancias[actualizacion.nodo_actualizado] = actualizacion.nueva_distancia;
                        predecesores[actualizacion.nodo_actualizado] = actualizacion.predecesor;
                    }
                }
            }

            // Si en una iteración (que no sea la última) no hubo ningún cambio, entonces ya terminamos
            if (!hubo_cambio_global && iter < num_nodos - 1) {
                // Les decimos a los hijos que vamos a terminar rompiendo el loop. El padre enviará una señal de "terminar"
                break;
            }
        }

        // Una vez terminado el ciclo de Bellman-Ford, matamos a los hijos
        for (int i = 0; i < NUM_HIJOS; ++i){
            // Cerrar los pipes del padre. (Esto debería EOF al read() del hijo, que sabrá que debe terminar)
            close(pipes_padre_a_hijo[i][1]);
            close(pipes_hijo_a_padre[i][0]);
        }

        // Esperar a que todos los hijos terminen (limpieza de zombies)
        for (int i = 0; i < NUM_HIJOS; ++i) {
            waitpid(pids_hijos[i], NULL, 0);
        }

        // Finalmente, procesar y escribir los resultados al archivo
        auto tiempo_fin_grafo = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> duracion_grafo = tiempo_fin_grafo - tiempo_inicio_grafo;

        archivo_salida << "\n★Resultados para " << nombre_archivo_entrada << "★" << endl;
        archivo_salida << "Grafo de " << num_nodos << " nodos. Ruta de " << nodo_inicio << " a " << nodo_final << "." << endl;

        if (ciclo_negativo_detectado) {
            archivo_salida << "Resultado: Se detectó un ciclo de peso negativo" << endl;
        }
        else if (distancias[nodo_final] == INF) {
            archivo_salida << "Resultado: No se encontró un camino" << endl;
        }
        else {
            string camino_str = reconstruir_camino(nodo_inicio, nodo_final, predecesores);
            archivo_salida << "Resultado: Éxito" << endl;
            archivo_salida << "Distancia más corta: " << distancias[nodo_final] << endl;
            archivo_salida << "Camino: " << camino_str << endl;
        }

        archivo_salida << "Tiempo de ejecución ("<< NUM_HIJOS << " hijos" <<"): " << fixed << setprecision(4) << duracion_grafo.count() << " ms." << endl;
    
        //escritura del archivo de datos_csv
        string nombre_archivo = entry.path().filename().string();
        datos_csv << nombre_archivo << ",";
        if (ciclo_negativo_detectado) {
            datos_csv << "ciclo_negativo" << ",";
        }
        else if (distancias[nodo_final] == INF) {
            datos_csv << "sin_camino" << ",";
        }
        else {
            datos_csv << "exito" << ",";
        }
        datos_csv << num_nodos << ","
                  << todas_las_aristas.size() << ","
                  << fixed << setprecision(4) << duracion_grafo.count() << endl;

    }

    // Fin de todos los grafos
    cout << "Procesamiento completado" << endl;
    archivo_salida.close();
    datos_csv.close();

    return 0;
}