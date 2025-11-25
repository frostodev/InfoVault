#include <iostream>
#include <string>

using namespace std;

// Los elementos del heap 
typedef string tipoElem;

// Definición de la estructura de comando
struct Command {
    string id;
    int priority;
    string instruction;
    Command* next;
};

// Clase para gestionar los comandos pendientes
class tColaP {
private:

    // Crear el elemento principal del heap
    Command* head;

public:

    // Constructor del Heap
    tColaP() {
      head = NULL;
    }

    // Destructor del heap
    ~tColaP() {
        while (head != NULL) {
            Command* temp = head;
            head = head->next;
            delete temp;
        }
    }

    /*****
    * void push()
    ******
    * Crea un nuevo comando a ser enviado a los dispositivos
    ******
    * Input:
    * const string& id: es el identificador unico del comando
    * int priority: es la prioridad del comando
    * const string& instruction: es la instruccion a ejecutar
    ******
    * Returns:
    * No hay retorno (función void)
    *****/
    void push(const string& id, int priority, const string& instruction) {
      
      // Crear un nuevo objeto Command en el heap y asignar los valores proporcionados
      Command* newCommand = new Command();
      newCommand->id = id;
      newCommand->priority = priority;
      newCommand->instruction = instruction;
      newCommand->next = NULL;
  
      if (head == NULL) {
          // Si la lista está vacía, establecer el nuevo comando como la cabeza de la lista
          head = newCommand;
      } else {
          Command* curr = head;
          Command* prev = NULL;
          // Recorrer la lista para encontrar la posición correcta para insertar el nuevo comando
          while (curr != NULL && curr->priority < priority) {
              prev = curr;
              curr = curr->next;
          }
            if (prev == NULL) {
                // Si el nuevo comando tiene la prioridad más alta, se convierte en la nueva cabeza
                newCommand->next = head;
                head = newCommand;
            } else {
              // Insertar el nuevo comando entre el comando anterior y el comando actual
              prev->next = newCommand;
              newCommand->next = curr;
          }
      }
  }

    /*****
    * void get()
    ******
    * Obtiene N comandos del heap
    ****** 
    * Input:
    * int n: numero de comandos a ser ejecutados
    ******
    * Returns:
    * No hay retorno (función void)
    *****/
    void get(int n) {
      // Apuntar al inicio de la lista
      Command* curr = head;
  
      if (curr == NULL) {
          // Si la lista está vacía, imprimir 0 y regresar
          cout << 0 << endl;
          return;
      }
  
      // Imprimir el valor de n
      cout << n;
  
      // Recorrer la lista y imprimir los ids de los comandos
      for (int i = 0; i < n && curr != NULL; i++) {
          cout << " " << curr->id;
          curr = curr->next;
      }
      cout << endl;
  
      // Reiniciar el puntero al inicio de la lista
      curr = head;
  
      // Recorrer la lista y imprimir las instrucciones de los comandos
      for (int i = 0; i < n && curr != NULL; i++) {
          cout << curr->instruction << endl;
          curr = curr->next;
      }
  
      // Eliminar los primeros n comandos de la lista
      for (int i = 0; i < n && head != NULL; i++) {
          Command* temp = head;
          head = head->next;
          delete temp;
      }
    }

    // Obtener la cantidad de comandos pendientes en el heap
    int sizeColaP() {
        int count = 0;
        Command* curr = head;
        while (curr != NULL) {
            count++;
            curr = curr->next;
        }
        return count;
    }
/*****
* int send
******
* Obtener la cantidad de comandos enviados y eliminarlos del heap
****** 
* Input: 
* int n: Número que acompaña al comando GET
******
* retorna un contador de comandos enviados
*****/
  int send(int n) {
      // Contador para el número de comandos enviados
      int sentCount = 0;
  
      // Apuntar al inicio de la lista
      Command* curr = head;
  
      // Puntero al comando anterior
      Command* prev = NULL;
  
      // Recorrer la lista y eliminar los comandos hasta alcanzar el límite n
      while (curr != NULL && sentCount < n) {
          prev = curr;
          curr = curr->next;
          delete prev;
          sentCount++;
      }
  
      // Actualizar la head de la lista para apuntar al siguiente comando después de los enviados
      head = curr;
  
      // Retornar el número de comandos enviados
      return sentCount;
  }
};

int main() {
    // Crear un heap
    tColaP heap;
    int createdCount = 0;
    int contador = -1;
    string input;

    while (getline(cin, input)) {
        // Obtener el tipo de comando de la entrada
        string commandType = input.substr(0, input.find(' '));

        // Comando PUSHCOMMAND
        if (commandType == "PUSHCOMMAND") {
            string id, instruction;
            int priority;

            // Encontrar las posiciones de los espacios en blanco para separar los elementos
            unsigned int pos1 = input.find(' ') + 1;
            unsigned int pos2 = input.find(' ', pos1);
            unsigned int pos3 = input.find(' ', pos2 + 1);

            // Extraer los valores de id, priority e instruction de la entrada
            id = input.substr(pos1, pos2 - pos1);
            priority = stoi(input.substr(pos2 + 1, pos3 - pos2 - 1));
            instruction = input.substr(pos3 + 1);

            // Agregar el comando a la cola de prioridad
            heap.push(id, priority, instruction);
            createdCount++;

            // Imprimir el tamaño de la cola de prioridad y el estado "PENDING"
            cout << heap.sizeColaP() << " PENDING" << endl;
            contador++;

        // Comando GET
        } 
        else if (commandType == "GET") {
            // Obtener el valor de n de la entrada
          int n = stoi(input.substr(input.find(' ') + 1));

            // Obtener y mostrar los comandos de la cola de prioridad
          heap.get(n);

        // Comando TERMINATE
        } 
        else if (commandType == "TERMINATE") {
            // Mostrar el número de comandos procesados y creados
          cout << contador << " SENT" << endl;
          cout << createdCount << " CREATED" << endl;
          break;
        }
    }

    return 0;
}