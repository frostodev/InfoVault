// bibliotecas estandares
#include <iostream>
#include <cstdlib>

//bibliotecas relacionadas a procesos / fork
#include <unistd.h> // fork, getpid, getppid
#include <sys/wait.h> // wait, waitpid
#include <sys/shm.h> // memoria compartida
#include <sys/ipc.h> // operaciones IPC
 
#define endl '\n'
using namespace std;

// Estructura de los reportes de cada equipo
struct ReporteEquipo {
    int id_equipo;
    int recursos_recolectados;
    int estado; // 0 = fracaso, 1 = exito parcial, 2 = exito total
    int pid; // PID del proceso
    
};

// Estructura principal de memoria compartida
struct MemoriaCompartida {
    ReporteEquipo reportes[4];  // Reportes de los 4 equipos
    int recursos_totales[4];  // [agua, alimentos, construccion, señales]
    int moral;
    int dia_actual;
    bool juego_activo;
};

int main() {
    // Creacion de memoria compartida
    key_t clave = ftok("supervivencia", 65);
    int shmid = shmget(clave, sizeof(MemoriaCompartida), 0666 | IPC_CREAT);
    MemoriaCompartida* memoria = (MemoriaCompartida*) shmat(shmid, NULL, 0);
    
    // Inicializar memoria
    memoria->moral = 100;
    memoria->dia_actual = 1;
    memoria->juego_activo = true;
    for (int i = 0; i < 4; i++) {
        memoria->recursos_totales[i] = 0;
        memoria->reportes[i].id_equipo = -1; // -1 = no ha reportado
    }
    cout << "\t===================================" << endl;
    cout << "\t=== SIMULACIÓN DE SUPERVIVENCIA ===" << endl;
    cout << "\t===================================\n" << endl;

    //solicitud de limite de dias a simular
    int dias_limite = 0;
    do{
        cout << "Ingrese la cantidad de días a simular: " << endl;
        cout << "VALOR DEBE ESTAR ENTRE 10 Y 30" << endl;
        cout << "\n>> ";
        cin >> dias_limite;
    } while(dias_limite < 10 || dias_limite > 30);

    // minimos diarios por equipo
    const int MIN_AGUA = 8;
    const int MIN_ALIMENTOS = 12;
    const int MIN_CONSTRUCCION = 4;
    const int MIN_SENALES = 2;

    int dias_consecutivos_senales = 0;

    // Interfaz y operacion de la tarea
    cout << "\nIniciando simulacion de " << dias_limite << " dias..." << endl;
    int dia = 1;
    while(dia <= dias_limite) {
        cout << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        cout << "\n\t★=== DIA " << dia << " DE SUPERVIVENCIA ===★" << endl;
        cout << "Iniciando equipos de recoleccion..." << endl;
        cout << endl;

        memoria->dia_actual = dia; // guardar el dia actual
        string nombres_equipo[4]  = {"AGUA", "ALIMENTOS", "CONSTRUCCIÓN", "SEÑALES"};

        // creacion de los 4 equipos / procesos / hijos
        int pids[4];
        for(int equipo = 0; equipo < 4; equipo++) {
            pids[equipo] = fork();

            if(pids[equipo] == 0) {

                if(equipo == 0) { // equipo agua

                    cout << "[EQUIPO " << nombres_equipo[equipo] << " - PID: " << getpid() <<"] Saliendo de la base, iniciando exploración..." << endl;  
                    sleep(1);
                    cout << "[EQUIPO " << nombres_equipo[equipo] << " - PID: " << getpid() <<"] Exploración finalizada, regresando a base..." << endl;
                    sleep(1);

                } else if(equipo == 1) { // equipo alimentos

                    cout << "[EQUIPO " << nombres_equipo[equipo] << " - PID: " << getpid() <<"] Saliendo de la base, iniciando exploración..." << endl;
                    sleep(1);
                    cout << "[EQUIPO " << nombres_equipo[equipo] << " - PID: " << getpid() <<"] Exploración finalizada, regresando a base..." << endl;
                    sleep(1);

                } else if(equipo == 2) { // equipo construccion

                    cout << "[EQUIPO " << nombres_equipo[equipo] << " - PID: " << getpid() <<"] Saliendo de la base, iniciando exploración..." << endl;
                    sleep(1);
                    cout << "[EQUIPO " << nombres_equipo[equipo] << " - PID: " << getpid() <<"] Exploración finalizada, regresando a base..." << endl;
                    sleep(1);

                } else if (equipo == 3) { // equipo señales
                    cout << "[EQUIPO " << nombres_equipo[equipo] << " - PID: " << getpid() <<"] Saliendo de la base, iniciando exploración..." << endl;
                    sleep(1);
                    cout << "[EQUIPO " << nombres_equipo[equipo] << " - PID: " << getpid() <<"] Exploración finalizada, regresando a base..." << endl;
                    sleep(1);
                }

                // --- logica de cada hijo ---
                srand(time(NULL) ^ (getpid()<<16));

                int objetivo = 0;
                switch (equipo) {
                    case 0: objetivo = 8; break; // agua
                    case 1: objetivo = 12; break; // alimentos
                    case 2: objetivo = 4; break; // construccion
                    case 3: objetivo = 2; break; // señales
                }

                int r = rand() % 100;
                int estado = 0;
                int produccion = 0;
                int porcentaje = 0;

                if (r < 30) {
                    // Exito total 
                    porcentaje = 100 + (rand() % 21);
                    produccion = (objetivo * porcentaje)/100;
                    estado = 2;
                } else if (r < 80) {
                    // Exito parcial 
                    porcentaje = 50 + (rand() % 31);  
                    produccion = (objetivo * porcentaje)/100; 
                    estado = 1;
                } else {
                    // Fracaso 
                    porcentaje = rand() % 30;  
                    produccion = (objetivo * porcentaje)/100; 
                    estado = 0;
                }

                // escribir en memoria compartida en la ranura correspondiente
                memoria->reportes[equipo].id_equipo = equipo + 1;
                memoria->reportes[equipo].recursos_recolectados = produccion;
                memoria->reportes[equipo].estado = estado;
                memoria->reportes[equipo].pid = getpid();
                exit(0); // eliminacion de los hijos
            }

        }

        // el padre espera a que los 4 hijos mueran
        for(int i = 0; i < 4; i++) {
            wait(NULL);
        }

        // --- logica del padre ---
        cout << "\n\t== REPORTES FINALES ==" << endl;
        cout << "\t======================" << endl << endl;
        for (int i = 0; i < 4; i++) {
            ReporteEquipo rep = memoria->reportes[i];
            cout << "- Equipo " << nombres_equipo[i] << " completó ciclo: " << rep.recursos_recolectados << " unidades obtenidas" << endl;
            memoria->recursos_totales[i] = rep.recursos_recolectados;
        }

        // Mostrar resultados del dia y evaluar minimos
        cout << "\n\t== RESULTADOS DEL DÍA ==" << endl;
        cout << "\t========================" << endl << endl;

        cout << "- Agua recolectada: " << memoria->recursos_totales[0] << "/" << MIN_AGUA;
        cout << (memoria->recursos_totales[0] >= MIN_AGUA ? " (SUFICIENTE)" : " (INSUFICIENTE)") << endl;

        cout << "- Alimentos obtenidos: " << memoria->recursos_totales[1] << "/" << MIN_ALIMENTOS;
        cout << (memoria->recursos_totales[1] >= MIN_ALIMENTOS ? " (SUFICIENTE)" : " (INSUFICIENTE)") << endl;

        cout << "- Materiales de construcción: " << memoria->recursos_totales[2] << "/" << MIN_CONSTRUCCION;
        cout << (memoria->recursos_totales[2] >= MIN_CONSTRUCCION ? " (SUFICIENTE)" : " (INSUFICIENTE)") << endl;

        cout << "- Señales mantenidas: " << memoria->recursos_totales[3] << "/" << MIN_SENALES;
        cout << (memoria->recursos_totales[3] >= MIN_SENALES ? " (SUFICIENTE)" : " (INSUFICIENTE)") << endl;

        // Evaluación de moral
        // regla: -5 puntos por cada equipo que no cumpla el minimo
        int faltas = 0;
        if (memoria->recursos_totales[0] < MIN_AGUA) faltas++;
        if (memoria->recursos_totales[1] < MIN_ALIMENTOS) faltas++;
        if (memoria->recursos_totales[2] < MIN_CONSTRUCCION) faltas++;
        if (memoria->recursos_totales[3] < MIN_SENALES) faltas++;

        if (faltas == 0) {
            cout << endl << "Estado: SUPERVIVENCIA ESTABLE (todos los recursos alcanzados)" << endl;
        } else {
            cout << endl << "Estado: SUPERVIVENCIA CRÍTICA (faltan " << faltas << " categorias)" << endl;
        }

        int penalizacion_por_categoria = 5;
        int perdida_moral = faltas * penalizacion_por_categoria;
        memoria->moral -= perdida_moral;
        if (memoria->moral < 0) memoria->moral = 0;

        cout << endl << "Moral del grupo: " << memoria->moral << "/100 (";
        if (perdida_moral > 0) cout << "-" << perdida_moral << " por recursos insuficientes)";
        else cout << "sin cambios)";
        cout << endl;

        // seguimiento de señales para condición de rescate (10 dias consecutivos con señales >= MIN_SENALES)
        if (memoria->recursos_totales[3] >= MIN_SENALES) {
            dias_consecutivos_senales++;
        } else {
            dias_consecutivos_senales = 0;
        }

        // condiciones de termino (rescate, moral 0) y finalización por días
        if (dias_consecutivos_senales >= 10) {
            cout << endl << "★== ¡Rescate exitoso! Mantuvieron señales durante 10 días consecutivos. ==★" << endl;
            memoria->juego_activo = false;
            break;
        }

        if (memoria->moral <= 0) {
            cout << endl << "★== La moral llego a 0. Simulación terminada en el día " << dia << "/" << dias_limite << " por fracaso del grupo ==★" << endl;
            memoria->juego_activo = false;
            break;
        }

        dia++;
    }

    if (dia > dias_limite) {
        cout << endl << "★== Se alcanzó el límite de días (" << dias_limite << ") ==★" << endl;
    }

    cout << "\n\t=============================" << endl;
    cout << "\t=== SIMULACIÓN FINALIZADA ===" << endl;
    cout << "\t=============================" << endl;

    // Limpiar memoria compartida
    shmdt(memoria);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
