import math

print("*** Kiwi ayuda al Coyote ***")
print("Ingrese los siguientes datos:")
print("")

coordenadax_correcaminos = float(input("Coordenada x del Correcaminos: "))
velocidad_inicial_lanz = float(input("Velocidad inicial de lanzamiento [kms/h]: "))
angulo_lanz_grados = float(input("Ángulo de lanzamiento, expresado [grados]: "))

coordenadax_coyote = float(input("Coordenada x del lanzamiento (Coyote): "))
coordenaday_coyote = float(input("Coordenada y del lanzamiento (Coyote): "))

angulo_lanz_rad = ((angulo_lanz_grados * math.pi) / 180)
velocidad_inicial_lanz_ms = velocidad_inicial_lanz * 10 / 36

velocidad_eje_y = (velocidad_inicial_lanz_ms * math.sin(angulo_lanz_rad))
velocidad_eje_x = (velocidad_inicial_lanz_ms * math.cos(angulo_lanz_rad))

discriminante = (velocidad_eje_y ** 2) - (4 * -4.9 * coordenaday_coyote)
tiempo_impact = ((-1 * velocidad_eje_y) - math.sqrt(discriminante)) / (2 * -4.9)

coordenadax_instante = coordenadax_coyote + (velocidad_eje_x * tiempo_impact)
coordenaday_instante = coordenaday_coyote + (velocidad_eje_y * tiempo_impact) - (4.9 * (tiempo_impact ** 2)) 

print("")
print("Valores ajustados: ")
print("Velocidad:",round((velocidad_inicial_lanz_ms),5),"[m/s]")
print("Ángulo de Lanzamiento:",round((angulo_lanz_rad),5),"[rad]")
print("vx =",round((velocidad_eje_x),5),"[m/s]")
print("vy =",round((velocidad_eje_y),5),"[m/s]")

#################################################################################

print("")
print("Evaluación del lanzamiento: ")
print("Tiempo de impacto estimado:",round((tiempo_impact),5),"[s]")

posicion_x = coordenadax_coyote + (velocidad_eje_x * 0)
posicion_y = (coordenaday_coyote + (velocidad_eje_y * 0)) - 4.9 * (0 ** 2)

print("En tiempo 0 el proyectil se encuentra en:",round((posicion_x),5),round((posicion_y),5))

posicion_x = coordenadax_coyote + (velocidad_eje_x * 0.1)
posicion_y = (coordenaday_coyote + (velocidad_eje_y * 0.1)) - 4.9 * (0.1 ** 2)

print("En tiempo 0.1 el proyectil se encuentra en:",round((posicion_x),5),round((posicion_y),5))

posicion_x = coordenadax_coyote + (velocidad_eje_x * 0.2)
posicion_y = (coordenaday_coyote + (velocidad_eje_y * 0.2)) - 4.9 * (0.2 ** 2)

print("En tiempo 0.2 el proyectil se encuentra en:",round((posicion_x),5),round((posicion_y),5))

posicion_x = coordenadax_coyote + (velocidad_eje_x * 0.3)
posicion_y = (coordenaday_coyote + (velocidad_eje_y * 0.3)) - 4.9 * (0.3 ** 2)

print("En tiempo 0.3 el proyectil se encuentra en:",round((posicion_x),5),round((posicion_y),5))

#################################################################################

tiempo_nuevo = tiempo_impact - 0.3
posicion_tiempo_nuevo_x = coordenadax_coyote + (velocidad_eje_x * tiempo_nuevo)
posicion_tiempo_nuevo_y = (coordenaday_coyote + (velocidad_eje_y * tiempo_nuevo)) - 4.9 * (tiempo_nuevo ** 2)

print("En tiempo",round((tiempo_nuevo),5),"el proyectil se encuentra en:",round((posicion_tiempo_nuevo_x),5),round((posicion_tiempo_nuevo_y),5))

tiempo_nuevo = tiempo_impact - 0.2
posicion_tiempo_nuevo_x = coordenadax_coyote + (velocidad_eje_x * tiempo_nuevo)
posicion_tiempo_nuevo_y = (coordenaday_coyote + (velocidad_eje_y * tiempo_nuevo)) - 4.9 * (tiempo_nuevo ** 2)

print("En tiempo",round((tiempo_nuevo),5),"el proyectil se encuentra en:",round((posicion_tiempo_nuevo_x),5),round((posicion_tiempo_nuevo_y),5))

tiempo_nuevo = tiempo_impact - 0.1
posicion_tiempo_nuevo_x = coordenadax_coyote + (velocidad_eje_x * tiempo_nuevo)
posicion_tiempo_nuevo_y = (coordenaday_coyote + (velocidad_eje_y * tiempo_nuevo)) - 4.9 * (tiempo_nuevo ** 2)

print("En tiempo",round((tiempo_nuevo),5),"el proyectil se encuentra en:",round((posicion_tiempo_nuevo_x),5),round((posicion_tiempo_nuevo_y),5))

#################################################################################

coordenada_impacto = coordenadax_coyote + (velocidad_eje_x * tiempo_impact)
distancia_fallo = coordenada_impacto - coordenadax_correcaminos

print("Proyectil impacta en coordenada X:",round((coordenada_impacto),5))
print("Se falló al correcaminos por:",round((distancia_fallo),5),"[m]")