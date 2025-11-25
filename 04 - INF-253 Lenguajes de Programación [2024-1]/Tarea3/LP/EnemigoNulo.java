package LP;

public class EnemigoNulo extends Enemigo {
    /*
    *   Nombre clase: EnemigoNulo
    *   Resumen: Clase que representa un enemigo nulo.
    */ 
    
    public EnemigoNulo() {
        /*
        *   Nombre: EnemigoNulo
        *   Parámetros: void
        *   Retorno: void
        *   Resumen: Constructor de la clase
        */
        super("Nulo", 0, 0, 0, 0, 0);
    }

    ////////////////////////////////////////////////////
    //                   GETTERS                      //
    public char getRepresentacion() {
        return 'N';
    }
}
