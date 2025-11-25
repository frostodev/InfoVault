# Sergio Cárcamo Naranjo
# Rol: 202273512-0
# Ayudante: Kimberly Álvarez 
# (te kiero kim)

def comunas_con_mas_oferta(lista, operacion):

    lista_comunas = []
    lista_final = []
    diccionario_cantidades = {}

    # Hacer la operación en mayuscula, asi se asegura que siempre funcione
    operacion = operacion.upper()

    for lista_propiedad in lista:
        # Añadir las comunas a una lista general, verificando que tipo de operación se está usando.
        if lista_propiedad[2] == operacion:
            lista_comunas.append(lista_propiedad[3])

    # Contar la cantidad de comunas y dejarlas en un diccionario y en una lista
    for comuna in lista_comunas:
        cantidad = lista_comunas.count(comuna)
        diccionario_cantidades[str(comuna)] = cantidad

    # Ordenar los valores del diccionario
    diccionario_ordenado = sorted(diccionario_cantidades)
    diccionario_ordenado.reverse()

    # Como no se puede asegurar la cantidad de valores, se hace un ciclo while para los 3 mayores valores
    flag = True
    i = 0
    while i < len(diccionario_ordenado):

        if len(diccionario_ordenado) >= 3 and flag:
            lista_final.append(diccionario_ordenado[0])
            lista_final.append(diccionario_ordenado[1])
            lista_final.append(diccionario_ordenado[2])
            flag = False
        
        if len(diccionario_ordenado) >= 2 and flag:
            lista_final.append(diccionario_ordenado[0])
            lista_final.append(diccionario_ordenado[1])
            flag = False

        if len(diccionario_ordenado) >= 1 and flag:
            lista_final.append(diccionario_ordenado[0])
            flag = False

        i += 1
    
    return lista_final

########################################################################################################

def filtrar(lista, tipo, operacion, min_dormitorios, min_m2):

    lista_ids = []
    lista_precios_ordenada = []
    lista_precios = []
    diccionario = {}

    # Verificar que se cumplan todas los requisitos
    for lista_id in lista:
        if lista_id[1] == tipo:
            if lista_id[2] == operacion:
                if lista_id[4] >= min_dormitorios:
                    if lista_id[7] >= min_m2:

                        # Si se cumple todo, añadir la propiedad a una lista
                        lista_ids.append(lista_id)
                        
    # Separar los precios en otra lista
    for elementos in lista_ids:
        precio_actual = elementos[10]
        lista_precios.append(precio_actual)
    
    # Ordenar la lista de precios
    lista_precios.sort()

    # Crear una nueva lista con los precios ordenados
    k = 0
    while k < len(lista_precios):

        # Comparar los precios de la lista anterior con la original, así se puede saber que precio le corresponde a que propiedad
        if lista_precios[k] == lista_ids[k][10]:
            # Añadir el precio
            lista_precios_ordenada.append(lista_ids[k][10])

            m = 0
            while m < len(lista_precios):
                lista_precios_ordenada.append(lista_precios[m])
                m += 1
            
        k += 1
            
    i = 0
    while i < len(lista_ids):
        lista_actual = lista_ids[i]
        comuna = lista_actual[3]
        lista_temporal = []

        # Añadir los precios al principio de la lista

        lista_temporal.append(lista_precios_ordenada[i])

        # Añadir a una lista nueva todos los elementos de la lista actual menos la comuna
        j = 0
        while j < 10:
            if j != 3:
                lista_temporal.append(lista_actual[j])
            j += 1
        
        # Crear una lista vacía si es que no está la comuna en el diccionario
        if comuna not in diccionario:
            diccionario[comuna] = []

        # Añadir la lista sin la comuna al diccionario
        diccionario[comuna].append(lista_temporal)
        
        i += 1

    return diccionario

########################################################################################################

def buscar(oferta, solicitudes):

    lista_personas = []
    lista_final = []

    # Crear una lista con los nombres de las personas
    g = 0
    while g < len(solicitudes):
        lista_personas.append(solicitudes[g][0])
        g += 1

    # Crear una lista gigante con todas las ofertas filtradas para cada persona
    z = 0
    while z < len(lista_personas):
        
        lista_filtrada = []

        # Determinar los requisitos
        persona = solicitudes[z][0]
        requisitos = solicitudes[z][1]
            
        tipo = requisitos[0]
        operacion = requisitos[1]
        min_dormitorios = requisitos[2]
        min_m2 = requisitos[3]

        # Filtrar las ofertas para obtener las pedidas, usando la función 2
        oferta_posible = filtrar(oferta, tipo, operacion, min_dormitorios, min_m2)

        # Añadir todo a la lista principal
        for comuna in oferta_posible:
            lista_filtrada.append(comuna)
            lista_filtrada.append(oferta_posible[comuna])

        lista_final.append([persona, lista_filtrada])

        z += 1

    return lista_final
