from math import sqrt
from random import randint

print("Bienvenid@s a GimnasIWI!")
meta_calorias = int(input("Ingrese meta en calorias: "))

menu = False
iterar = True

# Definimos el valor inicial de la cantidad de calorías quemadas.
calorias_quemadas = 0

while iterar:
  print("Ingrese ejercicio: ")
  print("(1) Sentadillas sumo")
  print("(2) Plancha abdominal")
  print("(3) Press frances")
  print("(4) Me canse")
  ejercicio = int(input(""))

  # Verificar que el número del ejercicio esté en el rango
  if ejercicio > 4 or ejercicio < 1:
    # Si no está dentro del rango
    print("Ingrese una opción válida")
  else:
    menu = True

  # Si está entonces continuamos
  while menu:
    ############################ Ejercicios ############################
        
    # Sentadillas Sumo
    if ejercicio == 1:
      print("SENTADILLAS SUMO")
      repeticiones_sentadillas = int(input("Cuantas repeticiones? "))
      calorias_quemadas_sentadillas = repeticiones_sentadillas * randint(3,7)
      calorias_quemadas = calorias_quemadas + calorias_quemadas_sentadillas

      print("Calorias quemadas con sentadillas:",calorias_quemadas_sentadillas)
      print("Calorías quemadas hasta el momento:",calorias_quemadas)

      # Se llegó al objetivo?
      if calorias_quemadas > meta_calorias:
        iterar = False

      menu = False

    # Plancha Abdominal
    elif ejercicio == 2:
      print("PLANCHA ABDOMINAL")
      tiempo_plancha = int(input("Cuantos segundos? "))

      # Valor inicial de n
      n = 1
      # Valor inicial sumatoria
      sumatoria = 0

      while n <= tiempo_plancha:
        j = n
        factorial = 1
        while j > 1:
          factorial *= j
          j -=1
        sumatoria += 4 ** n / factorial
        n += 1

      calorias_quemadas = calorias_quemadas + round(sumatoria)

      print("Calorías quemadas con plancha:",round(sumatoria))
      print("Calorías quemadas hasta el momento:",calorias_quemadas)

      # Se llegó al objetivo?
      if calorias_quemadas > meta_calorias:
        iterar = False

      menu = False
 
    # Press Francés
    elif ejercicio == 3:
      print("PRESS FRANCÉS")     
      repeticiones_press = int(input("Cuantas repeticiones? "))
      kilos = int(input("Cuantos kilos? "))

      # Determinar n y el primer termino

      if repeticiones_press > kilos:
          n = kilos
          primer_termino = repeticiones_press
      else:
          n = repeticiones_press
          primer_termino = kilos

      i = 1
      # El primer valor de la sumatoria es 0
      calculo = 0

      # Resolver la sumatoria
      while i <= n:
          calculo += (1 + sqrt(calculo))
          i += 1
      
      calorias_quemadas += round(calculo)

      print("Calorías quemadas con Press francés:",round(calculo))
      print("Calorías quemadas hasta el momento:",calorias_quemadas)

      # Se llegó al objetivo?
      if calorias_quemadas > meta_calorias:
        iterar = False

      menu = False
      #####################################################################
    
    # Me cansé
    elif ejercicio == 4:
      menu = False
      iterar = False


# Comparación calorías totales quemadas con objetivo

# Si se llega al objetivo
if calorias_quemadas >= meta_calorias:
  print("************")
  print("Meta cumplida! Quemaste",calorias_quemadas,"de un total de",meta_calorias)

# Si no se llega
else:
  print("************")
  print("A descansar! Quemaste",calorias_quemadas,"calorías")
        