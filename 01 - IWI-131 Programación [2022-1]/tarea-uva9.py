def calcular_ife_3meses(postulante):

    if postulante[2] == "F":
        # Calcular el IFE como el 45% del sueldo
        ife = 3 * (int(postulante[5]) * 0.45)

        # Si se pasa de 1 palo
        if ife > 1000000:
            ife = 1000000

    if postulante[2] == "M":
        if int(postulante[1]) >= 24 and int(postulante[1]) <= 55:
            # Calcular el IFE como el 35% del sueldo
            ife = 3 * round(int(postulante[5]) * 0.35)

            # Si se pasa de 750 lucas
            if ife > 750000:
                ife = 750000
        else:
            # Calcular el IFE como el 45% del sueldo
            ife = 3 * round(int(postulante[5]) * 0.45)

            # Si se pasa de 1 palo
            if ife > 1000000:
                ife = 1000000

    return round(ife)

# Abrir los archivos
archivo_postulantes = open("postulantes.txt", "r")
archivo_rangos = open("rangos.txt", "r")

cantidad_beneficiados = 0
total_ife_asignado = 0
edades_postulantes = []
lista_datos_postulantes = []

for postulante in archivo_postulantes:

    lista_postulante = postulante.strip("\n").split(";")
    nombre_postulante = lista_postulante[3]

    # Crear una lista con las edades de los postulantes
    edades_postulantes.append(int(lista_postulante[1]))
    ife = calcular_ife_3meses(lista_postulante)

    # Añadir el monto de ife al contador general y aumentar la cantidad de beneficiados
    # Esto es para la parte 2
    total_ife_asignado += ife
    cantidad_beneficiados += 1


    lista_datos_postulantes.append([nombre_postulante,int(lista_postulante[1]),int(lista_postulante[5]),ife])

lista_rango = []

# Crear una lista con las edades del archivo de rango
for rango in archivo_rangos:

    edad_actual = rango.strip("\n")
    lista_rango.append(int(edad_actual))

# Contar la cantidad de edades y de rangos
cantidad_edades = len(lista_rango)
cantidad_rangos = cantidad_edades + 1

# Obtener la edad mayor para poner un límite superior a los rangos
edad_mayor = max(edades_postulantes)

lista_conjunto_rangos = []

# Crear una lista con los rangos y sus nombres para los archivos
i = 0
while i <= cantidad_edades - 1:

    # En el primer rango el minimo siempre es 18
    if i == 0:
        minimo = 18
    else:
        minimo = lista_rango[i - 1] + 1

    # Verificar el valor máximo de la lista
    
    maximo = lista_rango[i]

    nombre_rango = str(minimo) + "_" + str(maximo)
    lista_conjunto_rangos.append([nombre_rango, minimo, maximo])

    # Añadir la lista final con el límite superior de edad
    if i == cantidad_edades - 1:
        minimo = lista_rango[i] + 1
        maximo = edad_mayor
        nombre_rango = str(minimo) + "_" + str(maximo)
        lista_conjunto_rangos.append([nombre_rango, minimo, maximo])

    i += 1

diccionario_cantidad_personas_rango = {}
diccionario_cantidad_monto_rango = {}

j = 0
while j < cantidad_rangos:
    archivo_resumen_actual = open("resumen_" + lista_conjunto_rangos[j][0] + ".txt", "w")
    
    contador = 0
    cantidad_monto_rango = 0
    for persona in lista_datos_postulantes:
        if persona[1] >= lista_conjunto_rangos[j][1] and persona[1] <= lista_conjunto_rangos[j][2]:
            archivo_resumen_actual.write(persona[0] + " " + str(persona[2]) + " " + str(persona[3]) + "\n")
            
            # Contar la cantidad de personas en el rango y el monto total del rango
            rango_actual = str(lista_conjunto_rangos[j][0])
            contador += 1
            diccionario_cantidad_personas_rango[rango_actual] = contador
            diccionario_cantidad_monto_rango[rango_actual] = cantidad_monto_rango + persona[3]


    archivo_resumen_actual.close()
    j += 1

#####################################################################################################

# Abrir el archivo de estadisticas
archivo_estadistica = open("estadisticas.txt", "w")

# Añadir la información inicial
archivo_estadistica.write("Resultado de la asignación del IFE \n")
archivo_estadistica.write("Monto total asignado: $" + str(total_ife_asignado) + "\n")
archivo_estadistica.write("Beneficiarios:" + str(cantidad_beneficiados) + "\n")
archivo_estadistica.write("\n")
archivo_estadistica.write("Distribución por rangos de edad: ")
archivo_estadistica.write("\n")

for rangos in lista_conjunto_rangos:
    flag = True
    
    k = 0
    while k <= len(lista_conjunto_rangos) and flag:
        # Obtener la información de los rangos
        rango = lista_conjunto_rangos[0][k]
        print(k)
        print(rango)
        minimo = rangos[1]
        maximo = rangos[2]
        

        cantidad_personas_rango = diccionario_cantidad_personas_rango[rango]
        cantidad_monto_rango = diccionario_cantidad_monto_rango[rango]

        # Calcular los porcentajes
        porcentaje_total = (cantidad_personas_rango * 100) / cantidad_beneficiados
        porcentaje_monto_asignado = (cantidad_monto_rango * 100) / total_ife_asignado

        # Añadir la información al archivo
        archivo_estadistica.write("Las personas entre " + str(minimo) + " y " + str(maximo) + " años representan un " + str(porcentaje_total) + " del total de personas. \n")
        archivo_estadistica.write("La asignación para este rango corresponde a un " + str(round(porcentaje_monto_asignado, 2)) + " del monto total asignado. \n")
        archivo_estadistica.write("\n")
       
        flag = False
        k += 1

    

print(lista_conjunto_rangos)

# Cerrar los archivos
archivo_estadistica.close()
archivo_postulantes.close()
archivo_rangos.close()