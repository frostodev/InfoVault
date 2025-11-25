package LP;

import java.util.Random;

public abstract class Arma {
    private String nombre;
    private int dano;
    private float precision;

    public Arma (String nombre, int dano, float precision) {
        /*
        *   Nombre: Arma
        *   Parámetros: String nombre, int dano, float precision
        *   Retorno: void
        *   Resumen: Constructor de la clase
        */

        this.nombre = nombre;
        this.dano = dano;
        this.precision = precision;
    }

    public float calcularDano(float prob_hit) {
        /*
        *   Nombre: calcularDano
        *   Parámetros: float prob_hit
        *   Retorno: float
        *   Resumen: Método que calcula el daño al enemigo
        */

        Random rand = new Random();

        float r = rand.nextFloat();
        if (r < prob_hit * this.precision) {
            return (float) dano;
        } else {
            return 0.0f;
        }
    }

    public void detonarBomba(Arena arena) {
        /*
        *   Nombre: detonarBomba
        *   Parámetros: Arena arena
        *   Retorno: void
        *   Resumen: Método que inicializa una nueva ronda
        */

        // Eliminar a todos los enemigos del mapa
        for (int i = 0; i < arena.getMapa().size(); i++) {

            // Verificar si en la casilla hay un enemigo
            if (arena.getMapa().get(i).getRepresentacion() == 'H' || 
                arena.getMapa().get(i).getRepresentacion() == 'K' || 
                arena.getMapa().get(i).getRepresentacion() == 'R') {

                    // En cuyo caso, eliminarlo
                    arena.eliminarCasilla(i);
            }
        }
    }

    ////////////////////////////////////////////////////
    //                   SETTERS                      //
    public void setNombre(String nombre) {
        this.nombre = nombre;
    }

    public void setPrecision(float precision) {
        this.precision = precision;
    }

    public void setDano(int dano) {
        this.dano = dano;
    }

    ////////////////////////////////////////////////////
    //                   GETTERS                      //
    public final String getNombre() {
        return nombre;
    }

    public float getPrecision() {
        return this.precision;
    }

    public int getDano() {
        return this.dano;
    }
}
