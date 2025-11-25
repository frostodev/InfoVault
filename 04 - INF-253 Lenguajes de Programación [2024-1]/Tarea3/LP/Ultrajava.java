package LP;

import java.util.Random;
import java.util.Scanner;

public class Ultrajava {

    public static void limpiarPantalla() {
        /*
        *   Nombre: limpiarPantalla
        *   Parámetros: void
        *   Retorno: void
        *   Resumen: Método que limpia la pantalla
        */

        System.out.print("\033[H\033[2J");
        System.out.flush();
    }

    public static Scanner genScanner() {
        /*
        *   Nombre: genScanner
        *   Parámetros: void
        *   Retorno: void
        *   Resumen: Método que genera un Scanner
        */

        Scanner scanner = new Scanner(System.in);
        return scanner;
    }

    public static void verEstadisticas(Jugador jugador, Arena arena, int rondas, Scanner pausa) {
        /*
        *   Nombre: verEstadisticas
        *   Parámetros: Jugador jugador, Arena arena, int rondas, Scanner pausa
        *   Retorno: void
        *   Resumen: Método que muestra las estadísticas del jugador
        */

        limpiarPantalla();

        System.out.println("");
        System.out.println("=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=");
        System.out.println("Nombre: " + jugador.getNombre());
        System.out.println("HP: " + jugador.getVida() + " | Energía: " + jugador.getEnergia() + " | P Points: " + jugador.getP_Points());
        System.out.println("Kills: " + jugador.getKills() + " | Ronda actual: " + arena.getRonda() + "/" + rondas + " | Puntuación: " + jugador.getPuntuacion());
        System.out.println("");
        System.out.println("Tu Arsenal:");
        for (int i = 0; i < jugador.getArmas().size(); i++) {
            System.out.println(jugador.getArmas().get(i).getNombre());
        }
        System.out.println("=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=");
        System.out.println("Presiona enter para volver...");
        pausa.nextLine();
    }

