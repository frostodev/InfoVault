#lang scheme

;; Función que descifra los mensajes en una matriz, con la función auxiliar de recursión simple.
;;
;; mensajes: la 'matriz' de mensajes
;; lista_funciones: la lista que contiene las funciones a usar
(define (descifrar_simple mensajes lista_funciones)
  (define (procesar_matriz mensajes lista_funciones) ;; Función interna para procesar la matriz
    (if (null? mensajes) ;; Si la lista de mensajes está vacía, retornar una lista vacía
        '()
        (cons (procesar_fila_simple (car mensajes) (car lista_funciones)) ;; Si hay mensajes en la lista, procesar la fila actual y seguir procesando el resto de la matriz
              (procesar_matriz (cdr mensajes) (cdr lista_funciones)))))
  (procesar_matriz mensajes lista_funciones)) ;; Llamar a la función interna con los mensajes y las funciones dadas

;; Función auxiliar que toma una fila (una lista) y una función, usando recursión simple
;;
;; fila: fila a trabajar
;; funcion: función a usar
(define (procesar_fila_simple fila funcion)
  (define (suma j m) ;; Función interna para calcular la suma
    (if (> j m) ;; Si el índice 'j' es mayor que la longitud de la fila
        0 ;; Retornar 0 (caso base)
        (+ (funcion (list-ref fila (- j 1))) (suma (+ j 1) m)))) ;; Sumar el resultado de aplicar la función a cada elemento de la fila y recursivamente continuar sumando
  (let ((m (length fila))) ;; Usar 'let' para definir 'm' como la longitud de la fila
    (modulo (floor (/ (suma 1 m) m)) 10))) ;; Retornar el módulo del promedio de la suma de los elementos de la fila

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


;; Función que descifra los mensajes en una matriz, con la función auxiliar de recursión de cola.
;;
;; mensajes: la 'matriz' de mensajes
;; lista_funciones: la lista que contiene las funciones a usar
(define (descifrar_cola mensajes lista_funciones) ;; Función interna que toma la matriz de mensajes, la lista de funciones y un acumulador.
  (define (procesar_matriz mensajes lista_funciones acc)
    (if (null? mensajes) ;; Si la lista de mensajes está vacía, devuelve el acumulador invertido.
        (reverse acc)
        (procesar_matriz (cdr mensajes) (cdr lista_funciones) ;; Si no, llama recursivamente a procesar_matriz con la cola de mensajes y de lista de funciones, y agrega el resultado de procesar_fila_cola al acumulador.
                         (cons (procesar_fila_cola (car mensajes) (car lista_funciones)) acc))))
  (procesar_matriz mensajes lista_funciones '())) ;; Llamar a la función procesar_matriz con los argumentos proporcionados.

;; Función auxiliar que toma una fila (una lista) y una función, usando recursión de cola
;;
;; fila: fila a trabajar
;; funcion: función a usar
(define (procesar_fila_cola fila funcion)
  (define (suma j m a) ;; Definición de la función interna suma que toma un índice, la longitud de la fila y un acumulador.
    (if (> j m) ;; Si el índice supera la longitud de la fila, devuelve el acumulador.
        a
        (suma (+ j 1) m (+ a (funcion (list-ref fila (- j 1))))))) ;; Si no, llamar recursivamente a suma con el siguiente índice, la misma longitud de fila y el acumulador actualizado sumando el valor de la función aplicada al elemento correspondiente de la fila.
  (let ((m (length fila)))  ;; Llamada a la función interna suma con el índice 1 y la longitud de la fila, y toma el módulo de la división entera del resultado entre 10.
  (modulo (floor (/ (suma 1 m 0) m)) 10)))