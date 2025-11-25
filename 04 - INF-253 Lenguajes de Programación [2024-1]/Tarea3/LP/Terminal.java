package LP;

import java.util.ArrayList;

public class Terminal implements Visible {

    private ArrayList<Arma> armas_disponibles;

    public Terminal() {
        /*
        *   Nombre: Terminal
        *   Parámetros: void
        *   Retorno: void
        *   Resumen: Constructor de la clase
        */

        // Añadir las armas disponibles
        this.armas_disponibles = new ArrayList<>();
        armas_disponibles.add(new Revolver());
        armas_disponibles.add(new Escopeta());
        armas_disponibles.add(new Bomba());
    }

    public void comprarArma(Jugador jugador, int arma) {
        /*
        *   Nombre: comprarArma
        *   Parámetros: Jugador jugador, int arma
        *   Retorno: void
        *   Resumen: Método maneja la compra del arma
        */

        if (arma == 1) {
            ArrayList<Arma> armas = new ArrayList<>();
            armas = jugador.getArmas();
            Revolver revolver = new Revolver();
            armas.add(revolver);
            jugador.setArmas(armas);

        } else if (arma == 2) {
            ArrayList<Arma> armas = new ArrayList<>();
            armas = jugador.getArmas();
            Escopeta escopeta = new Escopeta();
            armas.add(escopeta);
            jugador.setArmas(armas);

        } else if (arma == 3) {
            ArrayList<Arma> armas = new ArrayList<>();
            armas = jugador.getArmas();
            Bomba bomba = new Bomba();
            armas.add(bomba);
            jugador.setArmas(armas);

        }
    }

    public void comprarVida(Jugador jugador, int vidaExtra, int costo) {
        /*
        *   Nombre: comprarVida
        *   Parámetros: Jugador jugador, int vidaExtra, int costo
        *   Retorno: void
        *   Resumen: Método maneja la compra de vida
        */

        // Cambiar la vida del jugador y restar el coste de P Points
        jugador.setVida(jugador.getVida() + vidaExtra);
        jugador.setP_Points(jugador.getP_Points() - costo);
        
        // Revisar si la vida quedo > 100
        if (jugador.getVida() > 100) {
            jugador.setVida(100);
        }        
    }

    public void comprarEnergia(Jugador jugador, int energiaExtra, int costo) {
        /*
        *   Nombre: comprarEnergia
        *   Parámetros: Jugador jugador, int energiaExtra, int costo
        *   Retorno: void
        *   Resumen: Método maneja la compra de energía
        */

        // Cambiar la energía del jugador y restar el coste de P Points
        jugador.setEnergia(jugador.getEnergia() + energiaExtra);
        jugador.setP_Points(jugador.getP_Points() - costo);
        
        // Revisar si la energía quedo > 50
        if (jugador.getEnergia() > 50) {
            jugador.setEnergia(50);
        }        
    }

    ////////////////////////////////////////////////////
    //                   GETTERS                      //
    public char getRepresentacion() {
        return 'T';
    }

    public ArrayList<Arma> getListaArmas() {
        return this.armas_disponibles;
    }
}
