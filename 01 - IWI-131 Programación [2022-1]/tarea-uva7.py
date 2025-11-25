def clasificar(tiempos, n):

    # Crear las listas
    lista_tiempos = []
    lista_final = []
    lista_repetidos = []
    lista_presentada = []
    lista_pilotos = []

    # Separar los tiempos de la lista
    for lista in tiempos:
        lista_tiempos.append(lista[1])
        
        # Ordenarlos de menor a mayor
        lista_tiempos.sort()

    for tiempo in lista_tiempos:
        j = 0
        while j < len(lista_tiempos):
            # Comparar si cada tiempo con los tiempos de la lista original
            # La idea es que si el tiempo analizado es igual al de la lista original, entonces se puede obtener el nombre del piloto

            if tiempo == tiempos[j][1]:
                # Añadir el tiempo junto al nombre del piloto a una nueva lista
                lista_final.append([tiempos[j][1],tiempos[j][0]])
            j += 1

    # Encontrar a los pilotos repetidos
    for lista_piloto in lista_final:
        continuar = True
        k = 0
        menor = 0
        piloto = lista_piloto[1]
        tiempo_piloto = lista_piloto[0]

        while k < len(lista_final) and continuar:
            # Si el nombre del piloto se repite
            # Para evitar que se compare a si mismo, se añade además la condición de que el tiempo no sea igual.
            if piloto == lista_final[k][1] and tiempo_piloto != lista_final[k][0]:

                # Crear una lista con los pilotos repetidos
                lista_repetidos.append(lista_piloto)
                # Este flag sirve para indicar que el ciclo debe detenerse, pues ya añadió al piloto repetido
                continuar = False

            k += 1
    
    # Encontrar el tiempo menor de los pilotos
    for piloto in lista_repetidos:
        flag = True
        i = 0
        while i < len(lista_repetidos) and flag == True:
            if piloto[1] == lista_repetidos[i][1]:
                # Si el tiempo es mayor
                if piloto[0] > lista_repetidos[i][0]:
                    lista_pilotos.append(piloto)
                    flag = False
            i += 1
                    
    # Ahora, eliminar el piloto repetido, dejando el tiempo menor
    for elemento in lista_pilotos:
        lista_final.remove(elemento)

    # Finalmente, separar las listas de los clasificados dependiendo del valor de n
    for clasificados in lista_final:
        lista_presentada = [lista_final[0:n],lista_final[n:]]

    return lista_presentada

###########################################################################################################################################

print("Bienvenidos al Gran Premio de Pythonia (Pythonia Moto Grand Prix 2022)")
lista_practica = eval(input("Ingrese tiempos de la sesión de práctica: "))

# Usando la función con los 10 mejores tiempos (n = 10)
lista_pilotos = clasificar(lista_practica, 10)

# Tomar la primera lista que retorna la función (la de los mejores tiempos)
pilotos_clasificados_directo = lista_pilotos[0]

print("")
print("Clasifican directamente a Q2: ")
print("")
for clasificados in pilotos_clasificados_directo:
    tiempo = clasificados[0]
    piloto = clasificados[1]

    # Mostrar los tiempos en pantalla
    print(piloto,"con tiempo",tiempo)

# Ahora, lo mismo pero para Q1 (n = 2)
print("")
lista_q1 = eval(input("Ingrese tiempos de la sesión Q1: "))

pilotos_clasificados_q2 = clasificar(lista_q1, 2)

# Mostrar la lista en pantalla
print("")
print("Clasifican a Q2: ")
print(pilotos_clasificados_q2[0])
print("")

# Luego, mostrar el orden de partida
print("Orden de partida del puesto 13 en adelante:")
print("")
print(pilotos_clasificados_q2[1])

# Finalmente, mostrar el orden de partida para el premio
print("")
tiempos_sesion_q2 = eval(input("Ingrese tiempos de la sesión Q2: "))
print("")
print("Orden de partida para el Gran Premio de Pythonia 2022:")
print("")

# Separar las listas
orden = clasificar(tiempos_sesion_q2, 12)
primeros = orden[0]

# Usando el orden de partida obtenido anteriormente para el puesto 13 en adelante
ultimos = pilotos_clasificados_q2[1]

# Mostrar los primeros 12
cont = 1
for pilotos in primeros:
    
    nombre_piloto = pilotos[1]
    print(str(cont) + ". " + nombre_piloto)
    cont += 1

# Finalmente, mostrar desde el 13avo hasta el final
cont_2 = cont
for pilotos in ultimos:
    nombre_piloto = pilotos[1]
    print(str(cont_2) + ". " + nombre_piloto)
    cont_2 += 1
