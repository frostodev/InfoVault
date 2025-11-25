.data
# Entrada y mensajes
entrada:   	 	.space 65	# 64 caracteres + null terminator
valido_msg:      	.asciz "Código válido\n"
invalido_msg:    	.asciz "Código inválido\n"
encriptado_msg1: 	.asciz "Código encriptado: ["
coma_espacio:    	.asciz ", "
encriptado_msg2:	.asciz "]\n"

.text
.globl main

main:
    # Leer codigo de mision de entrada
    li a7, 8
    la a0, entrada
    li a1, 65
    ecall

    # Eliminar salto de linea de la entrada
    la t0, entrada
remover_saltolinea:
    lbu t1, 0(t0)
    addi t0, t0, 1
    bne t1, x0, remover_saltolinea 
    li t1, '\n'
    lbu t2, -2(t0)            	# Cargar penúltimo carácter
    bne t2, t1, inicio_contadores   # Si no es salto de linea, saltar
    sb zero, -2(t0)           	# Reemplazar el salto de linea con zero

inicio_contadores:
    # Contadores
    li t0, 0             # Contador mayúsculas
    li t1, 0             # Contador dígitos
    la t2, entrada  	 # Puntero a string

loop_contadores:
    lbu t3, 0(t2)        # Cargar caracter
    beq t3, x0, verificar_valido 
    
    # Verificar mayúscula (A-Z)
    li t4, 'A'
    blt t3, t4, not_upper
    li t4, 'Z'
    bgt t3, t4, not_upper
    addi t0, t0, 1       # Incrementar contador mayúsculas
    j siguiente_char

not_upper:
    # Verificar dígito (0-9)
    li t4, '0'
    blt t3, t4, siguiente_char
    li t4, '9'
    bgt t3, t4, siguiente_char
    addi t1, t1, 1       # Incrementar contador digitos

siguiente_char:
    addi t2, t2, 1       # Siguiente caracter
    j loop_contadores

verificar_valido:
    # Validar mayúsculas par y dígitos impar
    andi t4, t0, 1       # t4 = t0 % 2
    andi t5, t1, 1       # t5 = t1 % 2
    bne t4, x0, invalido  # si mayúsculas no es par
    beq t5, x0, invalido  # si dígitos no es impar

    # Código valido, entonces preparar encriptación
    la a0, encriptado_msg1
    li a7, 4
    ecall                # Imprimir la parte 1 del mensaje (lo de "Codigo encriptado: [")

    la t2, entrada  	 # Resetear puntero
    li t4, 0             # Primer elemento flag

loop_encriptacion:
    lbu t3, 0(t2)
    beq t3, x0, end_encriptacion # fin del string
    
    # Separador para elementos 2+ 
    beq t4, x0, saltarse_coma  	# si es primer byte
    la a0, coma_espacio 	# si no, entonces mostrar la coma y el espacio para separar los elementos
    li a7, 4
    ecall
saltarse_coma:
    li t4, 1 # Marcar que no es primer elemento

    # Aplicar XOR y convertir a hex
    xori a0, t3, 0x5A   	# XOR con clave 0x5A
    jal mostrar_byte_hex   	# Convertir e imprimir byte

    addi t2, t2, 1       # Siguiente caracter
    j loop_encriptacion

end_encriptacion:
    la a0, encriptado_msg2	# mostrar el "]" y un salto de línea para cerrar el mensaje
    li a7, 4
    ecall

    la a0, valido_msg
    j mostrar_msg

invalido:
    la a0, invalido_msg

mostrar_msg:
    li a7, 4
    ecall

exit:
    li a7, 10	# Terminar el programa con el syscall
    ecall
    
mostrar_byte_hex:
    # Función para imprimir byte en formato hexadecimal (0xXX)
    addi sp, sp, -16     # Reservar espacio en stack
    sw ra, 0(sp)
    sw t5, 4(sp)         # Preservar t5 (usado en convertir_hex)
    sw t6, 8(sp)         # Preservar t6 (el valor temporal)
    
    mv t6, a0            # Guardar byte
    li a0, '0'
    li a7, 11
    ecall                # Imprimir '0'
    li a0, 'x'
    ecall                # Imprimir 'x'

    # Nibble alto
    srli a0, t6, 4
    jal convertir_hex      # Convertir e imprimir nibble alto

    # Nibble bajo
    mv a0, t6
    andi a0, a0, 0x0F
    jal convertir_hex      # Convertir e imprimir nibble bajo

    lw t6, 8(sp)         # Restaurar registros
    lw t5, 4(sp)
    lw ra, 0(sp)
    addi sp, sp, 16      # Liberar stack
    ret

convertir_hex:
    # Función para convertir nibble a carácter hexadecimal
    li t5, 9
    bgt a0, t5, letra
    addi a0, a0, '0'    # Convertir a dígito
    j imprimir_caracter
letra:
    addi a0, a0, 55     # Convertir a letra (A-F)
imprimir_caracter:
    li a7, 11           # Configurar para imprimir carácter
    ecall
    ret
