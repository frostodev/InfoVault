#lang scheme

;; Implementa el recorrido de un Stratatree con una secuencia de movimientos especificada
;;
;; secuencia: lista con secuencia de movimientos
;; arbol: arbol al que hay que recorrer
(define (stratatree secuencia arbol)
  (define (moverse sec subarbol) ;; Función interna para moverse a través del árbol según la secuencia de movimientos
    (if (null? sec) ;; Si la secuencia esta vacía, retornar el contenido del subárbol actual
        (cadr subarbol)
        (let ((movimiento (car sec))
              (nodos (cdr subarbol)))
          (let ((subarbol-sig (encontrar-subarbol movimiento nodos))) ;; Buscamos el siguiente subárbol según el movimiento actual
            (if (equal? subarbol-sig #f) ;; Si no encontramos un subárbol válido
                #f ;; Entonces retornar false
                (moverse (cdr sec) subarbol-sig)))))) ;; Llamar recursivamente a 'moverse' con la secuencia restante y el siguiente subárbol
  (if (and (not (null? secuencia)) (equal? (car secuencia) 'S))
      (moverse (cdr secuencia) arbol) ;; Si la primera acción es 'S', comenzamos el movimiento desde el árbol dado
      #f)) ;; Si no hay acciones o la primera no es 'S', retornamos #f

;; Busca un subárbol específico dentro de una lista de nodos de un árbol, basado en un movimiento dado
;;
;; movimiento: Movimiento que se busca dentro de los nodos
;; nodos: Lista de nodos que representan el árbol
(define (encontrar-subarbol movimiento nodos)
   (cond ((null? nodos) #f) ;; Si la lista de nodos está vacía, retornamos #f (no se encontró el movimiento)
         ((equal? (car (car nodos)) movimiento) (car nodos)) ;; Si el movimiento coincide con el nodo actual, retornamos ese nodo
         (else (encontrar-subarbol movimiento (cdr nodos))))) ;; En caso contrario, seguimos buscando en el resto de la lista de nodos