    public static boolean interfazPelea(Jugador jugador, Visible enemigoV, Arena arena, Scanner scanner) {
        /*
        *   Nombre: interfazPelea
        *   Parámetros: Jugador jugador, Visible enemigoV, Arena arena, Scanner scanner
        *   Retorno: boolean
        *   Resumen: Método que muestra la interfaz de pelea
        */

        // Castear el enemigo de tipo Visible a tipo Enemigo
        Enemigo enemigo = (Enemigo) enemigoV;

        // Ciclo de batalla
        while (true) {

            // Verificar si el jugador tiene vida
            if (jugador.getVida() <= 0) {
                // Game over
                break;
            }

            // // Ataque del jugador // // 

            limpiarPantalla();
            System.out.println("");
            System.out.println("=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=");
            System.out.println("Haz sido emboscado por un " + enemigo.getNombre() + "!");
            System.out.println("-------------------------------------------------------------");
            System.out.println("HP: " + jugador.getVida() + " | Energía: " + jugador.getEnergia() + " | HP Enemigo: " + enemigo.getVida());
            System.out.println("");
            System.out.println("Elige arma: ");
            for (int i = 0; i < jugador.getArmas().size(); i++) {
                System.out.println((i + 1) + ") " + jugador.getArmas().get(i).getNombre());
            }
            System.out.print("> ");
            int seleccionArma = scanner.nextInt();
            limpiarPantalla();
            System.out.println("");
            System.out.println("=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=");
            System.out.println("Haz sido emboscado por un " + enemigo.getNombre() + "!");
            System.out.println("-------------------------------------------------------------");
            System.out.println("HP: " + jugador.getVida() + " | Energía: " + jugador.getEnergia() + " | HP Enemigo: " + enemigo.getVida());
            System.out.println("");
            System.out.println("Arma seleccionada: " + jugador.getArmas().get(seleccionArma - 1).getNombre());
            System.out.println("Lugar donde disparar: ");
            System.out.println("1) Cabeza");
            System.out.println("2) Torso");
            System.out.println("3) Pierna");
            System.out.print("> ");
            int seleccionDisparo = scanner.nextInt();
    
            // Disparar al enemigo
            // Cabeza
            if (seleccionDisparo == 1) {
                jugador.Disparar(enemigo, jugador.getArmas().get(seleccionArma - 1), arena, 1);

            // Torso
            } else if (seleccionDisparo == 2) {
                jugador.Disparar(enemigo, jugador.getArmas().get(seleccionArma - 1), arena, 2);
            
            // Pierna
            } else if (seleccionDisparo == 3) {
                jugador.Disparar(enemigo, jugador.getArmas().get(seleccionArma - 1), arena, 3);
            }
            
            // Verificar si se derrotó al enemigo
            if (enemigo.getVida() <= 0) {
                limpiarPantalla();
    
                int kills = jugador.getKills() + 1;
                jugador.setKills(kills);

                int ppoints = jugador.getP_Points();
                jugador.setP_Points(ppoints + 50);
    
                System.out.println("");
                System.out.println("=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=");
                System.out.println("Has derrotado al " + enemigo.getNombre() + "!");
                System.out.println("-------------------------------------------------------------");
                System.out.println("HP: " + jugador.getVida() + " | Energía: " + jugador.getEnergia() + " | HP Enemigo: " + enemigo.getVida());
                System.out.println("");
                System.out.println("Presiona enter para volver...");
                Scanner pausa = new Scanner(System.in);
                pausa.nextLine();
                return true;     
            }

            // // Ataque del enemigo // //

            // Intentar esquivar el ataque
            // Para ello, si tiene más de un punto de energía se le da la opción
            boolean esquivar = false;
            if (jugador.getEnergia() > 1) {
                int opcionEsquivar;
                System.out.println("Intentar esquivar el ataque del enemigo?: ");
                System.out.println("1) Sí (1 E)");
                System.out.println("2) No");
                System.out.print(">");
                opcionEsquivar = scanner.nextInt();

                // Esquivar
                if (opcionEsquivar == 1) {
                    // Hay un 40% de probabilidad de esquivar
                    Random random = new Random();
                    double probEsquivo = random.nextDouble();

                    if (probEsquivo < 0.4) {
                        esquivar = true;
                    } 
                }

                // No esquivar
                else if (opcionEsquivar == 2) {
                    esquivar = false;
                }
            }

            // Si no se esquivó el ataque
            if (!esquivar) {
                int vida = jugador.getVida() - enemigo.getAtk();
                jugador.setVida(vida);

                // En caso de que la vida del jugador sea menor a 0
                if (jugador.getVida() < 0) {
                    jugador.setVida(0);
                }

                limpiarPantalla();
        
                System.out.println("");
                System.out.println("=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=");
                System.out.println("El " + enemigo.getNombre() + " ataca! (-" + enemigo.getAtk() + " HP)");
                System.out.println("-------------------------------------------------------------");
                System.out.println("HP: " + jugador.getVida() + " | Energía: " + jugador.getEnergia() + " | HP Enemigo: " + enemigo.getVida());
                System.out.println("");
                System.out.println("Presiona enter para volver...");
                Scanner pausa = new Scanner(System.in);
                pausa.nextLine();
            }

            // En caso de que si se esquivó el ataque
            else {

                limpiarPantalla();

                // Recompensar al jugador
                int ppoints = jugador.getP_Points();
                jugador.setP_Points(ppoints + 5);

                System.out.println("");
                System.out.println("=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=");
                System.out.println("Ataque esquivado! (+5 P)");
                System.out.println("-------------------------------------------------------------");
                System.out.println("HP: " + jugador.getVida() + " | Energía: " + jugador.getEnergia() + " | HP Enemigo: " + enemigo.getVida());
                System.out.println("");
                System.out.println("Presiona enter para volver...");
                Scanner pausa = new Scanner(System.in);
                pausa.nextLine();             
            }
        }

        // Game over
        return false;
    }

