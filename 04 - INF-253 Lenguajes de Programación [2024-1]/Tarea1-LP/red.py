import re
import math

# Clase para un nodo del grafo. Esta representa una Empresa, con sus respectivos atributos.
class Empresa:
    def __init__(self, nombre, rut, productos):
        '''
        # ***
        # self: Empresa
        # nombre: string
        # rut: string
        # productos: Lista de tuplas
        # ***
        # None
        # ***
        # Constructor de la clase Empresa. Este asigna los atributos al nodo Empresa.
        '''
        
        self.nombre = nombre
        self.rut = rut
        self.productos = productos
        self.conexiones = {} 

# Clase para el grafo. Esta utiliza un diccionario de nodos Empresa.
class GrafoEmpresas:
    def __init__(self):
        '''
        # ***
        # self: GrafoEmpresas
        # ***
        # None
        # ***
        # Constructor de la clase GrafoEmpresas. Este crea el diccionario que contiene a los Nodos.
        # Inicializar el diccionario que contiene a las empresas
        '''
        
        self.empresas = {}
        

    def agregarEmpresa(self, empresa):
        '''
        ***
        self: GrafoEmpresas
        empresa: Empresa
        ***
        None si la empresa se agregó, False si ya existen los mismos datos en otra empresa
        ***
        Método que agrega un nuevo nodo al grafo, antes verificando que los datos no estén repetidos.
        Verificar si el nombre o el RUT ya existen en otra empresa
        '''
        
        for existing_empresa in self.empresas.values():
            if empresa.nombre == existing_empresa.nombre or empresa.rut == existing_empresa.rut:
                return False
        
        # Agregar la empresa al grafo
        self.empresas[empresa.nombre] = empresa
        

    def agregarConexion(self, identificador_origen, identificador_destino, conexion):
        '''
        ***
        self: GrafoEmpresas
        identificador_origen: string
        identificador_destino: string
        conexion: tupla
        ***
        None si todo sale bien, False si se detecta un ciclo
        ***
        Método que intenta agregar una conexión (puente) entre las empresas (nodos). El peso
        El peso asociado a las conexiones entre nodos es la tupla que contiene los datos de la venta
        '''
        
        nombre_origen = identificador_origen
        nombre_destino = identificador_destino

        # Realizar la conversion nombre -> rut o rut -> nombre
        for empresa in self.empresas.values():
            if identificador_origen == empresa.rut:
                nombre_origen = empresa.nombre
            if identificador_destino == empresa.rut:
                nombre_destino = empresa.nombre

        if nombre_origen in self.empresas and nombre_destino in self.empresas:
            # Verificar si la conexión crearía un ciclo en el grafo
            if self.detectarCicloConexion(nombre_origen, nombre_destino):
                return False

            # Agregar conexión de origen a destino con su valor asociado
            if nombre_destino not in self.empresas[nombre_origen].conexiones:
                self.empresas[nombre_origen].conexiones[nombre_destino] = [conexion]
            else:
                self.empresas[nombre_origen].conexiones[nombre_destino].append(conexion)
                

    def detectarCicloConexion(self, nombre_origen, nombre_destino):
        '''
        ***
        self: GrafoEmpresas
        nombre_origen: string
        nombre_destino: string
        ***
        True si existe un ciclo entre empresas, False si no existe un ciclo.
        ***
        Método que prepara para detectar ciclos usando DFS (mediante otro método) 
        '''
        
        visitados = set()
        en_proceso = set()

        # Realizar una búsqueda DFS desde el destino hacia el origen
        return self.dfsDetectarCiclo(nombre_destino, nombre_origen, visitados, en_proceso)
        

    def dfsDetectarCiclo(self, nombre_empresa, nombre_origen, visitados, en_proceso):
        '''
        ***
        self: GrafoEmpresas
        nombre_empresa: string
        nombre_origen: string
        visitados: set()
        en_proceso: set()
        ***
        True si existe un ciclo entre empresas, False si no existe un ciclo.
        ***
        Método que aplica el algoritmo de DFS para comprobar si existen ciclos en las ventas de la empresa.
        '''
        
        visitados.add(nombre_empresa)
        en_proceso.add(nombre_empresa)

        for empresa_conectada in self.empresas[nombre_empresa].conexiones:
            if empresa_conectada == nombre_origen:
                return True  # Se encontró un ciclo
            if empresa_conectada not in visitados:
                if self.dfsDetectarCiclo(empresa_conectada, nombre_origen, visitados, en_proceso):
                    return True
            elif empresa_conectada in en_proceso:
                return True

        en_proceso.remove(nombre_empresa)
        return False
        

    def empresaPorNombre(self, nombre):
        '''
        ***
        self: GrafoEmpresas
        nombre: string
        ***
        True si la empresa con tal nombre existe, False en caso contrario.
        ***
        Método que confirma si existe una empresa con tal nombre.
        '''
        return nombre in self.empresas


    def empresaPorRut(self, rut):
        '''
        ***
        self: GrafoEmpresas
        rut: string
        ***
        True si el rut existe en el grafo, False en caso contrario
        ***
        Método que confirma si existe un rut en al grafo.
        '''
        
        for empresa in self.empresas.values():
            if empresa.rut == rut:
                return True
        return False


    def buscarIDProductoPorNombre(self, nombre_producto):
        '''
        ***
        self: GrafoEmpresas
        identificador: string
        ***
        Un string que corresponde al id del producto si este se encuentra, o si mismo si no se encuentra.
        ***
        Método que busca el id de un producto a partir de su nombre.
        '''
        for empresa in self.empresas.values():
            for producto in empresa.productos:
                if producto[2] == nombre_producto:
                    return producto[0]  # Devuelve la ID del producto si se encuentra
        return nombre_producto  # Devuelve el mismo nombre si el producto no se encuentra en ninguna empresa    
    
    
    def buscarNombreProductoPorID(self, id_producto):
        '''     
        ***
        self: GrafoEmpresas
        identificador: string
        ***
        Un string que corresponde al nombre del producto si este se encuentra, o si mismo si no se encuentra.
        ***
        Método que busca el nombre de un producto a partir de su id.
        '''
        
        for empresa in self.empresas.values():
            for producto in empresa.productos:
                if producto[0] == id_producto:
                    return producto[2]  # Devuelve el nombre del producto si se encuentra
        return id_producto  # Devuelve el mismo ID si este no se encuentra en ninguna empresa


    def obtenerInformacionEmpresa(self, identificador):
        '''
        ***
        self: GrafoEmpresas
        identificador: string
        ***
        Un string que contiene los datos de la empresa a escribir
        ***
        Método que busca los datos de una empresa.
        '''
        
        # Verificar si el identificador coincide con el nombre o el rut de alguna empresa
        for empresa in self.empresas.values():
            if identificador == empresa.nombre or identificador == empresa.rut:
                # Formatear la información de los productos
                informacion_empresa = f"- {empresa.nombre}\n- {empresa.rut}\n- [\n"
                for producto in empresa.productos:
                    precioProducto = self.precioProducto(producto[2])
                    producto_info = f"#{producto[0]}-${precioProducto}-{producto[2]}"
                    informacion_empresa += f"{producto_info}\n"
                informacion_empresa += "]\n"
                return informacion_empresa


    def obtenerInformacionVenta(self, identificador):
        '''
        ***
        self: GrafoEmpresas
        identificador: string
        ***
        Un string que contiene los datos de las ventas a escribir
        ***
        Método que busca los datos de las ventas de una empresa.
        '''
        
        informacionVenta = ""

        for empresa in self.empresas.values():
            if identificador == empresa.nombre or identificador == empresa.rut:
                ventas = []
                for empresa_conectada, valor in empresa.conexiones.items():
                    for venta in valor:
                        producto, precio, cantidad = venta
                        id_producto = self.buscarIDProductoPorNombre(producto)
                        precio_final = int(math.ceil(float(precio.replace(".", ""))) * float(cantidad))
                        precio_formato = f"${precio}"
                        precio_final_formato = f"${precio_final}"
                        venta_linea = f"#{id_producto}-{precio_formato}-{producto} x{cantidad} {precio_final_formato} -> {empresa_conectada}"
                        ventas.append(venta_linea)

                informacionVenta = f"- {empresa.nombre}\n- {empresa.rut}\n- [\n"
                informacionVenta += "\n".join(ventas)
                informacionVenta += "\n]\n"
                break  # Detener el loop después de encontrar la empresa correspondiente

        return informacionVenta
        
        
    def precioProducto(self, identificador_producto):
        '''
        ***
        self: GrafoEmpresas
        identificador_producto: string
        ***
        Un string que corresponde al precio del producto, o None si el producto no existe
        ***
        Método que recorre el grafo en busca de un producto, para saber su precio.
        '''
        
        for empresa in self.empresas.values():
            for producto in empresa.productos:
                if identificador_producto in producto or identificador_producto == producto[2]:
                    return producto[1]  # Devuelve el precio del producto
        return None  # Devuelve None si el producto no se encuentra en ninguna empresa
    
    
    def buscarMP(self, identificador):
        '''
        ***
        self: GrafoEmpresas
        identificador: string
        ***
        Un string que contiene los datos de la línea de producción a escribir, o False si falla.
        ***
        Método que busca los participantes en la línea de producción de una empresa.
        '''
        
        # Verificar si los identificadores corresponden a nombres o RUTs de empresas
        if self.empresaPorNombre(identificador):
            empresa_destino = identificador
        elif self.empresaPorRut(identificador):
            for empresa in self.empresas.values():
                if empresa.rut == identificador:
                    empresa_destino = empresa.nombre
                    break
        else:
            return False
        
        # Verificar si la empresa de destino ya es productora de materia prima
        if empresa_destino in self.empresas:
            for empresa in self.empresas.values():
                if empresa_destino in empresa.conexiones:
                    # La empresa recibe ventas directas, no es productora de MP
                    break
            else:
                # La empresa no recibe ventas directas, por lo tanto ya es productora de MP
                return False
        
        # Texto para almacenar las empresas y productos en la línea de producción
        linea_produccion_texto = f"- {empresa_destino}, {self.empresas[empresa_destino].rut}\n- [\n"

        def buscarConexionesHaciaDestino(empresa_actual, visitados):
            '''
            ***
            empresa_actual: Empresa
            visitados: set()
            ***
            None
            ***
            Función auxiliar para explorar las conexiones de venta hacia la empresa de destino
            '''
            
            nonlocal linea_produccion_texto
            visitados.add(empresa_actual)

            # Buscar conexiones de venta hacia la empresa actual
            for empresa, conexiones in self.empresas.items():
                for empresa_conectada, ventas in conexiones.conexiones.items():
                    if empresa_conectada == empresa_actual and empresa not in visitados:
                        for venta in ventas:
                            id_producto = self.buscarIDProductoPorNombre(venta[0])
                            precioProducto = venta[1]
                            nombre_producto = venta[0]
                            producto_info = f"#{id_producto}-${precioProducto}-{nombre_producto} {empresa}"
                            linea_produccion_texto += f"{producto_info}\n"
                        buscarConexionesHaciaDestino(empresa, visitados)

        # Buscar conexiones de venta para la empresa de destino
        if empresa_destino in self.empresas:
            for empresa in self.empresas:
                if empresa_destino in self.empresas[empresa].conexiones:
                    buscarConexionesHaciaDestino(empresa, set())


        linea_produccion_texto += "] \n"
        return linea_produccion_texto

 
