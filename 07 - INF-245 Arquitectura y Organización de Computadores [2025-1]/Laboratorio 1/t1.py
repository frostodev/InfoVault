import random

class Snake:
    def __init__(self, fila, columna):
        self.posicion = (fila, columna)

    def mover(self, cantidadFilas, cantidadColumnas):
        # Calcula la dirección del movimiento y lo realiza paso a paso
        filaActual, colActual = self.posicion
        pasos = max(abs(cantidadFilas), abs(cantidadColumnas))
        filaDir = 1 if cantidadFilas > 0 else -1 if cantidadFilas < 0 else 0
        colDir = 1 if cantidadColumnas > 0 else -1 if cantidadColumnas < 0 else 0

        # Mover a Snake
        for paso in range(1, pasos + 1):
            nuevaFila = filaActual + paso * filaDir
            nuevaColumna = colActual + paso * colDir
            contenido = tablero[nuevaFila][nuevaColumna]
                
            # Si se encuentra con un guardia
            if isinstance(contenido, Guardia):
                self.posicion = (nuevaFila, nuevaColumna)
                gameOver()
                    
            # Si se encuentra con el objetivo
            elif isinstance(contenido, Objetivo):
                self.posicion = (nuevaFila, nuevaColumna)
                hackeo()
                return

        # Actualizar la posición
        self.posicion = (filaActual + cantidadFilas, colActual + cantidadColumnas)

class Objetivo:
    def __init__(self):
        self.posicion = None

class Guardia:
    def __init__(self):
        self.posicion = None

def convertirABase(numero, base):
    if base == "Binario":
        numero = int(numero)
        if numero == 0:
            return "0"
        convertido = ""
        while numero > 0:
            convertido = str(numero % 2) + convertido
            numero = numero // 2
        return convertido

    elif base == "Octal":
        numero = int(numero)
        if numero == 0:
            return "0"
        convertido = ""
        while numero > 0:
            convertido = str(numero % 8) + convertido
            numero = numero // 8
        return convertido

    elif base == "Hexadecimal":
        if numero == 0:
            return "0"
        digitosHex = "0123456789ABCDEF"
        convertido = ""
        while numero > 0:
            convertido = digitosHex[numero % 16] + convertido
            numero = numero // 16
        return convertido

    elif base == "Decimal":
        # Convierte desde binario, octal o hexadecimal a decimal
        numeroStr = str(numero).upper()

        if all(c in "01" for c in numeroStr):
            baseOrigen = 2
        elif all(c in "01234567" for c in numeroStr):
            baseOrigen = 8
        elif all(c in "0123456789ABCDEF" for c in numeroStr):
            baseOrigen = 16
        else:
            print("Error: el número ingresado no pertenece a una base válida.")
            return -1

        resultado = 0
        exponente = 0
        for caracter in reversed(numeroStr):
            if caracter.isdigit():
                valor = int(caracter)
            else:
                valor = ord(caracter) - ord('A') + 10
            resultado += valor * (baseOrigen ** exponente)
            exponente += 1

        return resultado

    else:
        print("Base no reconocida.")
        return -1

def agregarElemento(tablero, fila, columna, elemento):
    # Intenta agregar un elemento al tablero si la posición es válida
    if 0 <= fila < len(tablero) and 0 <= columna < len(tablero[0]):
        if not isinstance(tablero[fila][columna], Snake) and not isinstance(tablero[fila][columna], Objetivo):
            tablero[fila][columna] = elemento
        else:
            print("No se pudo colocar el elemento en la posición deseada")