    public static void interfazTerminal(Terminal terminal, Jugador jugador, Scanner scanner) {
        /*
        *   Nombre: interfazTerminal
        *   Parámetros: Terminal terminal, Jugador jugador, Scanner scanner
        *   Retorno: void
        *   Resumen: Método que muestra la interfaz de la terminal
        */

        limpiarPantalla();
        System.out.println("");
        System.out.println("=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=");
        System.out.println("                    --- TERMINAL ACTIVA ---");
        System.out.println("HP: " + jugador.getVida() + " | Energía: " + jugador.getEnergia() + " | P Points: " + jugador.getP_Points());
        System.out.println("");
        System.out.println("Que desea hacer?: ");
        System.out.println("1) Comprar Armas");
        System.out.println("2) Comprar Vida");
        System.out.println("3) Comprar Energía");
        System.out.println("4) Salir");
        System.out.print("> ");
        int seleccionTerminal = scanner.nextInt();

        // Si el jugador no tiene P Points para comprar 
        if (seleccionTerminal != 4 && jugador.getP_Points() < 30) {
            System.out.println("No tienes P Points suficientes para comprar.");
            System.out.println("Presiona enter para volver...");
            Scanner pausa = new Scanner(System.in);
            pausa.nextLine();
            return;
        }
        
        // Comprar arma
        if (seleccionTerminal == 1) {
            limpiarPantalla();
            System.out.println("");
            System.out.println("=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=");
            System.out.println("                    --- COMPRAR ARMAS ---");
            System.out.println("HP: " + jugador.getVida() + " | Energía: " + jugador.getEnergia() + " | P Points: " + jugador.getP_Points());
            System.out.println("");

            System.out.println("Sólo puedes tener una arma del mismo tipo a la vez.");
            System.out.println("Los explosivos no cuentan, pues son de un solo uso.");
            System.out.println("1) Revolver (30 P)");
            System.out.println("2) Escopeta (250 P)");
            System.out.println("3) Bomba Nuclear (7500 P)");
            System.out.println("4) Cerrar Terminal");
            System.out.print("> ");
            int seleccionArma = scanner.nextInt();

            // Revolver
            if (seleccionArma == 1) {
                // Revisar cantidad de P Points
                if (jugador.getP_Points() < 30) {
                    System.out.println("No tienes P Points suficientes para comprar.");
                    System.out.println("Presiona enter para volver...");
                    Scanner pausa = new Scanner(System.in);
                    pausa.nextLine();
                    return;         

                } else {
                    // Revisar armas del jugador
                    for (int i = 0; i < jugador.getArmas().size(); i++) {
                        if (jugador.getArmas().get(i) instanceof Revolver) {
                            System.out.println("Ya tienes esta arma!");
                            System.out.println("Presiona enter para continuar...");
                            Scanner pausa = new Scanner(System.in);
                            pausa.nextLine();

                        } else {
                            // Añadir el arma al inventario
                            terminal.comprarArma(jugador, 1);

                            // Cobrarle al jugador
                            int ppoints = jugador.getP_Points() - 30;
                            jugador.setP_Points(ppoints);

                            System.out.println("Arma comprada!");
                            System.out.println("Presiona enter para salir de la terminal.");
                            Scanner pausa = new Scanner(System.in);
                            pausa.nextLine();
                            break;
                        }
                    }
                }
            }

            // Escopeta
            else if (seleccionArma == 2) {
                // Revisar cantidad de P Points
                if (jugador.getP_Points() < 250) {
                    System.out.println("No tienes P Points suficientes para comprar.");
                    System.out.println("Presiona enter para volver...");
                    Scanner pausa = new Scanner(System.in);
                    pausa.nextLine();
                    return;     

                } else {
                // Revisar armas del jugador
                    for (int i = 0; i < jugador.getArmas().size(); i++) {

                        if (jugador.getArmas().get(i) instanceof Escopeta) {
                            System.out.println("Ya tienes esta arma!");
                            System.out.println("Presiona enter para continuar...");
                            Scanner pausa = new Scanner(System.in);
                            pausa.nextLine();

                        } else {
                            // Añadir el arma al inventario
                            terminal.comprarArma(jugador, 2);

                            // Cobrarle al jugador
                            int ppoints = jugador.getP_Points() - 250;
                            jugador.setP_Points(ppoints);

                            System.out.println("Arma comprada!");
                            System.out.println("Presiona enter para salir de la terminal.");
                            Scanner pausa = new Scanner(System.in);
                            pausa.nextLine();
                            break;
                        }
                    }
                }
            }

            // Bomba nuclear
            else if (seleccionArma == 3) {
                // Revisar cantidad de P Points
                if (jugador.getP_Points() < 7500) {
                    System.out.println("No tienes P Points suficientes para comprar.");
                    System.out.println("Presiona enter para volver...");
                    Scanner pausa = new Scanner(System.in);
                    pausa.nextLine();
                    return;

                } else {
                    // Revisar armas del jugador
                    for (int i = 0; i < jugador.getArmas().size(); i++) {
                        if (jugador.getArmas().get(i) instanceof Bomba) {
                            System.out.println("Ya tienes esta arma!");
                            System.out.println("Presiona enter para continuar...");
                            Scanner pausa = new Scanner(System.in);
                            pausa.nextLine();

                        } else {
                            // Añadir el arma al inventario
                            terminal.comprarArma(jugador, 3);

                            // Cobrarle al jugador
                            int ppoints = jugador.getP_Points() - 7500;
                            jugador.setP_Points(ppoints);

                            System.out.println("Arma comprada!");
                            System.out.println("Presiona enter para salir de la terminal.");
                            Scanner pausa = new Scanner(System.in);
                            pausa.nextLine();
							break;
                        }
                    }
                }
            }

            // Salir
            else if (seleccionArma == 4) {
                return;
            }
        }

        // Comprar vida
        else if (seleccionTerminal == 2) {
            limpiarPantalla();
            System.out.println("");
            System.out.println("=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=");
            System.out.println("                    --- COMPRAR VIDA ---");
            System.out.println("HP: " + jugador.getVida() + " | Energía: " + jugador.getEnergia() + " | P Points: " + jugador.getP_Points());
            System.out.println("");

            System.out.println("La cantidad máxima de HP es 100! El exceso no se rembolasará!");
            System.out.println("1) 25 HP (50 P Points)");
            System.out.println("2) 50 HP (90 P Points)");
            System.out.println("3) 75 HP (120 P Points)");
            System.out.println("4) Recargar vida (200 P Points)");
            System.out.println("5) Cerrar terminal");
            System.out.print("> ");
            int seleccionVida = scanner.nextInt();

            // 25HP
            if (seleccionVida == 1) {
                // Revisar cantidad de P Points
                if (jugador.getP_Points() < 50) {
                    System.out.println("No tienes P Points suficientes para comprar.");
                    System.out.println("Presiona enter para volver...");
                    Scanner pausa = new Scanner(System.in);
                    pausa.nextLine();
                    return;                    
                }

                // Si tiene la cantidad de P Points
                else {
                    // Añadir la vida
                    terminal.comprarVida(jugador, 25, 50);
                }
            } 
            
            // 50HP
            else if (seleccionVida == 2) {
                // Revisar cantidad de P Points
                if (jugador.getP_Points() < 90) {
                    System.out.println("No tienes P Points suficientes para comprar.");
                    System.out.println("Presiona enter para volver...");
                    Scanner pausa = new Scanner(System.in);
                    pausa.nextLine();
                    return;                    
                }

                // Si tiene la cantidad de P Points
                else {
                    // Añadir la vida
                    terminal.comprarVida(jugador, 50, 90);
                }
            }

            // 75HP
            else if (seleccionVida == 3) {
                // Revisar cantidad de P Points
                if (jugador.getP_Points() < 120) {
                    System.out.println("No tienes P Points suficientes para comprar.");
                    System.out.println("Presiona enter para volver...");
                    Scanner pausa = new Scanner(System.in);
                    pausa.nextLine();
                    return;                    
                }

                // Si tiene la cantidad de P Points
                else {
                    // Añadir la vida
                    terminal.comprarVida(jugador, 75, 120);
                }
            }

            // Recargar
            else if (seleccionVida == 4) {
                // Revisar cantidad de P Points
                if (jugador.getP_Points() < 200) {
                    System.out.println("No tienes P Points suficientes para comprar.");
                    System.out.println("Presiona enter para volver...");
                    Scanner pausa = new Scanner(System.in);
                    pausa.nextLine();
                    return;                    
                }

                // Si tiene la cantidad de P Points
                else {
                    // Añadir la vida
                    terminal.comprarVida(jugador, 100, 200);
                }
            }

            // Cerrar terminal
            else if (seleccionVida == 5) {
                return;
            }
        }

        // Comprar energía
        else if (seleccionTerminal == 3) {
            limpiarPantalla();
            System.out.println("");
            System.out.println("=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=");
            System.out.println("                    --- COMPRAR ENERGÍA ---");
            System.out.println("HP: " + jugador.getVida() + " | Energía: " + jugador.getEnergia() + " | P Points: " + jugador.getP_Points());
            System.out.println("");

            System.out.println("La cantidad máxima de energía es 50! El exceso no se rembolasará!");
            System.out.println("1) 5 E (60 P Points)");
            System.out.println("2) 10 E (100 P Points)");
            System.out.println("3) 25 E (250 P Points)");
            System.out.println("4) Recargar energía (450 P Points)");
            System.out.println("5) Cerrar terminal");
            System.out.print("> ");
            int seleccionVida = scanner.nextInt();

            // 5E
            if (seleccionVida == 1) {
                // Revisar cantidad de P Points
                if (jugador.getP_Points() < 60) {
                    System.out.println("No tienes P Points suficientes para comprar.");
                    System.out.println("Presiona enter para volver...");
                    Scanner pausa = new Scanner(System.in);
                    pausa.nextLine();
                    return;                    
                }

                // Si tiene la cantidad de P Points
                else {
                    // Añadir la energía
                    terminal.comprarEnergia(jugador, 5, 60);
                }
            } 
            
            // 10E
            else if (seleccionVida == 2) {
                // Revisar cantidad de P Points
                if (jugador.getP_Points() < 100) {
                    System.out.println("No tienes P Points suficientes para comprar.");
                    System.out.println("Presiona enter para volver...");
                    Scanner pausa = new Scanner(System.in);
                    pausa.nextLine();
                    return;                    
                }

                // Si tiene la cantidad de P Points
                else {
                    // Añadir la energía
                    terminal.comprarEnergia(jugador, 10, 100);
                }
            }

            // 25E
            else if (seleccionVida == 3) {
                // Revisar cantidad de P Points
                if (jugador.getP_Points() < 250) {
                    System.out.println("No tienes P Points suficientes para comprar.");
                    System.out.println("Presiona enter para volver...");
                    Scanner pausa = new Scanner(System.in);
                    pausa.nextLine();
                    return;                    
                }

                // Si tiene la cantidad de P Points
                else {
                    // Añadir la energía
                    terminal.comprarEnergia(jugador, 25, 250);
                }
            }

            // Recargar
            else if (seleccionVida == 4) {
                // Revisar cantidad de P Points
                if (jugador.getP_Points() < 450) {
                    System.out.println("No tienes P Points suficientes para comprar.");
                    System.out.println("Presiona enter para volver...");
                    Scanner pausa = new Scanner(System.in);
                    pausa.nextLine();
                    return;                    
                }

                // Si tiene la cantidad de P Points
                else {
                    // Añadir la energía
                    terminal.comprarEnergia(jugador, 50, 450);
                }
            }
        }

        // Salir
        else if (seleccionTerminal == 4) {
            return;
        }
    }

