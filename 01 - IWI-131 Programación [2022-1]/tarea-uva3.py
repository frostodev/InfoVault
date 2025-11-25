from random import randint

print("Bienvenido a LolaPYlooza")
print("------- Sistema de cotización de entradas -------------")
print("")

###########################################################################

cantidad_dias = int(input("Cantidad de días: "))

# 1 solo día

if cantidad_dias == 1:
  dia_a_asistir = input("Día que desea asistir: ")
  if dia_a_asistir == "Viernes":
    valor_entrada = 50000
    print("Día 1: Viernes Valor $ 50000")
  if dia_a_asistir == "Sábado":
    valor_entrada = 75000
    print("Día 1: Sábado Valor $ 75000")
  if dia_a_asistir == "Domingo":
    valor_entrada = 60000
    print("Día 1: Domingo Valor $ 60000")

# 2 días

if cantidad_dias == 2:
  primer_dia = input("Día 1 que desea asistir: ")
  segundo_dia = input("Día 2 que desea asistir: ")

  if primer_dia == "Viernes":
    valor_entrada_primer_dia = 50000
    print("Día 1: Viernes Valor $ 50000")
  if primer_dia == "Sábado":
    valor_entrada_primer_dia = 75000
    print("Día 1: Viernes Valor $ 75000")
  if primer_dia == "Domingo":
    valor_entrada_primer_dia = 60000
    print("Día 1: Domingo Valor $ 60000")

  if segundo_dia == "Viernes":
    valor_entrada_segundo_dia = 50000
    print("Día 2: Viernes Valor $ 50000")
  if segundo_dia == "Sábado":
    valor_entrada_segundo_dia = 75000
    print("Día 2: Viernes Valor $ 75000")
  if segundo_dia == "Domingo":
    valor_entrada_segundo_dia = 60000
    print("Día 2: Domingo Valor $ 60000")

  # Aplicar el descuento
  descuento_dos_dias = (valor_entrada_primer_dia + valor_entrada_segundo_dia) * 0.2
  valor_entrada = (valor_entrada_primer_dia + valor_entrada_segundo_dia) - descuento_dos_dias
  print("Descuento por días:",round(descuento_dos_dias))
    
# 3 días

if cantidad_dias == 3:
  valor_entrada = 150000
  print("Total a pagar por todos los días $ 150000")

###########################################################################
    
# VIP
  
vip = int(input("Ingrese 1 si desea entrada VIP: "))
if vip == 1:
  recargo_vip = (valor_entrada) * 0.4
  valor_entrada = valor_entrada + recargo_vip
  print("Recargo por entrada VIP $",round(recargo_vip))
else:
  # Estacionamiento
  estacionamiento = int(input("Ingrese 1 si desea estacionamiento: "))
  if estacionamiento == 1:
    recargo_estacionamiento = 10000 * cantidad_dias
    valor_entrada = valor_entrada + recargo_estacionamiento
    print("Recargo por estacionamiento $",round(recargo_estacionamiento))

# Banco Pythonia
banco = int(input("Ingrese 1 si tiene tarjeta Banco Pythonia: "))
if banco == 1:
    descuento_banco = valor_entrada * 0.3
    valor_entrada_final = valor_entrada - descuento_banco
    print("Descuento por cliente Banco Pythonia: $",round(descuento_banco))
else:
    # Descuento por sorteo
    descuento_sorteo = randint(0,10) 
    valor_descontado = valor_entrada * (descuento_sorteo / 100)
    valor_entrada_final = valor_entrada - valor_descontado
    print("Descuento por sorteo:",round(descuento_sorteo),"%","equivalente a: $",round(valor_descontado))

# Mostrar el total a pagar
print("Total a pagar: $",round(valor_entrada_final))