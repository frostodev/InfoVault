from random import randint

def generar_universo(size):
    
    con_salida = ""
    voc_salida = ""
    str_salida = ""
    vocales = "aeiou"
    consonantes = "bdfghjklmnñpqrstvxyz"

    # Calcular porcentajes
    cant_con = round(size * 0.6)
    cant_voc = round(size * 0.4)

    i = 0
    # Consonantes aleatorias
    while i < cant_con:
        con_salida += consonantes[randint(0,19)]
        i += 1

    j = 0
    # Vocales aleatorias
    while j < cant_voc:
        voc_salida += vocales[randint(0,4)]
        j += 1

    string = con_salida + voc_salida

    # Verificar tamaño string
    if len(string) >= 10 and len(string) <= 35:
        str_salida = string
    else:
        str_salida = ""

    return str_salida

#############################################################################

print("Bienvenido al PysCrable")
preguntar = True
verificar = True

# Verificar si el universo es >= 10
while preguntar:
    size = int(input("Ingrese el tamaño del universo de letras: "))
    if size >= 10:
        preguntar = False
    
universo = generar_universo(size)
print("Universo:",universo)
palabras = input("Ingrese sus 3 palabras separadas por un guión: ")

# Determinar las posiciones de los guiones para obtener las palabras
cont = 0
while cont <= len(palabras)-1 and verificar:
    # Posición primer guión
    if palabras[cont] == "-":
        pos_guion1 = cont
        verificar = False
    cont += 1

palabra1 = palabras[:pos_guion1]

verificar = True
cont2 = 0
while cont2 <= len(palabras)-1 and verificar:
    # Posición segundo guión
    if palabras[pos_guion1 + 1 + cont2] == "-":
        pos_guion2 = pos_guion1 + 1 + cont2
        verificar = False
    cont2 += 1

palabra3 = palabras[pos_guion2 + 1:]
palabra2 = palabras[pos_guion1 + 1:pos_guion2]

##############################################################################

# Calculo de puntaje

puntaje_total = 0
puntos = 0
cant_vocales_palabra = 0
cant_cons_palabra = 0
palabra_buena = False
continuar = True

z = 1
while z <= 3 and continuar:
    iterar = True

    # Ir cambiando las palabras dependiendo del número de ciclo
    if z == 1:
        palabra = palabra1

    elif z == 2:
        palabra = palabra2

    elif z == 3:
        palabra = palabra3

    # Ciclo para cada palabra
    while iterar:
        letras_acertadas = 0
        
        w = 0
        # Verificar si la palabra existe en el universo, letra por letra

        while w <= len(palabra)-1:
            if palabra[w] in universo:
                letras_acertadas += 1

            # Si todas las letras están en el universo, la palabra es válida
            if letras_acertadas == len(palabra)-1:
                palabra_buena = True
            
            # Sino, no es válida y era nomas.
            else:
                palabra_buena = False

            w += 1
        
        # Si la palabra actual tiene menos de 3 letras
        if len(palabra) < 3:
            print("Error, las palabras deben tener al menos 3 letras, perdió su intento")
            puntaje_total = 0
            palabra_buena = False
            iterar = False
            continuar = False

        # Determinar número de consonantes y vocales
        # En el caso de que la palabra se pueda formar con el universo, se calcula el puntaje
        if palabra_buena:
            r = 0
            while r <= len(palabra)-1:
                list_consonantes = "bdfghjklmnñpqrstvxyz"
                list_vocales = "aeiou"

                if palabra[r] in list_vocales:
                    cant_vocales_palabra += 1

                if palabra[r] in list_consonantes:
                    cant_cons_palabra += 1
                
                r += 1

            puntos += (len(palabra) * 10) + (5 * cant_vocales_palabra) + (3 * cant_vocales_palabra)
            print(palabra,"se puede generar con",universo)
            print("obtienes",puntos,"puntos")
            puntaje_total += puntos

            iterar = False

        # Sino, simplemente se pasa a la iteración de la palabra siguiente
        else:
            iterar = False

    # Si ya se llegó a la tercera palabra
    if z == 3:
        iterar = False
    z += 1

print("Fin del intento, obtuviste un total de",puntaje_total,"puntos")