############################################################################################

def verificarDigitoVerificador(rut):
    '''
    ***
    rut: string
    ***
    Retorna un string que corresponde al dígito verificador.
    ***
    Función que calcula el dígito verificador a partir de un rut.
    '''
    
    # Extraer el número del RUT sin el dígito verificador
    numero_rut = rut[:-2]
    
    # Contador inicializado en 2 para multiplicar los números del RUT
    contador = 2
    
    # Inicializar la suma en 0
    suma = 0
    
    # Lista para almacenar los números del RUT en orden invertido
    reves = []
    
    # Invertir los dígitos del RUT y almacenarlos en la lista 'reves'
    for numero in numero_rut:
        reves.insert(0, numero)
    
    # Iterar sobre los números invertidos del RUT
    for numero in reves:
        numero = int(numero)  # Convertir el número a entero
        if contador == 8:  # Si el contador llega a 8, se reinicia a 2
            contador = 2
        suma += (numero * contador)  # Sumar el resultado de la multiplicación al total
        contador += 1  # Incrementar el contador
    
    # Calcular el dígito verificador
    digito_verificador = suma % 11
    digito_verificador = 11 - digito_verificador
    if digito_verificador == 11:  # Si el resultado es 11, el dígito verificador es 0
        digito_verificador = 0
    elif digito_verificador == 10:  # Si el resultado es 10, el dígito verificador es 'k'
        digito_verificador = 'k'
    
    # Retornar el dígito verificador como un string
    return str(digito_verificador)


