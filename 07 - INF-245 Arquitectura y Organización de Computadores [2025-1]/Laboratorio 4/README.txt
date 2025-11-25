Nombres:
José Valenzuela, Rol: -, P201
Sergio Cárcamo, Rol: -, P200

AVISO: Mi compañero (José Valenzuela) congeló en el útlimo plazo, por lo que la entrega fue desarrollada en gran medida por mí (Sergio Cárcamo).

Para el desafío 1, se obtienen las entradas, y se calculan las sumas móviles con el loop "loop_sumas". 
En cada iteración, el programa calcula la suma de un bloque de k temperaturas comenzando desde un índice y guarda la suma correspondiente en el arreglo "sumas". 
Después de calcular las sumas, el codigo busca tres sumas consecutivas en el arreglo "sumas" que sean estrictamente crecientes.
Si encuentra una subsecuencia creciente, almacena el índice como el inicio de la tendencia en "inicio_tend" (en caso contrario, se indica que no se encontró.).
Finalmente se imprimen las sumas y si hubo una tendencia, imprime el índice de esta.

Para el desafío 2, el codigo lee una entrada y elimina el salto de línea. El código recorre la cadena y cuenta cuántas letras mayúsculas (A-Z) y cuántos dígitos (0-9) hay en la entrada.
Los contadores "t0" y "t1" almacenan los valores para mayúsculas y dígitos, respectivamente.
Luego, se valida que la cantidad de letras mayúsculas sea par y la cantidad de dígitos sea impar (usando la instrucción andi). Si no se cumplen ambas condiciones, el código imprime "Código inválido" y termina.
Si el código es válido, se procede a encriptar el mensaje aplicando un XOR con la clave 0x5A a cada carácter de la cadena. Esto se hace en el bucle "loop_encriptacion" con la instrucción xori a0, t3, 0x5A.
Después de aplicar XOR, cada byte resultante se convierte a formato hexadecimal (con el prefijo 0x), utilizando las funciones "mostrar_byte_hex" y "convertir_hex".
El mensaje "Código encriptado" es impreso con el texto, y cada carácter cifrado se muestra en formato hexadecimal.
Si la validación fue exitosa, el programa muestra el mensaje de "Código válido", y luego finaliza. En caso contrario, muestra el mensaje de "Código inválido".

SUPUESTOS:
Para el desafío 1, se asume que todas las entradas son válidas y dentro de los rangos establecidos por el enunciado. Además, las entradas temperaturas son recibidas cada una en una linea.
O sea, luego de una temperatura se debe presionar Enter para hacer un salto de línea e ingresar la siguiente.

En el desafío 2, se toma en consideración el error en la salida del ejemplo del enunciado.

En ambos casos se usa la pseudoinstrucción "ecall" para entrada/salida y para finalizar el programa (ecall es una syscall o llamada al sistema que lee el valor del registro a7 y según este realiza una acción).

