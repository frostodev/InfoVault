package LP;
import java.util.ArrayList;

public class Jugador implements Visible {

    private int vida;
    private int energia;
    private ArrayList<Arma> armas;
    private int P_Points;

    private String nombre;
    private int kills;
    private int puntuacion;
    private int pos;

    public Jugador(int vida, int energia, int P_Points, String nombre) {
        /*
        *   Nombre: Jugador
        *   Parámetros: int vida, int energia, int P_Points, String nombre
        *   Retorno: void
        *   Resumen: Constructor de la clase
        */

        this.vida = vida;
        this.energia = energia;
        this.P_Points = P_Points;
        this.armas = new ArrayList<>();
        this.P_Points = P_Points;
        this.nombre = nombre;
        this.kills = 0;
        this.puntuacion = 0;
        this.pos = 8;

        // Añadir el revolver default
        armas.add(new Revolver());
    }

    public boolean Disparar(Enemigo enemigo, Arma arma, Arena arena, int zona) {
        /*
        *   Nombre: Disparar
        *   Parámetros: Enemigo enemigo, Arma arma, Arena arena, int zona
        *   Retorno: boolean
        *   Resumen: Método que maneja el disparo a un enemigo. 
        */

        // Revolver
        if (arma instanceof Revolver) {
            // Calcular el daño producido
            int danoProducido = (int) arma.calcularDano(zona);
        
            // Ajustar la vida del enemigo
            int vidaEnemigo = enemigo.getVida();
            enemigo.setVida(vidaEnemigo - danoProducido);
        }

        // Escopeta
        else if (arma instanceof Escopeta) {
            // Calcular el daño producido
            // La escopeta tiene 6 perdigones
            int danoProducido = 6 * (int) arma.calcularDano(zona);
        
            // Ajustar la vida del enemigo
            int vidaEnemigo = enemigo.getVida();
            enemigo.setVida(vidaEnemigo - danoProducido);
        }

        // Bomba nuclear
        else if (arma instanceof Bomba) {

            // Detonar la bomba
            arma.detonarBomba(arena);

            // Eliminar la bomba de la lista de armas
            ArrayList<Arma> listaJ = this.armas;
            for (int i = 0; i < listaJ.size(); i++) {
                if (listaJ.get(i) instanceof Bomba) {
                    listaJ.remove(i);
                }
            }

            // Actualizar la lista de armas
            this.armas = listaJ;

            // Además, eliminar al enemigo actual
            enemigo.setVida(0);
            return true;
        }

        return true;

    }

    public Visible moverse(int direccion, Arena arena) {
        /*
        *   Nombre: moverse
        *   Parámetros: int direccion, Arena arena
        *   Retorno: Visible
        *   Resumen: Método que maneja el movimiento del jugador
        */
        
        // Izquierda
        if (direccion == 0) {
            // Revisar la posición adyacente
            // Primero revisar si no es vacía, en este caso devolver el tipo de entidad en esa casilla
            if (arena.getMapa().get(this.pos - 1).getRepresentacion() != 'V' ) {

                // Si hay una terminal
                if (arena.getMapa().get(this.pos - 1).getRepresentacion() == 'T') {
                    // Avanzar y retornar el objeto terminal
                    Vacio vacio = new Vacio();
                    ArrayList<Visible> mapaModificado = arena.getMapa();
                    mapaModificado.set(this.pos - 2, this);
                    mapaModificado.set(this.pos, vacio);
                    this.pos = this.pos - 2;
    
                    arena.setMapa(mapaModificado);

                    return (Terminal) arena.getMapa().get(7);

                // Si hay un enemigo
                } else {
                    return (Enemigo) arena.getMapa().get(this.pos - 1);
                }

            // En caso de que no haya nada, simplemente cambiar la posición del jugador
            } else {
                // Primero crear un vacío, luego modificar el mapa con los cambios
                Vacio vacio = new Vacio();
                ArrayList<Visible> mapaModificado = arena.getMapa();
                mapaModificado.set(this.pos - 1, this);
                mapaModificado.set(this.pos, vacio);
                this.pos = this.pos - 1;

                arena.setMapa(mapaModificado);
                    
                // Crear un enemigo nulo para retornar
                EnemigoNulo nulo = new EnemigoNulo();
                return nulo;
            }

        // Derecha
        } else if (direccion == 1) {
            // Revisar la posición adyacente
            // Primero revisar si no es vacía, en este caso devolver el tipo de entidad en esa casilla
            if (arena.getMapa().get(this.pos + 1).getRepresentacion() != 'V' ) {

                // Si hay una terminal
                if (arena.getMapa().get(this.pos + 1).getRepresentacion() == 'T') {
                    // Avanzar y retornar el objeto terminal
                    Vacio vacio = new Vacio();
                    ArrayList<Visible> mapaModificado = arena.getMapa();
                    mapaModificado.set(this.pos + 2, this);
                    mapaModificado.set(this.pos, vacio);
                    this.pos = this.pos + 2;
    
                    arena.setMapa(mapaModificado);

                    return (Terminal) arena.getMapa().get(7);
                
                // Si hay un enemigo
                } else {
                    return (Enemigo) arena.getMapa().get(this.pos + 1);
                }

                // En caso de que no haya nada, simplemente cambiar la posición del jugador
                } else {
                    // Primero crear un vacío, luego modificar el mapa con los cambios
                    Vacio vacio = new Vacio();
                    ArrayList<Visible> mapaModificado = arena.getMapa();
                    mapaModificado.set(this.pos + 1, this);
                    mapaModificado.set(this.pos, vacio);
                    this.pos = this.pos + 1;
    
                    arena.setMapa(mapaModificado);
                    
                    // Crear un enemigo nulo para retornar
                    EnemigoNulo nulo = new EnemigoNulo();
                    return nulo;
                }
        }
        return null;
    }

    ////////////////////////////////////////////////////
    //                   GETTERS                      //
    public int getVida() {
        return this.vida;
    }

    public int getEnergia() {
        return this.energia;
    }

    public int getP_Points() {
        return P_Points;
    }

    public ArrayList<Arma> getArmas() {
        return armas;
    }

    public String getNombre() {
        return this.nombre;
    }

    public int getKills() {
        return this.kills;
    }

    public int getPuntuacion() {
        return this.puntuacion;
    }

    public int getPos() {
        return this.pos;
    }

    public char getRepresentacion() {
        return 'J';
    }
    
    ////////////////////////////////////////////////////
    //                   SETTERS                      //
    public void setVida(int vida) {
        this.vida = vida;
    }

    public void setEnergia(int energia) {
        this.energia = energia;
    }

    public void setP_Points(int p_points) {
        this.P_Points = p_points;
    }

    public void setPuntuacion(int puntuacion) {
        this.puntuacion = puntuacion;
    }

    public void setPos(int pos) {
        this.pos = pos;
    }

    public void setArmas(ArrayList<Arma> armas) {
        this.armas = armas;
    }

    public void setKills(int Kills) {
        this.kills = Kills;
    }
}