def agregarEmpresaGrafo(grafo, tuplaEmpresa):
    '''
    ***
    grafo: GrafoEmpresas
    tuplaEmpresa: Tupla
    ***
    None si la empresa se agregó al grafo, False si los datos ya existen en el grafo y 0 si el rut es inválido.
    ***
    Función que intenta agregar un nuevo nodo Empresa, verificando antes si los datos de esta ya existen
    '''
    
    # Separar la tupla que contiene los datos de la empresa
    nombreEmpresa = tuplaEmpresa[0]
    rutEmpresa = tuplaEmpresa[1]
    productosEmpresa = tuplaEmpresa[2]
    
    # Verificar si el rut tiene un dígito verificador válido
    if rutEmpresa[-1] == verificarDigitoVerificador(rutEmpresa):
    
        # Intentar agregar la empresa al grafo. Si no se puede, es porque otra empresa con los mismos datos ya existen
        # en cuyo caso, simplemente retornar False para que analizarComando() se encargue
        if grafo.agregarEmpresa(Empresa(nombreEmpresa, rutEmpresa, productosEmpresa)) == False:
            return False
        else:
            # Los datos no se repiten. Devolver None.
            return
    else:
        # Rut inválido
        return 1
    

def obtenerDatosEmpresa(string):
    '''
    ***
    string: string
    ***
    Tupla, esta contiene los datos extraídos de la venta.
    ***
    Función que analiza los datos de un string válido de venta, y los almacena en una tupla
    '''
    
    # Expresión regular para encontrar el nombre de la empresa, su RUT y la lista de productos
    info_regex = r'^([A-Z][^.-]*(?:-[^.-]+)*)\.(\d+)-([^\.\[]+)\.\[([^\[\]]*)\]$'
    info_match = re.match(info_regex, string)

    if info_match:
        nombre_empresa = info_match.group(1).strip()
        rut_empresa = f"{info_match.group(2)}-{info_match.group(3)}"
        productos_str = info_match.group(4)

        # Extraer los productos de la cadena utilizando una expresión regular
        productos = re.findall(r'#([A-Z0-9]+)-\$(\d+(?:\.\d{1,3})+|\d+)-([^\[\],]+)', productos_str)
    else:
        nombre_empresa = ""
        rut_empresa = ""
        productos = []
    
    return nombre_empresa, rut_empresa, productos
    

