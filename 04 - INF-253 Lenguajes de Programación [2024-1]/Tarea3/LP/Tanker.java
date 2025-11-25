package LP;

public class Tanker extends Enemigo {

    public Tanker() {
        /*
        *   Nombre: Tanker
        *   Parámetros: void
        *   Retorno: void
        *   Resumen: Constructor de la clase
        */
        super("Tanker", 250, 75, 0.9f, 0.7f, 0.95f);
    }

    ////////////////////////////////////////////////////
    //                   GETTERS                      //
    public char getRepresentacion() {
        return 'K';
    }
}
