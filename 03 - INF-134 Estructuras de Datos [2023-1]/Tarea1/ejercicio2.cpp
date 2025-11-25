#include <iostream>
#include <cstring>

using namespace std;

struct Persona {
    string nombre;
    char fecha[11];
    int tamanio_tarjeta;
    int* tarjeta;
    bool quiere_intercambiar;
};

/*****
*  int* comprarTarjeta()
******
*  Recibe dos parámetros el nombre de la persona que quiere comprar, el día en que se 
*  compra la tarjeta y el tamaño del arreglo que se va a retornar. Debe retornar un arreglo 
*  de enteros de igual tamaño que el nombre, dónde en la posición i se encontrará el valor nombre[i] %dia.
******
*  Input:
*  string nombre: nombre de la persona que quiere comprar
*  int dia: el día en que se compra la tarjeta
*  int &m: el tamaño del arreglo que se va a retornar
******
*  Returns:
*  arreglo, un arreglo de enteros de igual tamaño que el nombre, dónde en la posición i
*  se encontrará el valor nombre[i]%dia
*****/
int* comprarTarjeta(string nombre, int dia, int &m) {
  int* arregloEnteros = new int[m]; // Crear el arreglo de enteros
  for (int i = 0; i < m; i++) {// Ciclo for para recorrer cada posicion del arreglo
    arregloEnteros[i] = nombre[i] % dia; // Dar el valor correspondiente a la ienesima posicion del arreglo
  }
  return arregloEnteros;
}

/*****
* void intercambiarTarjeta()
******
* Recibe 2 personas como parametros y se intercambian sus tarjetas
******
* Input:
* Persona* p1: persona 1 que quiere intercambiar
* Persona* p2: persona 2 que quiere intercambiar
******
* Returns: No hay
*****/
void intercambiarTarjeta(Persona* p1, Persona* p2) {
  int* temporal; // Crear una variable temporal
  temporal = p1 -> tarjeta; // Almacenar el valor de la tarjeta de p1 en la variable temporal
  p1 -> tarjeta = p2 -> tarjeta; // Almacenar el valor de la tarjeta de p2 en la tarjeta de p1
  p2 -> tarjeta = temporal; // Almacenar el valor de la variable temporal en la tarjeta de p2
}

/*****
* int puntaje()
******
* Recibe una persona y debe retornar un entero correspondiente al puntaje de la persona
******
* Input:
* Persona* p1: persona a la cual se le va a calcular el puntaje
******
* Returns:
* int, se retorna el puntaje de la persona ingresada
*****/
int puntaje(Persona* p1) {
  int puntajeTotal = 0; // Variable donde se va a calcular el puntaje de la persona
  for (unsigned int i = 0; i <= p1 -> nombre.size() - 1 ; i++) {  // Ciclo for para calcular la sumatoria   
    puntajeTotal += (p1 -> tarjeta[i]) * (p1 -> fecha[i % 10]); // Operatoria interior de la sumatoria
  }
  return puntajeTotal;
}

/*****
* Persona* unDia
******
* Recibe un arreglo de personas, luego hacer tantos intercambios de tarjetas como sea posible y
* debe retornar a la persona que haya obtenido el mayor puntaje
******
* Input:
* Persona* personas:  arreglo de personas a trabajar
* int dia: parametro dia para hacer la compra de tarjetas
******
* Returns:
* struct, la persona que haya obtenido el mayor puntaje
*****/
Persona* unDia(Persona* personas, int dia) {
  // Calcular longitud del arreglo
  int longitudArreglo = (sizeof(personas -> nombre)/sizeof(personas));
  for (int i = 0; i < longitudArreglo; i++) {
    // Comprar las tarjetas para cada persona
    int m = personas[i].nombre.size();
    personas[i].tarjeta = comprarTarjeta(personas[i].nombre, dia, m);
  }
  for (int j = 0; j < longitudArreglo; j++) {
    // Si nuestra persona quiere intercambiar
    if (personas[j].quiere_intercambiar) {
      // Buscar otra persona
      for (int m = 0; m < longitudArreglo; m++) {
        // Si esta otra también quiere
        if (personas[m].quiere_intercambiar) {
          // Intercambiar las tarjetas
          intercambiarTarjeta(&personas[j], &personas[m]);
        }
      }
    }
  }
  int puntajeMayor = 0, puntajeActual = 0, personaMejor= 0; 
  for(int f = 0; f<longitudArreglo;f++){ // Ciclo for para encontrar el mayor puntaje
    puntajeActual = puntaje(&personas[f]);
    if(puntajeActual>puntajeMayor){ // Condicion para modificar el puntaje mayor
      puntajeMayor=puntajeActual;
      personaMejor=f;
    }
  }
  return &(personas[personaMejor]);
}

/*****
* void variosDias
******
* recibe un arreglo de personas y la cantidad de dias en total a realizar y por cada día, 
* debe mostrar por pantalla el nombre, fecha de nacimiento y puntaje de la persona que obtuvo el mayor ese día
******
* Input:
* Persona* personas: arreglo de personas a trabajar
* int cant_dias: cantidad de dias en total a mostrar un ganador
******
* como es una función void, no retorna nada
*****/
void variosDias(Persona* personas, int cant_dias) {
  for (int i = 1; i <= cant_dias; i++) { // Ciclo for para recorrer cada dia
    Persona *z = unDia(personas, i); // llamar funcion unDia() para saber el ganador de ese dia
    cout << z -> nombre << " " << z -> fecha << " " << puntaje(z) << endl; // imprimir el nombre, dia de nacimiento y puntaje del ganador del dia
  }
}

int main(){
    cout<< "Ingrese la cantidad de personas"<<endl;
    int cantidad_de_personas;
    // Se crea un tamaño del arreglo basado en la cantidad de personas que ingreese el usuario
    cin>> cantidad_de_personas;
    //Se pide la cantidad de días que durará el juego
    int cant_dias;
    cout<<"Ingrese la cantidad de días"<<endl;
    cin>>cant_dias;
    //Se define un arreglo del struct para guardar las entradas del usuario
    Persona lista[cantidad_de_personas];
    cout<< "Ingrese el nombre de la persona, fecha de nacimiento y si quiere intercambiar"<<endl;
    //Ciclo que se encarga de guardar la cantidad de personas en un arreglo del Struct
    for (int i=0; i<cantidad_de_personas; i++){
        cin>>lista[i].nombre;
        cin>>lista[i].fecha;
        cin>>lista[i].quiere_intercambiar;
    }
    variosDias(lista, cant_dias);
  return 0;
}
