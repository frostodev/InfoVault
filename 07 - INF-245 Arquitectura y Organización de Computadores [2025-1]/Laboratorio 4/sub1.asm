.data
n:          	.word 0		# Valor temporal de n
k:          	.word 0		# Valor temporal de K
temps:       	.space 400	# Espacio para 100 enteros (4 bytes c/u)
sumas:       	.space 400	# Máx 100 sumas posibles (4 bytes c/u)
inicio_tend:	.word -1	# Índice donde comienza la tendencia, -1 si no hay

# Strings para entrada/salida, etc.
entrada_n:   		.asciz "Ingresar n (5-100): "
entrada_k:   		.asciz "Ingresar k (k < n): "
entrada_temps: 		.asciz "Ingresar temperaturas (una por línea): "
str_bloques: 		.asciz "Bloques crecientes: "
str_flecha: 		.asciz " -> "
str_saltolinea: 	.asciz "\n"
str_tendencia: 		.asciz "Tendencia detectada desde índice "
str_no_tendencia: 	.asciz "Sin tendencias detectadas"

.text
.globl main
main:
    # Leer n
    la a0, entrada_n
    li a7, 4
    ecall
    li a7, 5
    ecall
    la t0, n
    sw a0, 0(t0)

    # Leer k
    la a0, entrada_k
    li a7, 4
    ecall
    li a7, 5
    ecall
    la t0, k
    sw a0, 0(t0)

    # Leer las temperaturas
    la a0, entrada_temps
    li a7, 4
    ecall
    la t1, temps      # Dirección base de temps
    lw t2, n          # t2 = n
    li t3, 0          # Contador i = 0

# Bucle para leer las n temperaturas
loop_lectura:
    bge t3, t2, fin_lectura
    li a7, 5          # syscall para leer entero
    ecall
    sw a0, 0(t1)      # Almacenar temperatura
    addi t1, t1, 4    # Siguiente posición
    addi t3, t3, 1    # i++
    j loop_lectura
    
fin_lectura:

    # --- Calculo de sumas móviles ---
    # Cargar n y k
    lw t1, n          # t1 = n
    lw t2, k          # t2 = k

    # Calcular max_i = n - k + 1
    sub t0, t1, t2
    addi t0, t0, 1    # t0 = max_i (limite para i)

    # Guardar k y max_i en el stack
    addi sp, sp, -8
    sw t2, 0(sp)      # Guardar k en sp+0
    sw t0, 4(sp)      # Guardar max_i en sp+4

    la t5, temps      # Dirección base de temps
    la t6, sumas      # Dirección base de sumas
    li t3, 0          # i = 0

loop_sumas:
    lw t0, 4(sp)      # t0 = max_i
    bge t3, t0, end_loop_sumas  # Si i >= max_i, terminar

    # Guardar i en el stack
    addi sp, sp, -4
    sw t3, 0(sp)      # Guardar i

    # Inicializar suma y contador
    lw t1, 4(sp)      # t1 = k (contador j)
    slli t0, t3, 2    # offset = i*4
    add t0, t5, t0    # t0 = temps[i]
    li t4, 0          # suma = 0

bloque_suma:
    beqz t1, guardar_suma # Si j == 0, terminar bloque
    lw t2, 0(t0)      # Cargar temps
    add t4, t4, t2    # sumar a suma
    addi t0, t0, 4    # siguiente temperatura
    addi t1, t1, -1   # decrementar contador j
    j bloque_suma

guardar_suma:
    slli t0, t3, 2    # offset = i*4
    add t0, t6, t0    # t0 = sumas[i]
    sw t4, 0(t0)      # guardar suma
    lw t3, 0(sp)      # recuperar i
    addi sp, sp, 4    # liberar espacio de i
    addi t3, t3, 1    # i++
    j loop_sumas

end_loop_sumas:
    # Liberar espacio de k y max_i
    addi sp, sp, 8

    # --- Detectar tendencia creciente ---
    # Calcular max_i = n - k + 1
    lw t1, n          # t1 = n
    lw t2, k          # t2 = k
    sub t0, t1, t2
    addi t0, t0, 1    # t0 = max_i
    addi t2, t0, -2   # t2 = max_i - 2 (límite para i)

    la t0, sumas       # Dirección base de sumas
    li t1, 0          # i = 0
    li t5, -1         # inicio_tend = -1 (por defecto)

loop_verificado:
    bge t1, t2, end_verificacion  # Si i >= max_i-2, terminar

    # Cargar sumas consecutivas
    slli t3, t1, 2    # offset = i*4
    add t3, t0, t3    # t3 = sumas[i]
    lw t4, 0(t3)      # t4 = sumas[i]
    lw t6, 4(t3)      # t6 = sumas[i+1]
    lw t3, 8(t3)      # t3 = sumas[i+2]

    # Verificar sumas[i] < sumas[i+1] < sumas[i+2]
    bge t4, t6, not_inc  # Si sumas[i] >= sumas[i+1], saltar
    bge t6, t3, not_inc  # Si sumas[i+1] >= sumas[i+2], saltar

    # Tendencia encontrada
    mv t5, t1         # inicio_tend = i
    j end_verificacion

not_inc:
    addi t1, t1, 1    # i++
    j loop_verificado

end_verificacion:
    la t6, inicio_tend
    sw t5, 0(t6)      # Guardar resultado

    # --- Impresión de resultados ---
    # Primero imprimir todas las sumas móviles
    la a0, str_bloques
    li a7, 4
    ecall

    # Calcular número de bloques (max_i = n - k + 1)
    lw t0, n          # t0 = n
    lw t1, k          # t1 = k
    sub t2, t0, t1    # t2 = n - k
    addi t3, t2, 1    # t3 = max_i (total bloques)

    li t1, 0          # Contador de bloques (i=0)
    la t4, sumas      # Dirección base de sumas

loop_impresion:
    bge t1, t3, fin_impresion  # Terminar si i >= max_i

    # Imprimir suma actual
    slli t5, t1, 2    # Offset = i * 4
    add t5, t4, t5    # Dirección de sumas[i]
    lw a0, 0(t5)      # Cargar valor
    li a7, 1          # syscall para imprimir entero
    ecall

    addi t1, t1, 1    # i++
    beq t1, t3, fin_impresion  # Saltar separador si es último

    # Imprimir separador de la flecha " -> "
    la a0, str_flecha
    li a7, 4
    ecall

    j loop_impresion

fin_impresion:
    # Nueva línea después de las sumas
    la a0, str_saltolinea
    li a7, 4
    ecall

    # Ahora mprimir detección de tendencia
    lw t1, inicio_tend # Cargar índice de tendencia
    li t2, -1
    beq t1, t2, no_tendencia  # Saltar si no hay tendencia

    # Caso con tendencia detectada
    la a0, str_tendencia
    li a7, 4
    ecall
    mv a0, t1         # Cargar índice de inicio
    li a7, 1          # syscall imprimir entero
    ecall
    j exit

no_tendencia:
    la a0, str_no_tendencia
    li a7, 4
    ecall

exit:
    # Hacer un línea final y terminar programa
    la a0, str_saltolinea
    li a7, 4
    ecall
    li a7, 10         # syscall para terminar la ejecucón
    ecall
