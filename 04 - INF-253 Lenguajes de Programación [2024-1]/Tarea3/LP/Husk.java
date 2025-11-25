package LP;

public class Husk extends Enemigo {
    public Husk() {
        /*
        *   Nombre: Husk
        *   Parámetros: void
        *   Retorno: void
        *   Resumen: Constructor de la clase
        */

        super("Husk", 75, 30, 0.7f, 0.5f, 0.2f);
    }

    ////////////////////////////////////////////////////
    //                   GETTERS                      //
    public char getRepresentacion() {
        return 'H';
    }
}