def obtenerDatosVenta(string):
    '''
    ***
    string: string
    ***
    Tupla, esta contiene los datos extraídos de la venta. False si no hay match.
    ***
    Función que analiza los datos de un string válido de venta, y los almacena en una tupla
    '''
    
    # Definir el patrón de expresión regular
    patron = r'\((?P<empresa_vendedora>[\w#-]+)\.(?P<producto>[\w# .-]+)->(?P<empresa_compradora>[\w# -]+)\.x(?P<cantidad>\d+(\.\d+)?)\)'

    # Hacer coincidir el patrón con el string proporcionado
    match = re.match(patron, string)

    # Verificar si hubo una coincidencia
    if match:
        # Extraer los grupos de la coincidencia
        empresa_vendedora = match.group('empresa_vendedora').strip()
        producto = match.group('producto').strip()
        empresa_compradora = match.group('empresa_compradora').strip()
        cantidad = float(match.group('cantidad'))

        # Devolver la información extraída como tupla
        return empresa_vendedora, producto, empresa_compradora, cantidad
    else:
        # Si no hay coincidencia, devolver False
        return False
        

def analizarValidezEmpresa(string):
    '''
    ***
    string: string
    ***
    booleano, True si la empresa es válida según la regex, False en caso contrario
    ***
    Función que analiza la validez de una empresa mediante una expresión regular
    '''
    
    regex = r'^\s*([A-Z][^.]*(?:-[^.]*)*)\.(\d+)-([^\.\[]+)\.\[([^\[\]]*)\]\s*$'
    if re.match(regex, string):
        return True
    else:
        return False


