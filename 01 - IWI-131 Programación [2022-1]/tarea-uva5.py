from random import randint
from turtle import Screen, Turtle

# Parámetros de la Tortuga
pantalla = Screen()
tortuga = Turtle()
tortuga.screen.title("IWI-131: *** El Kiwi del Mote con Huesillos ***")
tortuga.speed(100)

# Posición Inicial
tortuga.penup()
tortuga.goto(-250,-50)
tortuga.pendown()

##########################################################

# Función 1: Cantidad de raciones a preparar
def cantidad_raciones(estacion,temperatura,lluvia):

    # Baja probabilidad de lluvia
    if lluvia < 0.5:
        if estacion == "verano":
            raciones = (3 * temperatura) + 20 * (1 - lluvia)
        elif estacion == "otoño":
            raciones = (2 * temperatura) + 15 * (1 - lluvia)
        elif estacion == "invierno":
            raciones = (0.5 * temperatura) + 5 * (1 - lluvia)
        elif estacion == "primavera":
            raciones = temperatura + 15 * (1 - lluvia)

    # Alta probabilidad de lluvia
    elif lluvia >= 0.5:
        if estacion == "verano":
            raciones = (2 * temperatura) + 20 * (1 - lluvia)
        elif estacion == "otoño":
            raciones = temperatura + 10 * (1 - lluvia)
        elif estacion == "invierno":
            raciones = (0.5 * temperatura)
        elif estacion == "primavera":
            raciones = temperatura + 10 * (1 - lluvia)

    return round(raciones)

# Función 2: Promoción de raciones
def regalo_raciones(estacion,raciones):
    if estacion == "verano":
        regalo = randint(1,(raciones // 10))
    elif estacion == "otoño":
        regalo = randint(1,(raciones // 8))
    elif estacion == "invierno":
        regalo = randint(1,(raciones // 5))
    elif estacion == "primavera":
        regalo = randint(1,(raciones // 7))  
    return regalo

# Función 3: Transformar el número de día a nombre de día
def transformacion_dia(dia):
    if dia == 1:
        nombre_dia = "Lunes"
    elif dia == 2:
        nombre_dia = "Martes"
    elif dia == 3:
        nombre_dia = "Miercoles"
    elif dia == 4:
        nombre_dia = "Jueves"
    elif dia == 5:
        nombre_dia = "Viernes"
    elif dia == 6:
        nombre_dia = "Sábado"
    elif dia == 7:
        nombre_dia = "Domingo"
    
    return nombre_dia

# Función 4: Dibujar una barra en el gráfico
def dibujar_barra(tortuga,x,y,dia):

    # 1 unidad = 3 pasos
    # y = cantidad de raciones

    tortuga.pendown()
    
    tortuga.left(90)
    tortuga.forward(y * 3)

    # Escribir el valor de la barra
    tortuga.penup()
    tortuga.forward(5)
    tortuga.right(90)
    tortuga.forward(35)
    tortuga.pendown()
    tortuga.write(y)
    tortuga.penup()
    tortuga.backward(35)
    tortuga.left(90)
    tortuga.backward(5)
    tortuga.pendown()

    tortuga.right(90)
    tortuga.forward(x * 3)
    tortuga.right(90)
    tortuga.forward(y * 3)
    tortuga.left(270)
    tortuga.forward(x * 3)
    tortuga.right(180)

    # Si ya se dibujó la barra del regalo, se mueve hacia la derecha
    if y == cant_regalo:
        tortuga.forward(x * 3)

    return x * y

# Función 5: Escribir la leyenda en el gráfico
def dibujar_leyenda_grafico(tortuga,leyenda):
    
    tortuga.penup()
    tortuga.right(90)
    tortuga.forward(20)
    tortuga.left(90)

    tortuga.forward(20)
    tortuga.write(leyenda)
    tortuga.backward(20)

    tortuga.left(90)
    tortuga.forward(20)
    tortuga.right(90)

    return leyenda

##########################################################

print("*** El Kiwi del Mote con Huesillos ***")
estacion = input("Estación: ")

# Ciclo para 7 días
i = 1
while i <= 7:
    print("Día",i)
    temperatura = int(input("Pronóstico de temperatura: "))
    lluvia = float(input("Probabilidad de lluvia: "))

    # Calcular las raciones
    cant_raciones = cantidad_raciones(estacion,temperatura,lluvia)
    # Calcular la cantidad de raciones de regalo
    cant_regalo = regalo_raciones(estacion,cant_raciones)
    # Transformar el día a texto
    leyenda = transformacion_dia(i)

    # Dibujar la leyenda del gráfico
    dibujar_leyenda_grafico(tortuga,leyenda)

    # Dibujar las barras del gráfico
    #             tortuga  x       y      dia
    dibujar_barra(tortuga,25,cant_raciones,i) # Barra raciones
    dibujar_barra(tortuga,25,cant_regalo,i) # Barra raciones de regalo

    print("Se producirá",cant_raciones,"raciones; se regalará",cant_regalo,"para promoción")
    i += 1

# Para que la ventana de la tortuga no se cierre
pantalla.exitonclick()