    public static void gameOver(Jugador jugador, Arena arena, boolean estado) {
        /*
        *   Nombre: gameOver
        *   Parámetros: Jugador jugador, Arena arena, boolean estado
        *   Retorno: void
        *   Resumen: Método que muestra la información del game over
        */

        limpiarPantalla();

        // Si es un game over victorioso
        if (estado) {
            System.out.println("Sobreviviste las " + (arena.getRonda() - 1) + " rondas.");
            System.out.println("Felicitaciones!");
            System.out.println("");
            System.out.println("Puntuación final: " + jugador.getPuntuacion());
            System.out.println("Hasta la próxima!");
            return;
        }

        // Si es un game over de derrota
        else {
            System.out.println("Sobreviviste " + (arena.getRonda() - 1) + " rondas.");
            System.out.println("Puntuación final: " + jugador.getPuntuacion());
            System.out.println("Hasta la próxima!");
            return;
        }
    }

    public static void main(String[] args) {
        /*
        *   Nombre: main
        *   Parámetros: String[] args
        *   Retorno: void
        *   Resumen: Método principal
        */

        Scanner scanner = genScanner();
        
        System.out.println("--- ULTRAJAVA ---");
        System.out.println("Ingresa el nombre de tu jugador: ");
        System.out.print("> ");
        String nombre = scanner.nextLine();

        System.out.println("Ingresa la cantidad de rondas (0 = infinitas): ");
        int rondas = scanner.nextInt();

        // Crear la arena con tamaño 15
	    System.out.println("Creando Arena...");
        Arena arenaJuego = new Arena(15);

        // Crear al jugador
        System.out.println("Creando jugador...");
        Jugador jugador = new Jugador(100, 3, 0, nombre);
        
        // Crear la terminal
        Terminal terminal = new Terminal();

        limpiarPantalla();

        System.out.println("--- CÓMO JUGAR ---");
        System.out.println("Puedes moverte de izquierda a derecha por el mapa.");
        System.out.println("En este, te encontrarás diversos enemigos de distinto nivel.");
        System.out.println("Para ganar, debes eliminar a todos los enemigos del mapa.");
        System.out.println("");
        System.out.println("Tienes acceso a una Terminal, en la cual puedes comprar vida, energía y armas.");
        System.out.println("Buena suerte.");
        System.out.println("");
        System.out.println("Presiona enter para comenzar el juego.");

        Scanner pausa = new Scanner(System.in);
        pausa.nextLine();

        // // // CICLO PRINCIPAL // // //
        String eleccion;
        arenaJuego.nuevaRonda(terminal, jugador);

        while (true) {

            // Verificar si ya se llegó a las rondas objetivo
            // Además, añadir la comprobación del límite de rondas
            if (rondas != 0 && arenaJuego.getRonda() == rondas + 1) {
                gameOver(jugador, arenaJuego, true);
                break;
            }

            // Calcular puntuación, esta es el producto acumulado entre las kills, los PPoints actuales y la ronda actual
            int puntuacion = jugador.getPuntuacion() + (jugador.getKills() * jugador.getP_Points() * arenaJuego.getRonda());
            jugador.setPuntuacion(puntuacion);            

            // Limpiar la pantalla
            limpiarPantalla();

            System.out.println("");
            System.out.println("=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=");

            if (arenaJuego.getRonda() == rondas) {
                System.out.println("                  RONDA FINAL (" + arenaJuego.getRonda() + ")       ");
            } else {
                System.out.println("                          RONDA " + arenaJuego.getRonda() + "       ");
            }

            arenaJuego.mostrar();
            System.out.println("=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=");
            System.out.println(" ");
            System.out.println("Acción: ");
            System.out.println("M.- Moverse | V.- Ver estadísticas | S.- Salir");
            System.out.print("> ");

            eleccion = scanner.nextLine();
            
            // Moverse
            if (eleccion.equals("m") || eleccion.equals("M")) {
                String eleccionLado;
                System.out.println("Derecha (D) o Izquierda (I)?: ");
                System.out.print("> ");
                eleccionLado = scanner.nextLine();

                // Derecha
                if (eleccionLado.equals("D") || eleccionLado.equals("d")) {

                    // Verificar si no se está saliendo del mapa
                    if (jugador.getPos() != 14) {
                        
                        Visible entidadSiguiente = jugador.moverse(1, arenaJuego);

                        // Si la entidad en el espacio a moverse es enemigo
                        if (entidadSiguiente instanceof Enemigo) {
                            if (entidadSiguiente.getRepresentacion() != 'N') {
                                boolean iPelea = interfazPelea(jugador, entidadSiguiente, arenaJuego, scanner);
                                // Eliminar al enemigo en caso de victoria
                                if (iPelea) {
                                    arenaJuego.eliminarCasilla(jugador.getPos() + 1);
                                }

                                // Game over
                                else {
                                    gameOver(jugador, arenaJuego, false);
                                    return;
                                }
                            }
                        }

                        // Por el otro lado, si es una terminal
                        else if (entidadSiguiente instanceof Terminal) {
                            if (entidadSiguiente.getRepresentacion() != 'N') {
                                interfazTerminal(terminal, jugador, scanner);
                            }
                        }                        
                    }
                
                // Izquierda
                } else if (eleccionLado.equals("I") || eleccionLado.equals("i")){

                    // Verificar si se está saliendo del mapa
                    if (jugador.getPos() != 0) {

                        Visible entidadSiguiente = jugador.moverse(0, arenaJuego);

                        // Si la entidad en el espacio a moverse es enemigo
                        if (entidadSiguiente instanceof Enemigo) {
                            if (entidadSiguiente.getRepresentacion() != 'N') {
                                boolean iPelea = interfazPelea(jugador, entidadSiguiente, arenaJuego, scanner);
                                // Eliminar al enemigo en caso de victoria
                                if (iPelea) {
                                    arenaJuego.eliminarCasilla(jugador.getPos() - 1);
                                }
    
                                // Game over
                                else {
                                    gameOver(jugador, arenaJuego, false);
                                    return;
                                }
                            }
                        }

                        // Por el otro lado, si es una terminal
                        else if (entidadSiguiente instanceof Terminal) {
                            if (entidadSiguiente.getRepresentacion() != 'N') {
                                interfazTerminal(terminal, jugador, scanner);
                            }
                        }
                    }
                }

            // Ver estadísticas
            } else if (eleccion.equals("v") || eleccion.equals("V")) {
                verEstadisticas(jugador, arenaJuego, rondas, scanner);

            // Salir
            } else if (eleccion.equals("S") || eleccion.equals("s")) {
                gameOver(jugador, arenaJuego, false);
                break;

            // Debug
            } else if (eleccion.equals("a")) {
                arenaJuego.nuevaRonda(terminal, jugador);
                jugador.setP_Points(100000);
                jugador.setVida(100);
            }

            // Verificar si ya se eliminaron a todos los enemigos
            if (!arenaJuego.verificarEnemigos()) {
                // En ese caso, avanzar la ronda
                arenaJuego.nuevaRonda(terminal, jugador);
            } 

        }

        // Cerrar el scanner
        scanner.close();
    }
}