def analizarValidezVenta(string):
    '''
    ***
    string: string
    ***
    booleano, True si la venta es válida según la regex, False en caso contrario
    ***
    Función que analiza la validez de una venta mediante una expresión regular
    '''

    # Definir el patrón de expresión regular
    patron = r'\((?P<empresa_vendedora>[\w#-]+)\.(?P<producto>[\w# .-]+)->(?P<empresa_compradora>[\w# -]+)\.x(?P<cantidad>\d+(\.\d+)?)\)'

    # Verificar si la cadena coincide con el patrón
    match = re.match(patron, string)
    if match:
        cantidad = match.group('cantidad')
        try:
            cantidad_float = float(cantidad)
            if cantidad_float.is_integer():
                if cantidad.endswith('.0'):
                    return False
            return True
        except ValueError:
            return False
    else:
        return False
        

def analizarComando(comando, grafo, archivoOutput):
    '''
    ***
    comando: string
    grafo: GrafoEmpresas
    archivoOutput: file
    ***
    Booleano, True si se ejecutó correctamente, False si el comando tiene errores
    ***
    Función que recibe un string comando y lo analiza   
    '''
    
    # Si el comando comienza con un paréntesis, es una venta
    if comando.startswith("("):   
        if analizarValidezVenta(comando) == True:
            
            # Obtenemos la información del comando
            datosVenta = obtenerDatosVenta(comando)

            # Verificamos si los nombres/rut son válidos
            # Primera Empresa
            if grafo.empresaPorRut(datosVenta[0]) == False:
                if grafo.empresaPorNombre(datosVenta[0]) == False:
                    archivoOutput.write("No se pudo crear conexión, no existe tal " + datosVenta[0] + "\n")
                    return True

            # Segunda empresa
            if grafo.empresaPorRut(datosVenta[2]) == False:
                if grafo.empresaPorNombre(datosVenta[2]) == False:
                    archivoOutput.write("No se pudo crear conexión, no existe tal " + datosVenta[2] + "\n")
                    return True
            
            # Si todos las pruebas pasaron, creamos la conexión entre empresas
            vendedor = datosVenta[0]
            comprador = datosVenta[2]
            producto = datosVenta[1]
            cantidad = datosVenta[3]

            # Asegurarnos que el id se guarde sin el #, por conveniencia
            if producto.startswith("#"):
                producto = producto[1:]

            precio = grafo.precioProducto(producto)
            
            # Intentar agregar la venta, en caso de que devuelva False es porque la venta generó un ciclo.
            if grafo.agregarConexion(vendedor, comprador, (grafo.buscarNombreProductoPorID(producto), precio, str(cantidad))) == False:
                # Si genera un ciclo, escribirlo en output.
                archivoOutput.write("La venta (" + vendedor + ".#" + producto + "->" + comprador + "." + "x" + str(cantidad) + ")" + " genera un ciclo. \n")
            else:
                return True
        
        # Si la venta tiene una sintaxis incorrecta
        else:
            return False
    
    # Comando ver_empresa
    elif comando.startswith("ver_empresa"):

        # Encontrar el espacio del string
        espacio = comando.index(" ")
        
        # Separar el comando del string y verificar su existencia
        nombreRutEmpresa = comando[espacio + 1:].strip()
        if grafo.empresaPorRut(nombreRutEmpresa) or grafo.empresaPorNombre(nombreRutEmpresa):
            # Escribir el archivo con la información requerida
            archivoOutput.write("VER EMPRESA: \n")
            archivoOutput.write(grafo.obtenerInformacionEmpresa(nombreRutEmpresa))
            return True
            
        else:
            # En caso que no exista la empresa
            archivoOutput.write("No existe la empresa con el nombre/Rut dado")
            return True
    

    # Comando ver_ventas
    elif comando.startswith("ver_ventas"):
        # Encontrar el espacio del string
        espacio = comando.index(" ")
        
        # Separar el comando del string y verificar su existencia
        nombreRutEmpresa = comando[espacio + 1:].strip()
        
        if grafo.empresaPorRut(nombreRutEmpresa) or grafo.empresaPorNombre(nombreRutEmpresa):
            # Escribir el archivo con la información requerida
            archivoOutput.write("VER VENTAS: \n")
            archivoOutput.write(grafo.obtenerInformacionVenta(nombreRutEmpresa))
            return True
            
        else:
            # En caso que no exista la empresa
            archivoOutput.write("No existe la empresa con el nombre/Rut dado")
            return True
        
        
    # Comando buscarMP
    elif comando.startswith("buscar_MP"):
        # Encontrar el espacio del string
        espacio = comando.index(" ")
        
        # Separar el comando del string y verificar su existencia
        nombreRutEmpresa = comando[espacio + 1:].strip()
        
        if grafo.empresaPorRut(nombreRutEmpresa) or grafo.empresaPorNombre(nombreRutEmpresa):
            
            comandoBuscarMP = grafo.buscarMP(nombreRutEmpresa)
            if comandoBuscarMP == False:
                archivoOutput.write("La empresa " + nombreRutEmpresa + " ya es productor de MP \n")
                
            else:
                archivoOutput.write("BUSCAR MP: \n")
                archivoOutput.write(comandoBuscarMP)
                return True
        else:
            # En caso que no exista la empresa
            archivoOutput.write("No existe la empresa con el nombre/Rut dado")
            return True

    # Ahora, en caso de no ser explícitamente un comando, analizamos si corresponde a una definición de empresa
    # En caso de que no, entonces se invalida
    elif analizarValidezEmpresa(comando):

        # Obtenemos los datos de la empresa desde el string, y luego los ingresamos al grafo
        datosEmpresa = obtenerDatosEmpresa(comando)
        
        # En caso de que ya exista una empresa con el mismo rut/nombre, indicar en el output e ignorar
        if agregarEmpresaGrafo(grafo, datosEmpresa) == False:
            archivoOutput.write(datosEmpresa[1] + " ya se encuentra en la linea de producción \n")
        
        # En caso de que el rut sea inválido, es un error de sintáxis.
        elif agregarEmpresaGrafo(grafo, datosEmpresa) == 1:
            return False
    
    # En caso de que fuese un comando inválido, retornamos False, y el comando pasa al archivo de errores.
    else:
        return False
    
    return True


############################################################################################

def main():
    '''
    ***
    Sin parámetros
    ***
    None
    ***
    Función principal que abre los archivos.
    '''
    # Crear el grafo que contiene a las empresas
    grafoEmpresas = GrafoEmpresas()
    
    # Leer el archivo input
    archivoInput = open("input.txt", "r")
    
    # Crear el archivo de errores
    archivoErrores = open("errores.txt", "w")
    
    # Crear el archivo output
    archivoOutput = open("output.txt", "w")
    
    # Analizar cada linea del archivo. La función analizarComando() solo retorna False si
    # el comando no corresponde a la sintáxis de alguno de los comandos predeterminados.
    # En ese caso, este se escribe en el archivo de errores.
    
    for comando in archivoInput.readlines():
        # Analizar el comando
        if analizarComando(comando, grafoEmpresas, archivoOutput) == False:
            archivoErrores.write(comando)
    
    # Cerrar el archivo input
    archivoInput.close()
    archivoErrores.close()
    archivoOutput.close()

# Llamar a la función principal
if __name__ == "__main__":
    main()