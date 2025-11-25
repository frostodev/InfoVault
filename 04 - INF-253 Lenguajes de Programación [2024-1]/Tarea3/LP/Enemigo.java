package LP;

public abstract class Enemigo implements Visible {
    private int vida;
    private int atk;

    private float prob_torso;
    private float prob_pierna;
    private float prob_cabeza;

    private String nombre;

    public Enemigo(String nombre, int vida, int atk, float prob_torso, float prob_pierna, float prob_cabeza) {
        this.nombre = nombre;
        this.vida = vida;
        this.atk = atk;
        this.prob_torso = prob_torso;
        this.prob_pierna = prob_pierna;
        this.prob_cabeza = prob_cabeza;
    } 

    ////////////////////////////////////////////////////
    //                   GETTERS                      //
    public int getVida() {
        return this.vida;
    }

    public int getAtk() {
        return this.atk;
    }

    public String getNombre() {
        return this.nombre;
    }

    public float getProbTorso() {
        return this.prob_torso;
    }

    public float getProbPierna() {
        return this.prob_pierna;
    }

    public float getProbCabeza() {
        return this.prob_cabeza;
    }
    
    ////////////////////////////////////////////////////
    //                   SETTERS                      //
    public void setVida(int vida) {
        this.vida = vida;
    }

    public void setAtk(int atk) {
        this.atk = atk;
    }

    public void setNombre(String nombre) {
        this.nombre = nombre;
    }

    public void setProbTorso(float probTorso) {
        this.prob_torso = probTorso;
    }

    public void setProbPierna(float probPierna) {
        this.prob_pierna = probPierna;
    }

    public void setProbCabeza(float probCabeza) {
        this.prob_cabeza = probCabeza;
    }
}