def generarTablero(largo, cantidadGuardias):
    # Crea el tablero con Snake, un objetivo y varios guardias
    tablero = [["X" for _ in range(largo)] for _ in range(11)]
    snake = Snake(4, 0) # La posición en en el tablero sigue siendo (5, 0)! (por la forma en la que lo implementamos) 
    agregarElemento(tablero, 4, 0, snake)

    # Objetivo
    objetivo = Objetivo()
    posicionFila = random.randint(0, 10)
    posicionColumna = random.randint(0, largo - 1)
    objetivo.posicion = posicionFila, posicionColumna
    agregarElemento(tablero, posicionFila, posicionColumna, objetivo)

    # Guardias
    for _ in range(cantidadGuardias):
        guardia = Guardia()
        posicionFila = random.randint(0, 10)
        posicionColumna = random.randint(0, largo - 1)
        guardia.posicion = posicionFila, posicionColumna
        agregarElemento(tablero, posicionFila, posicionColumna, guardia)

    return tablero, snake, objetivo

def mostrarTablero():
    # Imprime el tablero con todos los elementos actuales
    for i in range(len(tablero)):
        for j in range(len(tablero[0])):
            # Verificar que tipo de entidad estamos viendo
            
            # Snake
            if isinstance(tablero[i][j], Snake):
                print('S', end='')
                
            # Objetivo
            elif isinstance(tablero[i][j], Objetivo):
                print('*', end='')
                
            # Guardia
            elif isinstance(tablero[i][j], Guardia):
                print("!", end='')
                
            # Default, mostrar la celda como está nomás
            else:
                print(tablero[i][j], end='')
        print("")

def hackeo():
    # Etapa final del juego donde se realiza un desafío de conversión
    if 0 < largoPasillo <= 10:
        numeroAleatorio = random.randint(0, 20)
    elif 10 < largoPasillo <= 50:
        numeroAleatorio = random.randint(0, 100)
    else:
        numeroAleatorio = random.randint(0, 500)

    # Convertir el número generado a la base actual
    numeroConvertido = convertirABase(numeroAleatorio, tipoBase)
    
    print("--- Hackeando ---")
    print("Ingresa el siguiente número convertido a decimal: ")
    print(f"{numeroConvertido}")
    numeroIngresado = int(input("> "))

    if str(numeroIngresado) == str(numeroAleatorio):
        print("\n¡Felicidades! Has ganado.")
        exit(0)
    else:
        gameOver()

def gameOver():
    # Termina el juego con mensaje
    print("Game Over!")
    exit(0)

# Inputs iniciales del juego
largoPasillo = int(input("Ingrese el largo del pasillo: "))
cantidadGuardias = int(input("Ingrese la cantidad de guardias: "))

# Selección de base numérica según el tamaño del tablero
if largoPasillo <= 20:
    tipoBase = "Binario"
elif 20 < largoPasillo < 100:
    tipoBase = "Octal"
else:
    tipoBase = "Hexadecimal"

# Generar tablero y elementos
tablero, jugador, objetivo = generarTablero(largoPasillo, cantidadGuardias)

# Ciclo principal del juego
while True:
    mostrarTablero()
    print("\nIngresa una acción: ")
    print("W: Moverse hacia arriba")
    print("S: Moverse hacia abajo")
    print("A: Moverse hacia la izquierda")
    print("D: Moverse hacia la derecha")
    print("-1: Salir")

    filaAnterior, columnaAnterior = jugador.posicion
    movimiento = input("> ").lower()

    if movimiento in ["w", "s", "a", "d"]:
        print(f"Cantidad de pasos a moverse en {tipoBase}: ")
        cantidadPasos = input("> ").upper()
        cantidadPasos = convertirABase(cantidadPasos, "Decimal")

        if movimiento == "w":
            jugador.mover(-cantidadPasos, 0)
        elif movimiento == "s":
            jugador.mover(cantidadPasos, 0)
        elif movimiento == "a":
            jugador.mover(0, -cantidadPasos)
        elif movimiento == "d":
            jugador.mover(0, cantidadPasos)

    elif movimiento == "-1":
        gameOver()
    else:
        continue

    # Actualizar el tablero
    filaNueva, columnaNueva = jugador.posicion
    tablero[filaAnterior][columnaAnterior] = "X"
    tablero[filaNueva][columnaNueva] = jugador
    