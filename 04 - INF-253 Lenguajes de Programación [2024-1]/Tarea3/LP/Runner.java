package LP;

public class Runner extends Enemigo {
    public Runner() {
        /*
        *   Nombre: Runner
        *   Parámetros: void
        *   Retorno: void
        *   Resumen: Constructor de la clase
        */
        super("Runner", 50, 35, 0.1f, 0.3f, 0.45f);
    }

    ////////////////////////////////////////////////////
    //                   GETTERS                      //
    public char getRepresentacion() {
        return 'R';
    }
}
