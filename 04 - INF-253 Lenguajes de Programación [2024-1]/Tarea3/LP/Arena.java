package LP;
import java.util.ArrayList;
import java.util.Random;

public class Arena {

    private int ronda;
    private int tamano;
    private ArrayList<Visible> mapa;

    public Arena(int tamano) {
        /*
        *   Nombre: Arena
        *   Parámetros: int tamano
        *   Retorno: void
        *   Resumen: Constructor de la clase
        */
                
        this.ronda = 0;
        this.tamano = tamano;
        this.mapa = new ArrayList<>(tamano);

        // Rellenar el mapa con elemento vacío
        Vacio vacio = new Vacio();
        for (int i = 0; i < this.tamano; i++) {
            mapa.add(vacio);
        } 
    }

    public void mostrar() {
        /*
        *   Nombre: mostrar
        *   Parámetros: void
        *   Retorno: void
        *   Resumen: Método que muestra el mapa en pantalla
        */
        
        for (Visible v : mapa) {

            // Omitir los vacíos
            if (v.getRepresentacion() == 'V') {
                System.out.print(" ");
            } else {
                System.out.print(v.getRepresentacion());     
            }

            System.out.print(" | ");
        }
        System.out.println("");
    }

    public void nuevaRonda(Terminal terminal, Jugador jugador) {
        /*
        *   Nombre: nuevaRonda
        *   Parámetros: Terminal terminal, Jugador jugador
        *   Retorno: void
        *   Resumen: Método que inicializa una nueva ronda
        */

        // Eliminar los contenidos de la ronda anterior
        Vacio vacio = new Vacio();
        for (int i = 0; i < this.tamano; i++) {
            mapa.set(i, vacio);
        } 

        // Aumentar el contador de rondas
        this.ronda++;
        
        // Añadir la terminal
        mapa.set(7, terminal);

        // Añadir al jugador
        mapa.set(8, jugador);
        jugador.setPos(8);

        // Añadir los enemigos
        for (int i = 0; i < 5; i++) {
            Random rand = new Random();

            // Generar una posición aleatoria
            int posRandom;
            do {
                posRandom = rand.nextInt(14) + 1;
            } while (posRandom == 7 || posRandom == 8); // Si es 7, generar otro número

            // Determinar el enemigo de forma aleatoria
            char[] chars = {'K', 'H', 'R'};
            int enemigoIndex = rand.nextInt(chars.length);
            char enemigoRandom = chars[enemigoIndex];

            if (enemigoRandom == 'K') {
                Enemigo tanker = new Tanker();
                mapa.set(posRandom, tanker);

            } else if (enemigoRandom == 'H') {
                Enemigo husk = new Husk();
                mapa.set(posRandom, husk);

            } else if (enemigoRandom == 'R') {
                Enemigo runner = new Runner();
                mapa.set(posRandom, runner);
            }
        }

        // Además, para evitar conflictos al interactuar con una terminal, eliminar el enemigo de la posición 6
        mapa.set(6, vacio);
    }

    public boolean verificarEnemigos() {
        /*
        *   Nombre: verificarEnemigos
        *   Parámetros: void
        *   Retorno: boolean
        *   Resumen: Método que verifica si existen enemigos en el mapa
        */

        ArrayList<Visible> listaEnemigos = new ArrayList<>();

        // Añadir enemigos existentes a la lista
        for (int i = 0; i < mapa.size(); i++) {
            if (mapa.get(i).getRepresentacion() != 'V' && mapa.get(i).getRepresentacion() != 'J' && mapa.get(i).getRepresentacion() != 'T') {
                listaEnemigos.add(mapa.get(i));
            }
        }

        // Existen enemigos
        if (listaEnemigos.size() != 0) {
            return true;
        
        // No hay enemigos en el mapa
        } else {
            return false;
        }
    }

    public void eliminarCasilla(int pos) {
        /*
        *   Nombre: eliminarCasilla
        *   Parámetros: int pos
        *   Retorno: void
        *   Resumen: Método que elimina una casilla
        */

        Vacio vacio = new Vacio();
        mapa.set(pos, vacio);
    }

    ////////////////////////////////////////////////////
    //                   GETTERS                      //
    public int getRonda() {
        return this.ronda;
    }

    public int getTamano() {
        return this.tamano;
    }

    public ArrayList<Visible> getMapa() {
        return this.mapa;
    }
    
    ////////////////////////////////////////////////////
    //                   SETTERS                      //
    public void setRonda(int ronda) {
        this.ronda = ronda;
    }

    public void setMapa(ArrayList<Visible> mapa) {
        this.mapa = mapa;
    }

}